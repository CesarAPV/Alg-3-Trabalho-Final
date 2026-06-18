#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int insert(Heap *h, int *indexer, No no) {
    if(h->size == h->capacity) {
        printf("Error: heap full!\n");
        return -1;
    }

    int i = h->size;
    h->size++;

    h->array[i] = no;

    indexer[no.x] = i;
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

    memset(h->array, 0, sizeof(No) * capacity);

    return h;
}

/* Funcao que atualiza a posicao de um no 'descendo'
 * ele pelo heap, ate encontrar a posicao correta */
void go_down(Heap *h, int *indexer, int i) {
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
void go_up(Heap *h, int *indexer, int i) {
    while(i != 0 && h->array[i].key < h->array[parent(i)].key) {
        swap(&h->array[i], &h->array[parent(i)]);
        indexer[h->array[i].x] = i;
        indexer[h->array[parent(i)].x] = parent(i);
        i = parent(i);
    }
}

/* funcao que remove o primeiro no do heap, e atualiza
 * os indexers do nos modificados no processo */
int heap_remove(Heap *h, No *no, int *indexer) {
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
void update_heap(Heap *h, int *indexer, int x, float new_key) {
    int pos = indexer[x];
    float old_key = h->array[pos].key;
    h->array[pos].key = new_key;

    if(new_key > old_key)
        go_down(h, indexer, pos);
    else if(new_key < old_key)
        go_up(h, indexer, pos);
}

int main(int argc, char *argv[]) {
    int n_points;       // numero de pontos que serao lidos

    scanf("%d", &n_points);
    /* Vetor que indexa cada posicao do heap
     * por exemplo se i[1] -> 4, significa que
     * o ponto com abscissa x = 1 esta na posicao 4
     * do heap */
    int *indexer = malloc(sizeof(int) * n_points);
    // Vetor auxiliar que possui todas as coordenadas
    float *values = malloc(sizeof(float) * n_points);
    Heap *heap = create_heap(n_points);
    memset(indexer, 0, sizeof(int) * n_points);
    memset(values, 0, sizeof(float) * n_points);

    // le todos os valores: O(n)
    for(int i = 0; i < n_points; i++) {
        scanf("%f", &values[i]);
    }

    No no;
    // insere os pontos de [1..n-2] no heap
    // baseando-se em seus erros
    for(int i = 1; i < n_points - 1; i++) {
        float key = area_triangulo(i-1, values[i-1], i, values[i], i+1, values[i+1]);
        no = (No){i, values[i], key};
        indexer[i] = insert(heap, indexer, no);
    }


    // para testes: modifique o numero que esta na condicao do while
    while(heap->size > 0 && heap->array[0].key < 1.5){
        heap_remove(heap, &no, indexer);

        // i eh o indice do ponto removido
        int i = no.x;
        /* l e r point sao os vizinhos desse ponto.
         * a principio sao i-1 e i+1, mas podem estar
         * 'removidos' da lista, defini como removidos
         * aqueles que sao indexados para -1 */
        int l_point = i - 1, ll_point;
        int r_point = i + 1, rr_point;
        // ll e rr point sao os vizinhos dos vizinhos

        indexer[i] = -1;
        while(l_point > 0 && indexer[l_point] == -1 ) l_point--;
        while(r_point < (n_points-1) && indexer[r_point] == -1) r_point++;

        if(l_point > 0) {
            ll_point = l_point - 1;
            while(ll_point > 0 && indexer[ll_point] == -1) ll_point--;
            float l_key = area_triangulo(ll_point, values[ll_point], l_point, values[l_point], r_point, values[r_point]);
            // atualiza o ponto indexado por l_point com nova chave l_key
            update_heap(heap, indexer, l_point, l_key);
        }

        if(r_point < n_points - 1) {
            rr_point = r_point + 1;
            while(rr_point < n_points -1 && indexer[rr_point] == -1) rr_point++;
            float r_key = area_triangulo(l_point, values[l_point], r_point, values[r_point], rr_point, values[rr_point]);
            // atualiza o ponto indexaod por r_point com nova chave r_key
            update_heap(heap, indexer, r_point, r_key);
        }
    }


    // imprime os pontos restantes, incluindo sempre as bordas pelo menos
    printf("%d %f\n", 0, values[0]);
    while(heap->size != 0) {
        heap_remove(heap, &no, indexer);
        printf("%d %f \t %f\n", no.x, no.y, no.key);
    }
    printf("%d %f\n", n_points-1, values[n_points-1]);

    free(heap->array);
    free(heap);
    free(indexer);
    free(values);
    return 0;
}
