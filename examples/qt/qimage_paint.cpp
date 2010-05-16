#include "qimage_paint.h"

using namespace libcan;

QImage qimage_from_canvas(canvas& c) {
    matrix<unsigned char> my_matrix = c.get_matrix(0,1,2,3);

    int height = my_matrix.get_height();
    int width = my_matrix.get_width();


    QImage res(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; ++y) {
        for (int x=0; x<width;++x) {
            //QRgb color(1,2,3,4);
            QRgb color= qRgba(my_matrix.get(x,y,0), my_matrix.get(x,y,1), my_matrix.get(x,y,2), my_matrix.get(x,y,3));
            res.setPixel(x,y,color);
        }
    }

    return res;
}

