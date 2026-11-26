#include "MainWindow.h"

#include <QMenuBar>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PicoPhoto");
    resize(800, 600);

    canvas = new CanvasWidget(this);
    document = canvas->document;
    setCentralWidget(canvas);
    setupMenus();
    show();
    canvas->centerView();
}

void MainWindow::setupMenus() {
    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *openAction = fileMenu->addAction("Open");
    openAction->setShortcut(QKeySequence::Open);

    QAction *saveAction = fileMenu->addAction("Save as");
    saveAction->setShortcut(QKeySequence::SaveAs);

    connect(openAction, &QAction::triggered, this, [this]() {
        QString path = QFileDialog::getOpenFileName(
            this,
            "Select an Image",
            QString(),
            "Images (*.png *.jpg *.jpeg *.bmp)"
        );

        if (!path.isEmpty()) {
            document->loadImage(path);
            canvas->centerView();
        }
    });

    connect(saveAction, &QAction::triggered, this, [this]() {
        QString path = QFileDialog::getSaveFileName(
            this,
            "Save Image",
            QString(),
            "PNG Image (*.png);;JPEG Image (*.jpg *.jpeg)"
        );

        if (!path.isEmpty()) {
            document->saveImage(path);
        }
    });
}