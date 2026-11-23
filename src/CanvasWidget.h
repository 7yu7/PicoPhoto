#pragma once

#include <QWidget>

#include "Document.h"

class CanvasWidget : public QWidget {
    Q_OBJECT

public:
    CanvasWidget(QWidget *parent = nullptr);
    Document* document;

protected:
    void paintEvent(QPaintEvent *event) override;
};