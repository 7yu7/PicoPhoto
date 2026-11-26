#pragma once

#include <QImage>

class Document {
public:
    Document();
    void loadImage(const QString &path);
    bool saveImage(const QString &path) const;

    QImage image;
};