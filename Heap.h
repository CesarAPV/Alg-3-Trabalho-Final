#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>

/* Estrutura que representa um ponto, guardando seu x original, valor y e a chave (erro) */
typedef struct no {
    int     x;
    float   y;
    float   key;
} No;

typedef struct index {
    int     left;
    int     current;
    int     right;
} Index;


/* Estrutura do min-heap que armazena os nos em um vetor dinâmico e controla o tamanho */
typedef struct _heap {
    No      *array;
    int      size;
    int      capacity;
    Index    *indexer;
} Heap;


/* Funcao que cria um heap vazio com capacidade maxima definida */
Heap *create_heap(int capacity);

/* Funcao que libera a memoria do vetor do heap e da estrutura do heap */
void destroy_heap(Heap *h);

/* Funcao que insere um no no heap e atualiza a posicao dele no indexer */
int insert(Heap *h, No no);

/* Funcao que altera a chave (erro) de um ponto e o move para a posicao correta no heap */
void update_heap(Heap *h, int x, float new_key);

/* Funcao que remove o ponto com menor erro (topo do heap) e atualiza os indexers */
int heap_remove(Heap *h, No *no);

/* Funcao que atualiza a posicao de um no 'descendo' ele pelo heap, ate encontrar a posicao correta */
void go_down(Heap *h, int i);

/* Funcao que atualiza a posicao de um no 'subindo' ele pelo heap, ate encontrar a posicao correta */
void go_up(Heap *h, int i);

#endif
