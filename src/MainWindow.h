#pragma once

#include <QMainWindow>

#include "CanvasWidget.h"
#include "Document.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupMenus();
    CanvasWidget* canvas;
    Document* document;
};