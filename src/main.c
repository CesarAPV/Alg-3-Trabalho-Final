#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/Heap.h"
#include "../inc/Utility.h"

#define FLAG_HEIGHT 1
#define FLAG_AREA 2

#define REMOVED -1

float calc_key(Heap *heap, int l, int m, int r, int flag) {
    Index *indexer = heap->indexer;

    if(flag == FLAG_HEIGHT)
        return altura_triangulo(l, indexer[l].y, m, indexer[m].y, r, indexer[r].y);
    else
        return area_triangulo(l, indexer[l].y, m, indexer[m].y, r, indexer[r].y);
}

/**
 * Funcao que insere todos os pontos do vetor values no heap, exceto os pontos de borda,
 * usando a flag para determinar qual valor sera usado como chave
 */
void fill_heap(Heap *heap, int n_points, int flag) {
    No node;
    for(int i = 1; i < n_points - 1; i++) {
        node.key = calc_key(heap, heap->indexer[i].left, i, heap->indexer[i].right, flag);
        node.x = i;
        /* Indexa o nó com x = i na fila de prioridade */
        heap->indexer[i].current = insert(heap, node);
    }
}

/* Atualiza os vizinhos do no Nó cuidando para não ultrapassar as bordas */
void update_neighbors(Heap *heap, No *no, int n_points, int flag) {
    // i é o indice do ponto removido
    int i = no->x;

    /* Pega os vizinhos a esquerda e a direita do ponto removido */
    int l_point = heap->indexer[i].left;
    int r_point = heap->indexer[i].right;

    /* Marca o ponto i como removido
     * Corrige os indexes para seus vizinho */
    heap->indexer[i].current = REMOVED;

    if(r_point > 0 && r_point < n_points)
        heap->indexer[r_point].left = l_point;
    if(l_point > 0)
        heap->indexer[l_point].right = r_point;

    /* Garante que possa calcular um triangulo à esquerda */
    if (l_point > 0) {
        // Pega o vizinho a esquerda do vizinho a esquerda do ponto removido
        int ll_point = heap->indexer[l_point].left;
        // atualiza o ponto indexado por l_point com nova chave l_key
        float l_key = calc_key(heap, ll_point, l_point, r_point, flag);
        update_heap(heap, l_point, l_key);
    }

    /* Garante que possa calcular um triangulo à direita */
    if (r_point < n_points - 1) {
        // Pega o vizinho a direita do vizinho a direita do ponto removido
        int rr_point = heap->indexer[r_point].right;
        // atualiza o ponto indexado por r_point com nova chave r_key
        float r_key = calc_key(heap, l_point, r_point, rr_point, flag);
        update_heap(heap, r_point, r_key);
    }
}

void print_result(int n_points, int n_points_end, Index *indexer) {
    // Imprime a quantidade total de pares
    printf("%d\n", (n_points_end+2));

    /* Imprime cada par ordenado (X, Y) na ordem correta do eixo X *
     * Só imprime se o ponto não foi REMOVIDO */
    for (int i = 0; i < n_points; i++) {
        if (indexer[i].current != REMOVED) {
            printf("%.1f %g\n", (float)(i + 1), indexer[i].y);
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

    // Cria um heap com n pontos
    Heap *heap = create_heap(n_points);

    /* Inicializa todos os campos do index para pontos interiores */
    if (n_points > 0) {
        /* ler y da borda esquerda */
        heap->indexer[0].left = 0;
        heap->indexer[0].right = (n_points > 1) ? 1 : 0;
        heap->indexer[0].current = -2;
        scanf("%f", &heap->indexer[0].y);
    }

    for (int i = 1; i < n_points - 1; i++) {
       heap->indexer[i].left = i - 1;
       heap->indexer[i].right = i + 1;
       heap->indexer[i].current = -2; /* Ainda nao aponta para as pos no heap */

       /* Le y do ponto i: O(n) no total */
       scanf("%f", &heap->indexer[i].y);
    }

    if (n_points > 1) {
        /* ler y da borda direita */
        heap->indexer[n_points - 1].left = n_points - 2;
        heap->indexer[n_points - 1].right = n_points - 1;
        heap->indexer[n_points - 1].current = -2;
        scanf("%f", &heap->indexer[n_points - 1].y);
    }

    // Insere os pontos de [1..n-2] no heap, usando o calculo de chave definido pela flag
    fill_heap(heap, n_points, flag);

    No no;
    while(heap->size > 0 && heap->array[0].key < max_error){
        // Remove o ponto com menor chave do heap, ou seja, o ponto com menor erro
        printf("key: %f\n", heap->array[0].key);
        heap_remove(heap, &no);
        update_neighbors(heap, &no, n_points, flag);
    }

    // Imprime os pontos restantes, incluindo sempre as bordas pelo menos
    print_result(n_points, heap->size, heap->indexer);

    destroy_heap(heap);
    return 0;
}
