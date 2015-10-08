#ifndef __DIJKSTRA_H
#define __DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#define INFINITO 999999999

typedef struct st_edge Edge;

struct st_edge {
    int x, y, w;
    Edge *prox;
};

typedef struct {
    int n;
	int nv;
    Edge **adj;
} Grafo;

/* cria um novo grafo vazio */
Grafo *novoGrafoVazio();

/* cria um novo grafo com n vertices */
Grafo *novoGrafo(int n);

/* adiciona um vertice ao grafo
 * devolve o indice do novo vertice */
int adicionaVertice(Grafo *g);

/* adiciona uma aresta de x a y com peso w
   0 <= x,y <= n-1  */
void adicionaAresta(Grafo *g, int x, int y, int w);

/* para facilitar seu debug */
void printGrafo(Grafo *g);

/* calcula caminho minimo de s a t
   devolve o valor do caminho minimo
   coloca o caminho em P e o numero de arestas em np
   */
int dijkstra(Grafo *g, int s, int t, Edge ***P, int *np);




/* ----- IMPLEMENTACAO ----- */

/* vetor que cresce exponencialmente -- amortizado constante */
#define MULTVETOR 1024
void aumentaVetor(Grafo *g, int n) {
    if (g->nv >= n) return;
	while (g->nv < n)
    	g->nv *= 2;
    g->adj = realloc(g->adj, (g->nv)*sizeof(Edge *));
}

Grafo *novoGrafoVazio() {
	Grafo *g = malloc(sizeof(Grafo));
	g->n = 0;
	g->nv = MULTVETOR;
	g->adj = malloc(MULTVETOR*sizeof(Edge *));
	return g;
}

Grafo *novoGrafo(int n) {
    int i;
	Grafo *g;
	g = novoGrafoVazio();
	aumentaVetor(g, n);
    g->n = n;
    for (i = 0; i < g->n; i++)
        g->adj[i] = NULL;
    return g;
}

int adicionaVertice(Grafo *g) {
	g->n++;
	aumentaVetor(g, g->n);
	g->adj[(g->n) -1] = NULL;
	return (g->n) -1;
}

void adicionaAresta(Grafo *g, int x, int y, int w) {
    Edge *e = malloc(sizeof(Edge));
    e->x = x;
    e->y = y;
    e->w = w;
    e->prox = g->adj[x];
    g->adj[x] = e;
}

void printGrafo(Grafo *g) {
    int i;
    Edge *e;

    printf("# vertices: %d\n", g->n);
    for (i = 0; i < g->n; i++) {
        printf("%d: ", i);
        for (e = g->adj[i]; e != NULL; e = e->prox)
            printf("(%d, %d, %d) ", e->x, e->y, e->w);
        printf("\n");
    }
}

int *heap;
int *i_heap;
int n_heap;

void troca_heap(int i, int j) {
    int temp;
    i_heap[heap[j]] = i; i_heap[heap[i]] = j;
    temp = heap[j]; heap[j] = heap[i]; heap[i] = temp;
}

void corrige_descendo(int key[], int i) {
    int j;

    j = i;
    while (2*j+1 < n_heap) {
        int f;
        f = 2*j+1;
        if (f+1 < n_heap && key[heap[f]] > key[heap[f+1]])
            f++;
        if (key[heap[j]] <= key[heap[f]])
            break;
        else {
            troca_heap(j, f);
            j = f;
        }
    }
}

void corrige_subindo(int key[], int i) {
    while (i > 0 && key[heap[(i-1)/2]] > key[heap[i]]) {
        troca_heap(i, (i-1)/2);
        i = (i-1)/2;
    }
}

int removemin(int key[]) {
    int m;

    m = heap[0];
    heap[0] = heap[--n_heap];
    i_heap[heap[0]] = 0;

    corrige_descendo(key, 0);
    return m;
}

void buildheap(int n) {
    int i;

	free(heap); free(i_heap);
	heap = malloc(n * sizeof(int));
	i_heap = malloc(n * sizeof(int));

    n_heap = n;
    for (i = 0; i < n; i++)
        heap[i] = i_heap[i] = i;
}

void diminui(int key[], int v, int chave) {
    key[v] = chave;
    corrige_subindo(key, i_heap[v]);
}

int dijkstra(Grafo *g, int s, int t, Edge ***P, int *np) {
    int i;
	int *d;
	Edge **pai, *e;

	d = malloc((g->n)*sizeof(int));
	pai = malloc((g->n)*sizeof(Edge*));
	*P = malloc((g->n)*sizeof(Edge*));

    for (i = 0; i < g->n; i++) {
        d[i] = INFINITO;
		pai[i] = NULL;
    }

    buildheap(g->n);

    d[s] = 0;
    diminui(d, s, 0);

    while (n_heap > 0) {
        int u;

        u = removemin(d);

        for (e = g->adj[u]; e != NULL; e = e->prox) {
            int v = e->y;
            if (d[v] > d[u] + e->w) {
				pai[v] = e;
                d[v] = d[u] + e->w;
                diminui(d, v, d[v]);
            }
        }
    }

	*np = 0;
	for (e = pai[t]; e != NULL; e = pai[e->x])
		(*P)[(*np)++] = e;

	/* coloca o caminho na ordem correta */
	for (i = 0; i < (*np)/2; i++) {
		Edge *temp;
		temp = (*P)[i];
		(*P)[i] = (*P)[*np-1-i];
		(*P)[*np-1-i] = temp;
	}

	return d[t];
}

#endif /* __DIJKSTRA_H */
