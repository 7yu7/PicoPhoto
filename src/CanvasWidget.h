#pragma once

#include <QWidget>

#include "Document.h"

class CanvasWidget : public QWidget {
    Q_OBJECT

    public:
        CanvasWidget(QWidget *parent = nullptr);
        void centerView();
        
        Document* document;
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        bool event(QEvent *event) override;
        
    private:
        void zoom(double value);

        int x_pos = 0;
        int y_pos = 0;
        double scale = 1.0;
        QPoint mouse_pos;
};