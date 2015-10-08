/* Copyright 2014, Gurobi Optimization, Inc. */
#include <stdlib.h>
#include <math.h>
#include <iostream> 
#include <fstream> 
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include "gurobi_c++.h"
#define N 100
using namespace std;

typedef struct Prg{
	double valor;
	double custo;
	double valorporcusto;
} Propaganda;

Propaganda p[N];
double W[N][N];
int size;
double max_lucro;
double max_custo;
int bestInclude[N];

bool pode_expandir(int i,double custo, double lucro){
	if(custo >= max_custo)
		return false;

	vector<GRBVar> x(N);
	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env);
	// disable standard output from Gurobi
	model.getEnv().set(GRB_IntParam_OutputFlag, 0);
	GRBLinExpr expr;
	// gives a name to the problem
	model.set(GRB_StringAttr_ModelName, "Knapsack Variation"); 
	// says that lp is a maximization problem
	// model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE); 
	for (int j = i; j <= size ;j++) {
		//x[j] = model.addVar(0, 1, 1,GRB_BINARY,"");
		x[j] = model.addVar(0, 1, NULL,GRB_CONTINUOUS,"");
		expr += p[j].custo * x[j];
	}
	// run update to use model inserted variables
	model.update(); 
	model.addConstr(expr <= max_custo - custo);
	// Process any pending model modifications.
	model.update(); 

	GRBQuadExpr obj;
	
	for(int j = i; j < size; j++){
		obj += x[j] * p[j].valor; 
		for(int k = j + 1; k < size; k++){
			obj += x[j] * x[k] * W[j][k];
		}	
	}
	model.update();
	model.setObjective(obj, GRB_MAXIMIZE);
	model.update();
	model.write("model.lp");
	model.optimize();
	double objvalP = model.get(GRB_DoubleAttr_ObjVal);
	//cout << objvalP << endl;
	/*
	max_lucro = objvalP;

	for (int i=0;i<size;i++) {
    	if (x[i].get(GRB_DoubleAttr_X) > 0.999){
    		bestInclude[i] = 1;
    	}
    }

    return false;
    */
	if(objvalP + lucro < max_lucro)
		return false;
	return true;
}

double novo_lucro(double lucro,int i,int include[]){
	double extra = 0;

	for(int j = 0; j < i; j++){
		if(include[j] == 1){
			extra += W[i][j];	
			extra += W[j][i];
		}
	}
	return lucro + extra + p[i].valor;
}

void backtracking(int i, double custo, double lucro, int include[]){
	//cout << "Custo: " << custo << " Lucro: " << lucro << " MAX: " << max_lucro << endl;
	if(custo <= max_custo && lucro > max_lucro){
		max_lucro = lucro;
			for(int j = 0; j < i; j++){
			bestInclude[j] = include[j];
		}
	}

	if(pode_expandir(i,custo,lucro)){
		include[i] = 1;
		backtracking(i + 1, custo + p[i].custo,novo_lucro(lucro,i,include),include);
		include[i] = 0;
		backtracking(i + 1, custo,lucro,include);
	}

	return;	
}


void seleciona_propagandas(int n, double C, double V[N], double P[N], double w[N][N], int *S, double *UpperBound) {
	int Scopy[N];
	double custo = 0;
	double l = 0;
	// inicializa variaveis globais
	max_custo = C;
	size = n;
	max_lucro = 0;
	for(int i = 0; i < n; i++) {
		bestInclude[i] = 0;
		S[i] = 0; 
		Scopy[i] = 0; 
		p[i].valor = V[i];
		p[i].custo = P[i];
		p[i].valorporcusto = V[i]/P[i];
		for(int j = 0; j < n; j++) {
			W[i][j] = w[i][j];
		}  
	}

	backtracking(0,0,0,Scopy);
	// salva o melhor resultado
	for(int i = 0; i < n; i++){
		cout << bestInclude[i] << ' ';
		if(bestInclude[i] == 1){
			custo += P[i];
			l += V[i];
		}
	}
	cout << " ---- ";
	*UpperBound = max_lucro;
	
	cout << custo << " <= " << C << " : ";
	return ;
}




