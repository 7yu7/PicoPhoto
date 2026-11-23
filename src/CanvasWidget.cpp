#include "CanvasWidget.h"

#include <QPainter>

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(100, 100);
    document = new Document();
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.drawImage(0, 0, document->image);
}