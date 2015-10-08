#include <stdio.h>
#include<queue>
#include<vector>
#include<iostream>
#include<cstring>
#include <time.h>
#include<math.h>

#define DEBUG false

using namespace std; 
    
int cca[2000];  //sendo aqui o custo do caminho atual, apenas para manter o track no calculo
int c[2][2000][2000];
int coriginal[2][2000][2000];
int x[2000];
int y[2000];
int aguenta[2000];
int npeng[21000];
int fluxomax;
vector<int> graph[2000];
int visitado[2000];
int pi[2000];
int saida[2000];       

//--------------------------------------------------------------------------
int bfsAlterada(int terminal, int source){
		  int atual;
		  unsigned int i;
		  
	//setando todos com valores iniciais
		memset(pi, -1, sizeof(pi));
    	memset(cca, 0, sizeof(cca));
    	memset(visitado, 0, sizeof(visitado));
      //for(j=0; j<na; j++){
      //		pi[j]=-1;
      //		visitado[j]=0;
      //		cca[j]=0;	
     // }
      queue<int> fila;
      cca[source] = 10000;//valor maximo possível de custo
      fila.push(source);
      pi[source] = -1;

		while(!fila.empty()){
	
				atual = fila.front();
				fila.pop();
				for(i=0; i<graph[atual].size(); i++){
            	//printf("fez bfs %d\n", i);
                if(visitado[graph[atual][i]]==0) {//ainda nao visitado
                    if( c[0][atual][ graph[atual][i] ] - c[1][atual][ graph[atual][i] ] > 0) {
                    		//calculo do flow utilizado (minimo dentre o caminho percorrido)
                        cca[ graph[atual][i] ] = min(cca[atual],c[0][atual][graph[atual][i]] - c[1][atual][graph[atual][i]]);
                        pi[graph[atual][i]] = atual; //prossegue com lista de Pais
                        if( graph[atual][i] == terminal) {
                        	 
                            return cca[terminal];//se for o terminal, ja retornamos a capacidade (ja a minima)
                        }
                        fila.push(graph[atual][i]); //senao prossegue adicionando o vizinho na fila
                        visitado[atual]=1;	
							}
						}
				}
		}
        //senão retorna zero
        return 0;
    }

     
//----------------------------------------------------------------------
int main(int argc, char *argv[]){
    	
   int it=0, nv, totalpeng = 0;
   float dpulo;
   int atual, anterior;
   int u, v, fluxo = 0, contsaida=0;
	int k1, k2;
   scanf("%d %f", &nv, &dpulo);
    		
   for(it = 0; it<nv; it++) {
		scanf("%d %d %d %d", &x[it], &y[it], &aguenta[it], &npeng[it]);
		totalpeng = totalpeng + npeng[it];
	}
	
	for(u = 0; u<nv; u++) {
		for(v = 0; v<nv; v++){
			if((   sqrt( pow((x[u]-x[v]),2) + pow((y[u]-y[v]),2) ) <= dpulo )  &&  (u!=v)  )
				graph[u].push_back(u+nv); //adiciona as arestas plausiveis para os cubos de gelo espelho
				c[0][u][u+nv]= aguenta[u];//quantos podem pular
				coriginal[0][u][u+nv]= aguenta[u];
				graph[u+nv].push_back(v); //liga os cubos de gelo espelho ate os cubos que realmente ha o caminho
				c[0][u+nv][v]= aguenta[u];//quantos podem pular
				coriginal[0][u+nv][v]= aguenta[u];
				    //isto eh feito para ao rodar o algoritmo de fluxo maximo, nao movimentar um pinguim que ja 
					//foi movimentado
							
		if(npeng[u]>0){
			 graph[2*nv].push_back(u); //ligo o source universal até os blocos originais
			 c[0][2*nv][u] = npeng[u]; //flow da aresta vindo da source é o numero de penguins que o vertice seguinte possui
		}
	 }
  }	

//execuita edmonds karp para cada  vertice
	for(it=0; it<nv;it++){
			printf("iteracao %d\n", it);
	      //algoritmo lab A de edmonds karp
	      while(true) {
	            fluxo = bfsAlterada(it, 2*nv);
	            if(DEBUG)
	        			printf("%d\n", fluxomax);
	            if (fluxo == 0) {//ja nao existe mais caminhos com fluxo disponivel em graph
	                break;
	            }
	
	            fluxomax = fluxomax + fluxo;
	            atual = it;//nele faremos o caminho inverso que ficou gravado pela bfs
	            	
	            while(atual != 2*nv){
	                anterior = pi[atual];
	                c[1][anterior][atual] += fluxo;
	                c[1][atual][anterior] -= fluxo;
	                if(DEBUG)
	        					printf("%d\n", fluxomax);
	                atual = anterior; //prossegue no caminho inverso
	            }
	      }
	      if(fluxomax >= totalpeng){
				saida[contsaida]=it;
				contsaida++;
	      }
	      //reinicio todos valores de custo iniciais e zero a residual e vets auxiliares
	      for(k1=0; k1<(2*nv); k1++){
	      	for(k2=0; k2<(2*nv); k2++){
	      		c[1][k1][k2] = 0;
	      		c[0][k1][k2] = coriginal[0][k1][k2];
	      	}
	      	pi[k1]=-1;
	      	cca[k1]=0;
	      	visitado[k1]=0;
	      }
	}
	//impressao
	for(it=0; it<contsaida;it++) 
		printf("%d ", saida[it]);
	it++;
	printf("%d", saida[it]);
}