int main(int argc,char *argv[]) {
	srand48(1);
	if (argc!=4) {cout<<endl<<"Usage: "<< argv[0]<<"  <filename>"<< endl << endl;
	cout << "Example:      " << argv[0] << " arq_e20_n10_m45_c100.in tempoMaximo raAluno" << endl << endl; exit(0);}

	ifstream ifile;
	ifile.open(argv[1]);  if (!ifile) return(false);  
	
	int Numero_Otimas = 0;
	int Numero_Nao_Otimas = 0;
	int Numero_Invalidas = 0;
	double Tempo_Otimas = 0;
	double Tempo_Nao_Otimas = 0;
	double Valor_Otimas = 0;
	double Valor_Nao_Otimas = 0;
	int tempoMaximo;
	sscanf(argv[2], "%d", &tempoMaximo);
	
	for (int ins = 0; ins < 20; ins++) {
		//4.1. Leia a E=(n,C,V,P,W) próxima entrada não processada de A.
		int n, m;
		double C;
		double otimo;
		ifile >> n; ifile >> m; ifile >> C; ifile >> otimo;
		double V[N];
		double P[N];
		int *S;
		S = (int*)malloc(N*sizeof(int));
		 
		double w[N][N];
		std::string name;
		
		for(int i=0; i<n; i++) {
			ifile >> name >> P[i] >> V[i];  
		}

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				w[i][j] = 0.0;
			}  
		}

		for(int i=0; i<n; i++) {
			for(int j = i+1; j<n; j++) {
				ifile >> name >> name >> w[i][j];
			}  
		}

		double VAL = 0;
	
		struct timeval inicio, final;
		// 4.2. Tempo_Inicio <--- Tempo do clock atual do sistema em milisegundos.
		gettimeofday(&inicio, NULL);
		
		// 4.3. Seleciona_Propagandas( E ,S, UB, Maxtime )
		seleciona_propagandas(n, C, V, P, w, S, &VAL);
		    
		//4.4. Tempo_Fim <--- Tempo do clock atual do sistema em milisegundos.
		gettimeofday(&final, NULL);
		
		//4.5. Tempo_resolucao <--- [Tempo_Fim - Tempo_Inicio] (tempo gasto em miliseg.)
		double tempo_resolucao = (double) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / (double)1000);
		
		//4.6 Verifica se a solução S é viável e se viável seja VAL o valor da solução
		double val_verif = 0.0;
		for(int i = 0; i < n; i++) {
			if(S[i] == 1) {
				val_verif += P[i];
			}
		}

		//4.7. Se Tempo_resolucão > 1.01 * Tmax OU S não é viável
		//o 1.01 é para dar um tempo de segurança de 1% de tempo que ele pode gastar a mais.
		if((val_verif > C) || (tempo_resolucao > 10.01*tempoMaximo)) {
			//4.8 então Numero_Invalidas++;
			//cout << val_verif << " > " << C << " OR " << tempo_resolucao << " > " << 1.01*tempoMaximo << endl;
			Numero_Invalidas++;
		} else {
			// 4.9 senão se Solução S é otima, então
			cout << VAL << " >= "<< otimo;
			if ( VAL >= otimo ) {
				cout << " OK";
				//4.10 então Numero_Otimas++
				Numero_Otimas++;
				//4.11 Tempo_Otimas += Tempo_resolucao
				Tempo_Otimas += tempo_resolucao;
				//4.12 Valor_Otimas += VAL
				Valor_Otimas += VAL;
			} else {
				//4.13 senão Numero_Nao_Otimas++
				Numero_Nao_Otimas++;
				//4.14 Tempo_Nao_Otimas += Tempo_resolucao
				Tempo_Nao_Otimas += tempo_resolucao;
				//4.15 Valor_Nao_Otimas += VAL
				Valor_Nao_Otimas += VAL;
			}
		}
		cout << endl;
	}
	
	/*	>	4.16 // Imprime
		>
		> Numero de sol. ótimas encontradas = <Numero_Otimas>
		> Valor médio das sol. ótimas encontradas = <Valor_Otimas / Numero_Otimas>
		> Numero de sol. não ótimas encontradas = <Numero_Nao_Otimas>
		> Valor médio das sol. ótimas encontradas = <Valor_Nao_Otimas /
		> Numero_Nao_Otimas>
		> Numero de sol. invalidas = <Numero_Invalidas>
	*/
	
	char fname[100];
	sprintf(fname,"%s_RES.out",argv[3]);
	
	FILE *fp;
	fp = fopen(fname,"w");   /* Arquivo ASCII, para escrita */
	if(!fp)
	{
		printf( "Erro na abertura do arquivo");
		exit(0);
	}
	
	fprintf(fp,"Numero de sol. ótimas encontradas = %d\n", Numero_Otimas);
	fprintf(fp,"Valor médio das sol. ótimas encontradas = %.4lf\n", Valor_Otimas / (double)Numero_Otimas);
	fprintf(fp,"Numero de sol. não ótimas encontradas = %d\n", Numero_Nao_Otimas);
	fprintf(fp,"Valor médio das sol. não ótimas encontradas = %.4lf\n", Valor_Nao_Otimas / (double)Numero_Nao_Otimas);
	fprintf(fp,"Numero de sol. invalidas = %d\n", Numero_Invalidas);
	
	cout << "Numero de sol. ótimas encontradas = " << Numero_Otimas <<"\n";
	cout << "Valor médio das sol. ótimas encontradas = " << Valor_Otimas / Numero_Otimas <<"\n";
	cout << "Numero de sol. não ótimas encontradas = " << Numero_Nao_Otimas <<"\n";
	cout << "Valor médio das sol. não ótimas encontradas = " << Valor_Nao_Otimas / Numero_Nao_Otimas <<"\n";
	cout << "Numero de sol. invalidas = " << Numero_Invalidas <<"\n";
	
	fclose(fp);
	ifile.close();

	return 0;
}

