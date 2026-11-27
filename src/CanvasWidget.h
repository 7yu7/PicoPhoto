#pragma once

#include <QWidget>

#include "Document.h"
#include "BrushSettings.h"

class CanvasWidget : public QWidget {
    Q_OBJECT

    public:
        CanvasWidget(QWidget *parent = nullptr);
        void centerView();
        
        Document* document;
        BrushSettings brushSettings;

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        bool event(QEvent *event) override;
        
    private:
        void zoom(double value);
        QPointF screenToDocument(const QPointF &screenPos) const;
        void drawStroke(const QPointF &from, const QPointF &to);

        int x_pos = 0;
        int y_pos = 0;
        double scale = 1.0;
        QPointF mouse_pos;
};
