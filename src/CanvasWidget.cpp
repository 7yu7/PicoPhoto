#include "CanvasWidget.h"

#include <QPainter>
#include <QMouseEvent>


CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(100, 100);
    document = new Document();
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.translate(x_pos, y_pos);
    painter.scale(scale, scale);
    painter.drawImage(0, 0, document->image);
}

void CanvasWidget::centerView() {
    scale = 0.95 * std::min(((double) width())/document->image.width(), ((double) height())/document->image.height()) ;
    x_pos = width() / 2 - scale * document->image.width() / 2;
    y_pos = height() / 2 - scale * document->image.height() / 2;
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    QPoint new_mouse_pos = event->pos();

    if (event->buttons() & Qt::MiddleButton) {
        x_pos += new_mouse_pos.x() - mouse_pos.x();
        y_pos += new_mouse_pos.y() - mouse_pos.y();
    }

    mouse_pos = new_mouse_pos;
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