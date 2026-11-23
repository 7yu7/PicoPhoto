#include "Document.h"

Document::Document() {
    image = QImage(600, 400, QImage::Format_ARGB32);
    image.fill(Qt::white);
}