#ifndef UTILITY_H
#define UTILITY_H

/* Funcao que calcula a area de um triangulo formado por tres pontos usando determinantes */
float area_triangulo(float x1, float y1, float x2, float y2, float x3, float y3 );

/* Funcao que calcula o quadrado da altura de um ponto em relacao a base formada pelos seus vizinhos */
float altura_triangulo(float x1, float y1,float x2, float y2, float x3, float y3);

#endif