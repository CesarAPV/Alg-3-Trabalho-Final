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

float area_triangulo(float x1, float y1,
                     float x2, float y2,
                     float x3, float y3){
    float area;
    area = (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2;
    
    return(area < 0.0) ? - area : area;
}

float altura_triangulo(float x1, float y1,
                     float x2, float y2,
                     float x3, float y3){

    float area, dx, dy;

    area = area_triangulo(x1, y1, x2, y2, x3, y3);
    dx = x3 - x1;
    dy = y3 -y1;
    return 4 * (area * area) / (dx * dy + dy * dy);
}

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

// Retorna o id do min para verificar possível fim do programa;
int id_Heap_min(Heap *h){
    No *aux;
    *aux = h->array[0];
    return aux->id;
}

// Função principal do programa
// Pede o erro_base a ser retirado;
// Retira o no min_heap se for menor que o erro;
// Pede para recalcular a área...
//...->tanto a direita quanto a esquerda do no a ser substituído;
void process_execute(Heap *h, No *vet){
    
    float Erro;
    int controle_id;
    int flag_erro = 0;

    scanf("%f\n", &Erro);

    controle_id = id_Heap_min(h);
    // Laço de execução da modificação;
    while(controle_id != 1 && controle_id != h->capacity && !flag_erro){
    }
    

}
int main() {
    
    int tam_entrada;
    scanf("%d\n", &tam_entrada);

    No *vet = malloc(sizeof(No) * tam_entrada);
    Heap *h = create_heap(tam_entrada);

    No d1  = {1,  15.5 };
    vet[d1.id--] = d1;
    No d2  = {2,  10.3 };
    vet[d2.id--] = d2;
    No d3  = {3,  40.3 };
    vet[d3.id--] = d3;
    No d4  = {4,  5.6  };
    vet[d4.id--] = d4;
    No d5  = {5,  231.2};
    vet[d5.id--] = d5;
    No d6  = {6,  1.2  };
    vet[d6.id--] = d6;
    No d7  = {7,  23.1 };
    vet[d7.id--] = d7;
    No d8  = {8,  0.2  };
    vet[d8.id--] = d8;
    No d9  = {9,  4.3  };
    vet[d9.id--] = d9;
    No d10 = {10, 2.1  };
    vet[d10.id--] = d10;

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

    process_execute(h,vet);

    No aux = {0};
    while(h->size > 0) {
        heap_remove(h, &aux);
        print_no(aux);
    }
    

    free(h->array);
    free(h);
    return 0;
}
