#include "Utility.h"

float area_triangulo(float x1, float y1, float x2, float y2, float x3, float y3 ){
    float area;
    area = (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2;

    return(area < 0.0) ? - area : area;
}

float altura_triangulo(float x1, float y1,float x2, float y2, float x3, float y3){
    float area, dx, dy;

    area = area_triangulo(x1, y1, x2, y2, x3, y3);
    dx = x3 - x1;
    dy = y3 -y1;
    return 4 * (area * area) / (dx * dy + dy * dy);
}