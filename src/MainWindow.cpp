#include "MainWindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QWidgetAction>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QShortcut>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PicoPhoto");
    resize(800, 600);

    document = Document();
    brush = Brush();

    canvas = new CanvasWidget(this, &document, &brush);
    setCentralWidget(canvas);
    setupMenus();
    show();
    canvas->centerView();
}

void MainWindow::setupMenus() {

    // File Menu
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
            document.loadImage(path);
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
            document.saveImage(path);
        }
    });

    // Brush Menu
    QWidget *brushBar = new QWidget(this);
    auto *layout = new QHBoxLayout(brushBar);
    layout->setContentsMargins(6, 2, 6, 2);
    layout->setSpacing(8);

    layout->addWidget(new QLabel("Color:", brushBar));
    QPushButton *colorButton = new QPushButton(brushBar);
    colorButton->setFixedSize(20, 20);
    colorButton->setStyleSheet("background-color: black; border: 1px solid #666;");
    layout->addWidget(colorButton);

    QObject::connect(colorButton, &QPushButton::clicked, this, [this, colorButton]() {
        QColor newColor = QColorDialog::getColor(brush.color, this, "Select Color", QColorDialog::ShowAlphaChannel);
        if (newColor.isValid()) {
            brush.color = newColor;
            colorButton->setStyleSheet(
                QString("background-color: %1; border: 1px solid #666;").arg(newColor.name(QColor::HexArgb))
            );
        }
    });

    layout->addWidget(new QLabel("Size:", brushBar));
    QSpinBox *sizeSpin = new QSpinBox(brushBar);
    sizeSpin->setRange(1, 200);
    sizeSpin->setValue(5);
    sizeSpin->setSuffix(" px");
    layout->addWidget(sizeSpin);

    connect(sizeSpin, &QSpinBox::valueChanged, this, [this](int value) {
        brush.size = value;
        canvas->rebuildBrushOutline();
    });

    auto *decSize = new QShortcut(QKeySequence(Qt::Key_BracketLeft), this);
    connect(decSize, &QShortcut::activated, this, [sizeSpin]() {sizeSpin->setValue(sizeSpin->value() - 1);});

    auto *incSize = new QShortcut(QKeySequence(Qt::Key_BracketRight), this);
    connect(incSize, &QShortcut::activated, this, [sizeSpin]() {sizeSpin->setValue(sizeSpin->value() + 1);});

    QCheckBox *aaCheck = new QCheckBox("Antialiasing", brushBar);
    layout->addWidget(aaCheck);

    connect(aaCheck , &QCheckBox::toggled, this, [this](bool checked) {
        brush.antialiased = checked;
    });

    menuBar()->setCornerWidget(brushBar, Qt::TopRightCorner);
}