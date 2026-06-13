#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int id; // "x"
    float key; // "y"
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


// - Da para usar um ponteiro para função aqui, por causa do 'tipo'
// - Precisa corrigir a operação lógica, pois estou calculando a área
// com base no tipo, porém não estou inserindo/construindo a heap
// simultaneamente
void calcula_area_for_heap_array(Heap *h, No *vet, int tipo){

    float x1, y1, x2, y2, x3, y3;
    if (tipo = 'a'){
        h->array[0].id = &vet[0].id;
        h->array[0].key = vet[0].key;
        for(int i = 1; i < h->size; i ++){
            x1 = vet[i-1].id;
            y1 = vet[i-1].key;
            x2 = vet[i].id;
            y2 = vet[i].key;   
            x3 = vet[i+1].id;
            y3 = vet[i+1].key;
            h->array[i].id = &vet[i].id;
            h->array[i].key = area_triangulo(x1,y1,x2,y2,x3,y3);        
        }
        h->array[h->size--].id = &vet[h->size--].id;
        h->array[h->size--].key = vet[h->size--].key;
    }
    else{
        h->array[0].id = &vet[0].id;
        h->array[0].key = vet[0].key;
        for(int i = 1; i < h->size; i ++){
            x1 = vet[i-1].id;
            y1 = vet[i-1].key;
            x2 = vet[i].id;
            y2 = vet[i].key;   
            x3 = vet[i+1].id;
            y3 = vet[i+1].key;
            h->array[i].id = &vet[i].id;
            h->array[i].key = altura_triangulo(x1,y1,x2,y2,x3,y3);        
        }
        h->array[h->size--].id = &vet[h->size--].id;
        h->array[h->size--].key = vet[h->size--].key;
    }
}

// Função principal do programa
// Pede o erro_base a ser retirado;
// Retira o no min_heap se for menor que o erro;
// Pede para recalcular a área...
//...->tanto a direita quanto a esquerda do no a ser substituído;
void process_execute(Heap *h, float erro, int tipo){
    
    
}

int main(int argc, char *argv[]) {

    if (argc < 3){
        printf("Uso correto da entrada: './programa' 'tipo' 'Erro'\n");
        return 1;
    }
    
    // Acho que será necessário fazer casting com função que converte
    // string em inteiro/float;
    int tipo = atoi(argv[1]);
    float erro = atof(argv[2]);
    int tam_entrada;

    scanf("%d\n", &tam_entrada);

    No *vet = malloc(sizeof(No) * tam_entrada);
    Heap *h = create_heap(tam_entrada);

    No d1  = {1,  15.5 };
    vet[d1.id--] = d1;
    //h->array[0] = d1;
    No d2  = {2,  10.3 };
    vet[d2.id--] = d2;
    //h->array[1] = d2;
    No d3  = {3,  40.3 };
    vet[d3.id--] = d3;
    //h->array[2] = d3;
    No d4  = {4,  5.6  };
    vet[d4.id--] = d4;
    //h->array[3] = d4;
    No d5  = {5,  231.2};
    vet[d5.id--] = d5;
    //h->array[4] = d5;
    No d6  = {6,  1.2  };
    vet[d6.id--] = d6;
    //h->array[5] = d6;
    No d7  = {7,  23.1 };
    vet[d7.id--] = d7;
    //h->array[6] = d7;
    No d8  = {8,  0.2  };
    vet[d8.id--] = d8;
    //h->array[7] = d8;
    No d9  = {9,  4.3  };
    vet[d9.id--] = d9;
    //h->array[8] = d9;
    No d10 = {10, 2.1  };
    vet[d10.id--] = d10;
    //h->array[9] = d10;

    calcula_area_for_heap_array(h, vet, tipo);
    process_execute(h,erro, tipo);


    insert(h, &d1);
    insert(h, &d2);
    insert(h, &d3);
    insert(h, &d4);
    insert(h, &d5);
    insert(h, &d6);
    insert(h, &d7);
    insert(h, &d8);
    insert(h, &d9);
    insert(h, &d10);

    No aux = {0};
    while(h->size > 0) {
        heap_remove(h, &aux);
        print_no(aux);
    }
    

    free(h->array);
    free(h);
    return 0;
}
