/* Copyright 2014, Gurobi Optimization, Inc. */

#include <math.h>
#include <iostream> 
#include <fstream> 
#include <cstdlib>
#include <cstring>
#include <time.h>
#include "gurobi_c++.h"

#define N 20
#define QTD_IN 20
#define ALPHA 5

using namespace std;


void seleciona_propagandas(int n, double C, double V[N], double P[N], double w[N][N], int S[N], double *UpperBound, long maxtime) {
	GRBVar y[N][N];
	GRBVar x[N];

	int seed=0;
	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env);
	model.getEnv().set(GRB_IntParam_Seed, seed);
	model.set(GRB_StringAttr_ModelName, "Alocacao Propaganda"); // prob. name
	model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE); // is a minimization problem  

	for(int i = 0; i < n; i++) {
		char name[100];
		sprintf(name,"x_%d",i);
		x[i] = model.addVar(0.0, 1.0, V[i],GRB_BINARY, name); 
	}

	for( int i = 0; i < n; i++) {

		for(int j = i+1; j < n; j++) {
			char name[100];
			sprintf(name,"y_%d_%d",i, j);
			y[i][j] = model.addVar(0.0, 1.0, w[i][j], GRB_BINARY, name); 
		}
	}
	
	model.update();
	
	GRBLinExpr expr;
	for(int i = 0; i < n; i++) {
		expr += (x[i] * P[i]);
	}
	model.addConstr(expr <= C);
	model.update();
	
	for(int i = 0; i < n; i++) {
		
		for(int j =i+1; j < n; j++) {
			model.addConstr(y[i][j] <= x[i]);
			model.addConstr(y[i][j] <= x[j]);
			model.addConstr(x[i] + x[j] <= (y[i][j] + 1));
		}
	}
	model.update();
	
	model.write("model.lp");
	
	model.optimize();
	
	*UpperBound = model.get(GRB_DoubleAttr_ObjVal);
	
}

int main(){
	srand48(1);
	
	srand(time(NULL));
		
	int numArestas = (N*(N - 1))/2;
	//A capacidade da mochila é o valor C=100 para todos os itens.
	double C = 100;
	
	char fname[100];
	sprintf(fname,"arq_e%d_n%d_m%d_c%.0lf.in",QTD_IN, N, numArestas, C);
	
	FILE *fp;
	fp = fopen(fname,"w");   /* Arquivo ASCII, para escrita */
	if(!fp)
	{
		printf( "Erro na abertura do arquivo");
		exit(0);
	}
	
	
	for(int nin = 0; nin < QTD_IN; nin++) {
		double P[N];
		double V[N];
		int S[N];
		double w[N][N];
		double otimo;
		
		for(int i = 0; i < N; i++) {
			
			double k;
			double d, low = (0.2*C), high = (0.5*C);
			d = (double) rand () / ((double) RAND_MAX + 1);
			k = d * (high - low + 1);
			
			//O peso de um item é igual a um valor aleatório real dentre 0,2*C e 0,5*C
			P[i] = low + k;
			
			low = 0.75;
			high = 1.25;
			d = (double) rand () / ((double) RAND_MAX + 1);
			k = d * (high - low + 1);
			
			//O valor dos itens é gerado igual ao peso dele vezes um valor aleatório entre [0,75 e 1,25].			
			V[i] = P[i] * (low + k);  
		}

		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				w[i][j] = 0.0;
			}  
		}

		for(int i=0; i<N; i++) {
			for(int j = i+1; j<N; j++) {
				
				double k;
				double d, low = (-(ALPHA/(double)10) * (sqrt(V[i]) + sqrt(V[i])));
				double high = ((ALPHA/(double)10) * (sqrt(V[i]) + sqrt(V[i])));
				d = (double) rand () / ((double) RAND_MAX + 1);
				k = d * (high - low + 1);
			
				//E se um arquivo foi gerado com parâmetro alpha, então os pesos w_i_j dele são calculados 
				//como um valor aleatório no intervalo 
				//[ -(alpha/10) * [sqrt(v_i)+sqrt(v_j) ; (alpha/10) * [sqrt(v_i)+sqrt(v_j)]
				w[i][j] = low + k;
			}  
		}

		seleciona_propagandas(N, C, V, P, w, S, &otimo, 100000);    

		fprintf(fp, "%d %d %.2lf %.3lf\n", N, numArestas,  C, otimo);
		for(int i = 0; i < N; i++) {
			//v_1 peso_vertice(v_1) valor_vertice(v_1
			fprintf(fp, "v_%d %.3lf %.3lf\n", i, P[i], V[i]);
		}
		
		for(int i = 0; i < N; i++) {
			for(int j = i+1; j < N; j++) {
				// e_u e_v valor_aresta(e_u,e_v)
				fprintf(fp, "e_%d e_%d %.3lf\n", i, j, w[i][j]);
			}
		}
	}
	
	fclose(fp);
	
	return 0;
}

