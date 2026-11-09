#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PicoPhoto");
    resize(800, 600);

    show();
}