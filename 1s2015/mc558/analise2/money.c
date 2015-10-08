/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Laboratório 6: O Problema do Troco
    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dijkstra.h"

#define MAXCOINS 150

typedef struct Coins
{
    int index;
    int value;
    int weight;
}Coins;

int sort_coins(const void *a, const void *b) 
{ 
    Coins *p1 = (Coins *)a;
    Coins *p2 = (Coins *)b;
    return (p1->value) - (p2->value);
} 

void create_path( Grafo *g, Coins coins[], int ncoins, int change);

int main(void) {
    //clock_t end,begin = clock();
    Grafo *g;
    Edge **P;
    int i, np, dist;
    int change, ncoins, c[MAXCOINS];
    Coins coins[MAXCOINS];

    scanf("%d %d",&ncoins,&change);
    for(i = 0; i < ncoins; i++){
        scanf("%d %d",&coins[i].value,&coins[i].weight);
        coins[i].index = i;
        c[i] = 0;
    }

    // sort coins by value
    qsort(coins,ncoins,sizeof(Coins), sort_coins);

    // source = ncoins*(change+1)
    // dest = ncoins*(change+1) - 1
    g = novoGrafo(ncoins*(change+1) + 1);

    create_path(g,coins,ncoins,change);

    dist = dijkstra(g,ncoins*(change+1),ncoins*(change+1)-1, &P, &np);
    printf("%d\n", dist);
    
    for (i = 0; i < np; i++){
        c[coins[i].index] = P[i]->w / coins[i].weight;
    }

    for(i = 0; i < ncoins; i++){
        if( i + 1 == ncoins)
            printf("%d\n",c[i]);
        else
            printf("%d ",c[i]);
    }
    //end = clock();
    //printf("%f\n",(double)(end - begin) / CLOCKS_PER_SEC);
	return 0;
}

void create_path( Grafo *g, Coins coins[], int ncoins, int change){
    int i,j,k;
    int mult;
    int from,to;

    // source to first layer
    for( i = 0; i <= change; i++ ){
        adicionaAresta(g,ncoins*(change+1),i,i*coins[0].weight);
    }

    for( i = 0; i < ncoins - 1; i++){
        for( j = 0; j <= change; j++){
            adicionaAresta(g,j + i*(change+1),j + (i + 1)*(change+1),0);
            for( k = j + coins[i+1].value; k <= change; k += coins[i+1].value){
                if( (k - j) % coins[i+1].value == 0 ){
                    mult = (k - j) / coins[i+1].value;
                    from = j + i*(change+1);
                    to = k + (i+1)*(change+1);
                    adicionaAresta(g,from,to,mult*coins[i+1].weight);
                }
            }
        }
    }

    return;
}



