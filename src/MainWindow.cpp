#include "MainWindow.h"
#include "CanvasWidget.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PicoPhoto");
    resize(800, 600);

    CanvasWidget* canvas = new CanvasWidget(this);
    setCentralWidget(canvas);
    show();
    canvas->centerView();
}