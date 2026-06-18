#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>


typedef struct no {
    int     x;
    float   y;
    float   key;
} No;

typedef struct _heap {
    No      *array;
    int      size;
    int      capacity;
} Heap;


Heap *create_heap(int capacity);
void destroy_heap(Heap *h);

int insert(Heap *h, int *indexer, No no);
void update_heap(Heap *h, int *indexer, int x, float new_key);
int heap_remove(Heap *h, No *no, int *indexer);
void go_down(Heap *h, int *indexer, int i);
void go_up(Heap *h, int *indexer, int i);

#endif 