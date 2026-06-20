#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Heap.h"
#include "Utility.h"


#define FLAG_HEIGHT 1
#define FLAG_AREA 2

#define REMOVED -1

/**
 * Funcao que insere todos os pontos do vetor values no heap, exceto os pontos de borda,
 * usando a flag para determinar qual valor sera usado como chave
 */
void entries_to_heap(Heap *heap, int *indexer, float *values, int n_points, int flag) {
    No node;
    for(int i = 1; i < n_points - 1; i++) {
        float key;

        if(flag == FLAG_HEIGHT)
            key = altura_triangulo(i-1, values[i-1], i, values[i], i+1, values[i+1]);
        else
            key = area_triangulo(i-1, values[i-1], i, values[i], i+1, values[i+1]);
        node = (No){i, values[i], key};

        indexer[i] = insert(heap, indexer, node);
    }
}

/**
 * Funcao que busca o vizinho a esquerda de um ponto, ou seja, o proximo ponto a esquerda que nao foi 'removido' do heap
 */
int search_left_neighbor(int *indexer, int i) {
    while(i > 0 && indexer[i] == REMOVED) i--;
    return i;
}

/**
 * Funcao que busca o vizinho a direita de um ponto, ou seja, o proximo ponto a direita que nao foi 'removido' do heap
 */
int search_neighbor_right(int *indexer, int i, int n_points) {
    while(i < (n_points-1) && indexer[i] == REMOVED) i++;
    return i;
}

void update_neighbors(Heap *heap, No *no, int *indexer, float *values, int n_points, int flag) {
    // i é o indice do ponto removido
    int i = no->x;

    /* l e r point sao os vizinhos desse ponto.
     * a principio sao i-1 e i+1, mas podem estar
     * 'removidos' da lista, defini como removidos
     * aqueles que sao indexados para -1 */
    int l_point = i - 1;
    int r_point = i + 1;
    // ll e rr point sao os vizinhos dos vizinhos

    // Marca o ponto como removido
    indexer[i] = REMOVED;

    // Busca os vizinhos a esquerda e a direita do ponto removido, achando um que não esteja removido
    l_point = search_left_neighbor(indexer, l_point);
    r_point = search_neighbor_right(indexer, r_point, n_points);

    if (l_point > 0) {
        // Busca o vizinho a esquerda do vizinho a esquerda do ponto removido
        int ll_point = l_point - 1;
        ll_point = search_left_neighbor(indexer, ll_point);

        // atualiza o ponto indexado por l_point com nova chave l_key
        float l_key;
        if( flag == FLAG_HEIGHT)
            l_key = altura_triangulo(ll_point, values[ll_point], l_point, values[l_point], r_point, values[r_point]);
        else
            l_key = area_triangulo(ll_point, values[ll_point], l_point, values[l_point], r_point, values[r_point]);

        update_heap(heap, indexer, l_point, l_key);
    }

    if (r_point < n_points - 1) {
        // Busca o vizinho a direita do vizinho a direita do ponto removido
        int rr_point = r_point + 1;
        rr_point = search_neighbor_right(indexer, rr_point, n_points);

        // atualiza o ponto indexaod por r_point com nova chave r_key
        float r_key;
        if( flag == FLAG_HEIGHT)
            r_key = altura_triangulo(l_point, values[l_point], r_point, values[r_point], rr_point, values[rr_point]);
        else
            r_key = area_triangulo(l_point, values[l_point], r_point, values[r_point], rr_point, values[rr_point]);

        update_heap(heap, indexer, r_point, r_key);
    }
}

void print_result(int n_points, int n_points_end, int *indexer, float *values) {
    // Imprime a quantidade total de pares
    printf("%d\n", (n_points_end+2));

    // Imprime cada par ordenado (X, Y) na ordem correta do eixo X
    for (int i = 0; i < n_points; i++) {
        if (indexer[i] != -1) {
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
    if (!strcmp(argv[1], "-h")) {
        flag = FLAG_HEIGHT;
    } else if (!strcmp(argv[1], "-a")) {
        flag = FLAG_AREA;
    } else {
        printf("Flag desconhecida: %s\n", argv[1]);
        return 0;
    }

    // Valor maximo para remover um ponto do heap, ou seja, o erro maximo permitido
    float max_error = atof(argv[2]);

    // numero de pontos que serao lidos
    int n_points;       
    scanf("%d", &n_points);

    /* Vetor que indexa cada posicao do heap
     * por exemplo se i[1] -> 4, significa que
     * o ponto com abscissa x = 1 esta na posicao 4
     * do heap */
    int *indexer = malloc(sizeof(int) * n_points);
    memset(indexer, 0, sizeof(int) * n_points);

    // Vetor auxiliar que possui todas as coordenadas
    float *values = malloc(sizeof(float) * n_points);
    memset(values, 0, sizeof(float) * n_points);
    
    Heap *heap = create_heap(n_points);
    
    // Le todos os valores: O(n)
    for(int i = 0; i < n_points; i++) {
        scanf("%f", &values[i]);
    }

    // Insere os pontos de [1..n-2] no heap, usando o calculo de chave definido pela flag
    entries_to_heap(heap, indexer, values, n_points, flag);

    No no;  
    while(heap->size > 0 && heap->array[0].key < max_error){
        // Remove o ponto com menor chave do heap, ou seja, o ponto com menor erro
        heap_remove(heap, &no, indexer);

        update_neighbors(heap, &no, indexer, values, n_points, flag);
    }


    // Imprime os pontos restantes, incluindo sempre as bordas pelo menos
    print_result(n_points, heap->size, indexer, values);

    destroy_heap(heap);
    free(indexer);
    free(values);
    return 0;
}
