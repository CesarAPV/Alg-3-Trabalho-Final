#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/Heap.h"
#include "../inc/Utility.h"


#define FLAG_HEIGHT 1
#define FLAG_AREA 2

#define REMOVED -1

/**
 * Funcao que insere todos os pontos do vetor values no heap, exceto os pontos de borda,
 * usando a flag para determinar qual valor sera usado como chave
 */
void fill_heap(Heap *heap,  float *values, int n_points, int flag) {
    No node;
    for(int i = 1; i < n_points - 1; i++) {
        float key;

        if(flag == FLAG_HEIGHT)
            key = altura_triangulo(i-1, values[i-1], i, values[i], i+1, values[i+1]);
        else
            key = area_triangulo(i-1, values[i-1], i, values[i], i+1, values[i+1]);
        node = (No){i, values[i], key};
        printf("key: %f\n", key);

        /* Indexa o nó com x = i na fila de prioridade */
        heap->indexer[i].current = insert(heap, node);
    }
}

/* Funcao que retorna o indice do vizinho a esquerda de um ponto */
int search_left_neighbor(Index *indexer, int i) {
    return indexer[i].left;
}

/* Funcao que retorna o indice do vizinho a esquerda de um ponto */
int search_neighbor_right(Index *indexer, int i) {
    return indexer[i].right;
}

/* Atualiza os vizinhos do no Nó cuidando para não ultrapassar as bordas */
void update_neighbors(Heap *heap, No *no, float *values, int n_points, int flag) {
    // i é o indice do ponto removido
    int i = no->x;

    /* Pega os vizinhos a esquerda e a direita do ponto removido */
    int l_point = search_left_neighbor(heap->indexer, i);
    int r_point = search_neighbor_right(heap->indexer, i);
    // ll e rr point sao os vizinhos dos vizinhos

    /* Marca o ponto i como removido
     * Corrige os indexes para seus vizinho */
    heap->indexer[i].current = REMOVED;
    heap->indexer[heap->indexer[i].right].left = heap->indexer[i].left;
    heap->indexer[heap->indexer[i].left].right = heap->indexer[i].right;

    /* Garante que possa calcular um triangulo à esquerda */
    if (l_point > 0) {
        // Pega o vizinho a esquerda do vizinho a esquerda do ponto removido
        int ll_point = search_left_neighbor(heap->indexer, l_point);
        // atualiza o ponto indexado por l_point com nova chave l_key
        float l_key;
        if( flag == FLAG_HEIGHT)
            l_key = altura_triangulo(ll_point, values[ll_point], l_point, values[l_point], r_point, values[r_point]);
        else
            l_key = area_triangulo(ll_point, values[ll_point], l_point, values[l_point], r_point, values[r_point]);
        update_heap(heap, l_point, l_key);
    }

    /* Garante que possa calcular um triangulo à direita */
    if (r_point < n_points - 1) {
        // Pega o vizinho a direita do vizinho a direita do ponto removido
        int rr_point = search_neighbor_right(heap->indexer, r_point);
        // atualiza o ponto indexaod por r_point com nova chave r_key
        float r_key;
        if( flag == FLAG_HEIGHT)
            r_key = altura_triangulo(l_point, values[l_point], r_point, values[r_point], rr_point, values[rr_point]);
        else
            r_key = area_triangulo(l_point, values[l_point], r_point, values[r_point], rr_point, values[rr_point]);
        update_heap(heap, r_point, r_key);
    }
}

void print_result(int n_points, int n_points_end, Index *indexer, float *values) {
    // Imprime a quantidade total de pares
    printf("%d\n", (n_points_end+2));

    /* Imprime cada par ordenado (X, Y) na ordem correta do eixo X *
     * Só imprime se o ponto não foi REMOVIDO */
    for (int i = 0; i < n_points; i++) {
        if (indexer[i].current != REMOVED) {
            printf("%.1f %g\n", (float)(i + 1), values[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    // Verifica se o numero de argumentos esta correto
    if(argc < 3){
        printf("Uso correto: %s -{flag} {valor_de_ponto_flutuante}\n", argv[0]);
        return 0;
    }

    // Inteiro que define se o programa vai usar a area ou a altura do triangulo como chave do heap
    int flag;
    if (!strcmp(argv[1], "-h"))
        flag = FLAG_HEIGHT;
    else if (!strcmp(argv[1], "-a"))
        flag = FLAG_AREA;
    else {
        printf("Flag desconhecida: %s\n", argv[1]);
        return 0;
    }

    // Valor maximo para remover um ponto do heap, ou seja, o erro maximo permitido
    float max_error = atof(argv[2]);

    // numero de pontos que serao lidos
    int n_points;
    scanf("%d", &n_points);

    // Vetor auxiliar que possui todas as coordenadas
    float *values = malloc(sizeof(float) * n_points);
    memset(values, 0, sizeof(float) * n_points);

    // Cria um heap com n pontos
    Heap *heap = create_heap(n_points);

    /* Inicializa todos os campos do index */
    for (int i = 0; i < n_points; i++) {
       heap->indexer[i].left = i - 1;
       heap->indexer[i].right = i + 1;
       heap->indexer[i].current = -2; /* Ainda nao aponta para as pos no heap */
    }

    // Le todos os valores: O(n)
    for(int i = 0; i < n_points; i++) {
        scanf("%f", &values[i]);
    }

    // Insere os pontos de [1..n-2] no heap, usando o calculo de chave definido pela flag
    fill_heap(heap, values, n_points, flag);

    No no;
    while(heap->size > 0 && heap->array[0].key < max_error){
        // Remove o ponto com menor chave do heap, ou seja, o ponto com menor erro
        heap_remove(heap, &no);
        update_neighbors(heap, &no, values, n_points, flag);
    }

    // Imprime os pontos restantes, incluindo sempre as bordas pelo menos
    print_result(n_points, heap->size, heap->indexer, values);

    destroy_heap(heap);
    free(values);
    return 0;
}
