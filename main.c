#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int id;
    float key;
} No;

typedef struct _heap {
    No      *array;
    int      size;
    int      capacity;
} Heap;


void swap(No *a, No *b) {
    No temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int i) {
    return (i-1)/2;
}

void insert(Heap *h, No *no) {
    if(h->size == h->capacity) {
        printf("Erro: heap full!\n");
        return;
    }

    int i = h->size;
    h->size++;

    h->array[i] = *no;

    while(i != 0 && h->array[i].key < h->array[parent(i)].key) {
        swap(&h->array[i], &h->array[parent(i)]);
        i = parent(i);
    }
}

Heap *create_heap(int capacity) {
    Heap *h = (Heap*) malloc(sizeof(Heap));
    h->capacity = capacity;
    h->size = 0;
    h->array = malloc(sizeof(No) * capacity);

    return h;
}

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
        go_down(h, small);
    }
}

int heap_remove(Heap *h, No *no) {
    if(h->size <= 0) {
        printf("Erro: heap empty\n");
        no->id = -1;
        return -1;
    }
    if(h->size == 1) {
        h->size--;
        *no = h->array[0];
        return -1;
    }

    *no = h->array[0];
    h->array[0] = h->array[h->size-1];
    h->size--;

    go_down(h, 0);
    return 0;
}
void print_no(No no) {
    printf("ID: %d \tKey: %f\n", no.id, no.key);
}

int main() {
    Heap *h = create_heap(10);
    No d1  = {1,  15.5 };
    No d2  = {2,  10.3 };
    No d3  = {3,  40.3 };
    No d4  = {4,  5.6  };
    No d5  = {5,  231.2};
    No d6  = {6,  1.2  };
    No d7  = {7,  23.1 };
    No d8  = {8,  0.2  };
    No d9  = {9,  4.3  };
    No d10 = {10, 2.1  };

    insert(h, &d1);
    insert(h, &d2);
    insert(h, &d3);
    insert(h, &d4);
    insert(h, &d5);
    insert(h, &d6);
    insert(h, &d7);
    insert(h, &d8);
    insert(h, &d9);
    insert(h, &d4);


    No aux = {0};
    while(h->size > 0) {
        heap_remove(h, &aux);
        print_no(aux);
    }
    

    free(h->array);
    free(h);
    return 0;
}
