#include <iostream>
#include <stdio.h>
#include<queue>
#include<vector>
#include<iostream>
#include<cstring>
#include<math.h>

#include "dijcpp.h"

using namespace std;

typedef struct moedas{
    int valor,peso;
}Moedas;

Moedas m[150]; // vetor de struct para armazenar as moedas

int main(void) {
    int it,it2,it3,totalmoedas,troco;

    scanf("%d %d",&totalmoedas,&troco);
    for(it = 0; it < totalmoedas; it++){
        scanf("%d %d",&m[it].valor,&m[it].peso);
    }

    Grafo g(totalmoedas*(troco+1) + 1);

    for( it = 0; it <= troco; it++ ){ // adiciona aresta do vertice fonte a todas as combinacoes da primeira moeda
        if( it%m[0].valor != 0 ){
            g.adicionaAresta(totalmoedas *(troco+1), it, 1000*150 + 1); // caso nao exista um multiplo, esse caminho nao pode ser percorrido
        }
        else{
            g.adicionaAresta(totalmoedas *(troco+1), it, (it/m[0].valor)*m[0].peso);
        }
    }

    for( it = 0; it < totalmoedas - 1; it++){
        for( it2 = 0; it2 <= troco; it2++){
            g.adicionaAresta(it2 + it * (troco + 1), it2 + (it+1)*(troco+1), 0); // vertices no mesmo nivel sao sempre conectados com zero
            for( it3 = it2 + m[it+1].valor; it3 <= troco; it3 += m[it+1].valor){
                if( (it3 - it2) % m[it+1].valor == 0 ){ // se exitir uma combinacao inteira da proxima moeda que nao ultrapasse o valor de troco
                    g.adicionaAresta(it2 + it*(troco+1) , it3 + (it+1)*(troco+1), ((it3 - it2) / m[it+1].valor)*m[it+1].peso);
                }
            }
        }
    }


    vector<Grafo::Edge> P;
    int dist = g.dijkstra(totalmoedas*(troco+1),totalmoedas*(troco+1)-1, P);
    cout << dist << endl;
    it2 = 0;

    for (vector<Grafo::Edge>::iterator it = P.begin(); it != P.end(); it++){
        //cout << "(" << it->x << ", " << it->y << ", " << it->w << ") ";
        cout << it->w / m[it2].peso;
        if( it + 1 == P.end())
            cout << endl;
        else{
            cout << " ";
        }
        it2++;
    }
    
    return 0;

}
