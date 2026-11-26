#include "Document.h"

#include <QDebug>

Document::Document() {
    image = QImage(600, 400, QImage::Format_ARGB32);
    image.fill(Qt::white);
}

void Document::loadImage(const QString &path) {
    QImage loaded;
    if (loaded.load(path)) {
        image = loaded;
    } else {
        qDebug() << "Failed to load image from" << path;
    }
}

bool Document::saveImage(const QString &path) const {
    if (!image.save(path)) {
        qDebug() << "Failed to save image to" << path;
        return false;
    }
    return true;
}
