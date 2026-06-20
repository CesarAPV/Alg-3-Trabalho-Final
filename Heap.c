#include "Heap.h"
#include <string.h>

void swap(No *a, No *b) {
    No temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int i) {
    return (i-1)/2;
}


/* funcao que insere um no no heap, e atualiza o
 * indexer dos nos que foram modificados no processo */
int insert(Heap *h, No no) {
    if(h->size == h->capacity) {
        printf("Error: heap full!\n");
        return -1;
    }

    int i = h->size;
    h->size++;

    h->array[i] = no;

    h->indexer[no.x] = i;
    while(i != 0 && h->array[i].key < h->array[parent(i)].key) {
        swap(&h->array[i], &h->array[parent(i)]);
        indexer[h->array[i].x] = i;
        indexer[h->array[parent(i)].x] = parent(i);
        i = parent(i);
    }

    return i;
}

/* funcao que cria um heap vazio com capacidade maxima definida */
Heap *create_heap(int capacity) {
    Heap *h = (Heap*) malloc(sizeof(Heap));
    h->capacity = capacity;
    h->size = 0;
    h->array = malloc(sizeof(No) * capacity);
    h->indexer = malloc(sizeof(Index) * capacity);

    memset(h->array, 0, sizeof(No) * capacity);
    memset(h->indexer, 0, sizeof(Index) * capacity);

    return h;
}

/* Funcao que atualiza a posicao de um no 'descendo'
 * ele pelo heap, ate encontrar a posicao correta */
void go_down(Heap *h, int i) {
    int small = i;
    int left  = 2*i + 1;
    int right = 2*i + 2;

    if(left < h->size && h->array[left].key < h->array[small].key)
        small = left;
    if(right < h->size && h->array[right].key < h->array[small].key)
        small = right;

    if(small != i) {
        swap(&h->array[i], &h->array[small]);
        indexer[h->array[i].x] = i;
        indexer[h->array[small].x] = small;
        go_down(h, indexer, small);
    }
}


/* funcao simetrica a anterior */
void go_up(Heap *h, int i) {
    while(i != 0 && h->array[i].key < h->array[parent(i)].key) {
        swap(&h->array[i], &h->array[parent(i)]);
        indexer[h->array[i].x] = i;
        indexer[h->array[parent(i)].x] = parent(i);
        i = parent(i);
    }
}

/* funcao que remove o primeiro no do heap, e atualiza
 * os indexers do nos modificados no processo */
int heap_remove(Heap *h, No *no) {
    if(h->size <= 0) {
        printf("Erro: heap empty\n");
        no->x = -1;
        return -1;
    }
    if(h->size == 1) {
        h->size--;
        *no = h->array[0];
        return -1;
    }

    *no = h->array[0];
    h->array[0] = h->array[h->size-1];
    indexer[h->array[0].x] = 0;
    h->size--;

    go_down(h, indexer, 0);
    return 0;
}

/* funcao que atualiza a posicao de um no que teve sua chave alterada */
void update_heap(Heap *h, int x, float new_key) {
    int pos = indexer[x];
    float old_key = h->array[pos].key;
    h->array[pos].key = new_key;

    if(new_key > old_key)
        go_down(h, indexer, pos);
    else if(new_key < old_key)
        go_up(h, indexer, pos);
}
