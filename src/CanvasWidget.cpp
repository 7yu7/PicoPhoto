#include "CanvasWidget.h"

#include <QPainter>
#include <QMouseEvent>


CanvasWidget::CanvasWidget(QWidget *parent, Document* document, Brush* brush) : QWidget(parent), document(document), brush(brush) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::CrossCursor);
    setMinimumSize(100, 100);
    rebuildBrushOutline();
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.translate(x_pos, y_pos);
    painter.scale(scale, scale);
    painter.drawImage(0, 0, document->image);

    if (underMouse()) {
        QPen previewPen(Qt::white, 0);
        previewPen.setCosmetic(true);
        painter.setPen(previewPen);
        painter.setCompositionMode(QPainter::CompositionMode_Difference);
        painter.setRenderHint(QPainter::Antialiasing, false);

        painter.translate(std::floor((mouse_pos.x() - x_pos) / scale), std::floor((mouse_pos.y() - y_pos) / scale));
        painter.drawLines(brushOutline);
    }
}

void CanvasWidget::centerView() {
    scale = 0.95 * std::min(((double) width())/document->image.width(), ((double) height())/document->image.height()) ;
    x_pos = width() / 2 - scale * document->image.width() / 2;
    y_pos = height() / 2 - scale * document->image.height() / 2;
}

QPointF CanvasWidget::screenToDocument(const QPointF &screenPos) const {
    double offset = (brush->size % 2 == 0) ? 0 : 0.5;
    return QPointF(std::floor((screenPos.x() - x_pos) / scale) + offset,
                   std::floor((screenPos.y() - y_pos) / scale) + offset);
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    QPointF new_mouse_pos = event->position();
    if (event->buttons() & Qt::MiddleButton) {
        x_pos += new_mouse_pos.x() - mouse_pos.x();
        y_pos += new_mouse_pos.y() - mouse_pos.y();
    } else if (event->buttons() & Qt::LeftButton) {
        drawStroke(screenToDocument(mouse_pos), screenToDocument(new_mouse_pos));
    }

    mouse_pos = new_mouse_pos;
    update();
}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        drawStroke(screenToDocument(mouse_pos), screenToDocument(mouse_pos));
    }
    update();
}

void CanvasWidget::leaveEvent(QEvent *event) {
    update();
}

void CanvasWidget::wheelEvent(QWheelEvent *event) {
    if (!(QGuiApplication::queryKeyboardModifiers() & Qt::ControlModifier)) {
        zoom(event->angleDelta().y() * 0.001);
    } else {
        x_pos += event->angleDelta().x();
        y_pos += event->angleDelta().y();
    }
    update();
}

bool CanvasWidget::event(QEvent *event) {
    if (event->type() == QEvent::NativeGesture) {
        auto *gesture = static_cast<QNativeGestureEvent *>(event);

        if (gesture->gestureType() == Qt::ZoomNativeGesture) {
            zoom(gesture->value());
            return true;
        }
    }

    return QWidget::event(event);
}

void CanvasWidget::zoom(double value) {
    double old_scale = scale;
    scale *= std::exp(value);

    x_pos = mouse_pos.x() - (mouse_pos.x() - x_pos) * (scale / old_scale);
    y_pos = mouse_pos.y() - (mouse_pos.y() - y_pos) * (scale / old_scale);

    update();
}

void CanvasWidget::drawStroke(const QPointF &from, const QPointF &to) {
    QPainter painter(&document->image);
    painter.setRenderHint(QPainter::Antialiasing, brush->antialiased);
    painter.setPen(QPen(brush->color, brush->size,
                        Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if (from == to) {
        painter.drawPoint(from);
    } else {
        painter.drawLine(from, to);
    }
}

void CanvasWidget::rebuildBrushOutline() {
    brushOutline.clear();

    const int dim = brush->size;
    const int half = dim / 2;
    const double center = (dim - 1) / 2.0;

    auto isFilled = [&](int x, int y) {
        double dx = x - center;
        double dy = y - center;
        return dx * dx + dy * dy < (dim / 2.0) * (dim / 2.0);
    };

    for (int y = 0; y < dim; y++) {
        for (int x = 0; x < dim; x++) {
            if (!isFilled(x, y)) continue;

            int px = x - half;
            int py = y - half;

            if (!isFilled(x, y - 1)) brushOutline.append(QLineF(px, py, px + 1, py));
            if (!isFilled(x, y + 1)) brushOutline.append(QLineF(px, py + 1, px + 1, py + 1));
            if (!isFilled(x - 1, y)) brushOutline.append(QLineF(px, py, px, py + 1));
            if (!isFilled(x + 1, y)) brushOutline.append(QLineF(px + 1, py, px + 1, py + 1));
        }
    }

    update();
}