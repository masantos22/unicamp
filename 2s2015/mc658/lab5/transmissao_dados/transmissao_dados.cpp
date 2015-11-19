	// Project and Analysis of Algorithms
// Flávio Keidi Miyazawa
// Problems with connectivity: Minimum Cost k-paths (edge disjoint)
#include <gurobi_c++.h>
#include <iostream>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <lemon/list_graph.h>
#define BC_EPS 0.001
#define BC_INF 1000000000000.0
//#define BC_INF 100000000.0
#include "mygraphlib.h"
#include <string>
#include "myutils.h"
#include <lemon/concepts/digraph.h>
#include <lemon/bfs.h>
#include <map>
#include <lemon/preflow.h>
#include "MTRand.h"
#include "BRKGA.h"
#include <list>
#include <vector>
#include <iomanip>  
using namespace lemon;
using namespace std;
#define EPS 0.00001

int cutcount = 0;

class SampleDecoder {
public:
	SampleDecoder();
	~SampleDecoder();

	double decode(const std::vector< double >& chromosome) const;

private:

};


class Problem_Data {
public:
	Problem_Data(ListGraph &g,
		NodeName &nodename,
		NodePos &posx,
		NodePos &posy,
		vector<Node> &s,
		vector<Node> &t,
		vector<double> &tmax,
		vector<double> &q,
		EdgeWeight &custo,
		EdgeWeight &capacidade,
		EdgeWeight &latencia);

	Problem_Data();

	ListGraph &g;
	int NNodes,NEdges,NPairs;
	NodeName &nodename;
	NodePos &posx;
	NodePos &posy;
	vector<Node> &s;
	vector<Node> &t;
	vector<double> &Tmax;
	vector<double> &q;
	EdgeWeight &custo; 
	EdgeWeight &capacidade; 
	EdgeWeight &latencia;
	vector<vector<Node> > BestSol;
	
	
	double BestVal;
};

Problem_Data::Problem_Data(ListGraph &graph,
		NodeName &vname,
		NodePos &posicaox,
		NodePos &posicaoy,
		vector<Node> &source,
		vector<Node> &tail,
		vector<double> &tempomax,
		vector<double> &qtd,
		EdgeWeight &cust,
		EdgeWeight &capac,
		EdgeWeight &lat):
	g(graph), 
	nodename(vname), 
	posx(posicaox), 
	posy(posicaoy), 
	s(source), t(tail), 
	Tmax(tempomax), 
	q(qtd),
	custo(cust),
	capacidade(capac),
	latencia(lat)
{
	BestVal = 0;
}

Problem_Data *H;
map<Node,int> map_node2int;
map<Edge,int> map_edge2int;
map<int,Node> map_int2node;
map<int,Edge> map_int2edge;


map<Node,int> node2int(){
	map<Node,int> m;
	int i = 0;
	for(ListGraph::NodeIt n(H->g); n != INVALID; ++n){
		m[n] = i;
		i++;
	}

	return m;
}

map<Edge,int> edge2int(){
	map<Edge,int> m;
	int i = 0;
	for(ListGraph::EdgeIt n(H->g); n != INVALID; ++n){
		m[n] = i;
		i++;
	}

	return m;
}

map<int,Node> int2node(){
	map<int,Node> m;
	int i = 0;
	for(ListGraph::NodeIt n(H->g); n != INVALID; ++n){
		m[i] = n;
		i++;
	}

	return m;
}

map<int,Edge> int2edge(){
	map<int,Edge> m;
	int i = 0;
	for(ListGraph::EdgeIt n(H->g); n != INVALID; ++n){
		m[i] = n;
		i++;
	}

	return m;
}

// retorna o custo se o caminho existir, senao retorna 0
double bfs(int source,int dest,vector<int> arestas_diponiveis,vector<double> &capacidade_arestas,double dados, double Tmax){
	vector<int> foi_empilhado;
	queue<int> pilha;
	vector<int> pai_vertice;
	bool caminho_completo = false;
	double custo = 0;

	// inicializa os pais e os vertices que foram visitados
	for(int i = 0; i < H->NNodes; i++){
		foi_empilhado.push_back(0);
		pai_vertice.push_back(-1);
	}
	
	// inicia a bfs
	pilha.push(source);
	foi_empilhado[source] = 1;
	
	//cout << endl <<"BFS" << endl;
	//cout << "("<< source << "," << dest << ")" << endl;
	//cout << "Empilhou: " << pilha.front() << endl;
	// enquanto a pilha nao estiver vazia
	while(!pilha.empty() && caminho_completo == false){
		int id_atual = pilha.front();
		//cout << "ATUAL: " << id_atual << endl; 
		// desempilha
		pilha.pop();
		Node atual = map_int2node[id_atual];

		for(ListGraph::IncEdgeIt e(H->g,atual); e!=INVALID; ++e){	
			int id_vizinho = map_node2int[H->g.u(e)];
			if(id_vizinho == id_atual)
				id_vizinho = map_node2int[H->g.v(e)];
			
			//cout << "ID VIZIN" << id_vizinho << endl;
			// id da aresta que conecta o vizinho e o atual
			int id_aresta = map_edge2int[e];

			// se a aresta esta disponivel e o vizinho nao foi visitado
			if(arestas_diponiveis[id_aresta] == 1 && foi_empilhado[id_vizinho] == 0){
				foi_empilhado[id_vizinho] = 1;
				pilha.push(id_vizinho);
				pai_vertice[id_vizinho] = id_atual;
				// se o destino foi alcancado, pare
				//cout << id_vizinho << " == " << dest << endl;
				if(id_vizinho == dest){
					//cout << "CAMINHO COMPLETO" << endl;
					caminho_completo = true;
					//cout << endl << "CAMINHO COMPLETO" << endl;
					break;
				}
			}

		}
	}
	int n = dest;
	double tempo = 0;

	while(n != -1 && caminho_completo){
		//if(n == dest){
		//	cout << endl<<"(" << source << "," << dest << ")" << endl;
		//}
		if(pai_vertice[n] != -1){
			//cout << n << " ";
			Node i = map_int2node[n];
			Node j = map_int2node[pai_vertice[n]];
			// existe aresta
			Edge e = findEdge(H->g,i,j);
			//cout << n << " --(" << map_edge2int[e] << ")-- ";
			
			custo += dados*H->custo[e];
			tempo += H->latencia[e];
			capacidade_arestas[map_edge2int[e]] += dados;
		}
		//else{
		//	cout << source;
		//}
		// atualiza vertice
		n = pai_vertice[n];
	}

	if(tempo > Tmax){
		tempo = 99999 * H->NPairs * 2;
	}
		
	else{
		tempo = 0;
	}
		
	if(caminho_completo){
		return custo + tempo;
	}
		
	return 0;
}

// second = node int
double SampleDecoder::decode(const std::vector< double >& chromosome) const {
	//Bfs<ListGraph> bfs(H->g);
	// vetor que verifica se ambos pares foram incluidos
	vector<int> pares;
	//
	vector<int> vertices;
	// vetor que armazena as arestas disponiveis
	vector<int> arestas;
	// vetor que verifica a capacidade utilizada das arestas
	vector<double> capacidade_arestas;
	// global maps
	map_node2int = node2int();
	map_edge2int = edge2int();
	map_int2node = int2node();
	map_int2edge = int2edge();

	vector< pair< double, unsigned > > ranking;
	
	int pares_conectados = 0;
	int consideradas = -1;
	int numero_pares = H->s.size();

	double fit = 0;

	// inicializa o vetor de pares
	for(int i =0; i < H->s.size();i++){
		pares.push_back(0);
	}

	for(int i = 0; i < H->NNodes; i++){
		vertices.push_back(0);
	}

	// inicializa as arestas utilizadas e o ranking
	for(int i = 0; i < H->NEdges; i++){
		capacidade_arestas.push_back(0);
		arestas.push_back(0);
		pair <double, int> P(chromosome[i], i);
		ranking.push_back(P);
	}

	std::sort(ranking.begin(), ranking.end());
	// enquanto nao conectar todos os pares
	// adiciona uma aresta
	// se conectou um dos pares NAO conectados
	// atualiza as capacidades das arestas utilziadas e custo total
	// por fim, penalidade pelas arestas nao utilizadas
	while(pares_conectados != numero_pares){
		consideradas++;
		Edge e = map_int2edge[ranking[consideradas].second];
		// atualiza vertices que contem arestas
		vertices[map_node2int[H->g.u(e)]] = 1;
		vertices[map_node2int[H->g.v(e)]] = 1;
		//cout << endl << consideradas;
		// adiciona a aresta na solucao
		arestas[ranking[consideradas].second] = 1;
		//for(int i = 0 ; i < pares.size(); i++){
		//	cout << pares[i] << " ";
		//}
		//cout << endl << endl;

		// verifica se a nova aresta considerada conecta um novo par
		for(int i = 0; i < numero_pares; i++){
			int s = map_node2int[H->s[i]]; 
			int t = map_node2int[H->t[i]];
			// se o par i nao estiver conectado e ambos possuim arestas
			if(pares[i] != 1 && vertices[s] > 0 && vertices[t] > 0){
				double custo_caminho = bfs(map_node2int[H->s[i]],map_node2int[H->t[i]],arestas,capacidade_arestas,H->q[i],H->Tmax[i]);
				// se o caminho existir, o custo sera maior que zero
				if(custo_caminho > 0){
					//cout << "CAMINHO ENCONTRADO: " << map_node2int[H->t[i]] << " " << map_node2int[H->s[i]] << " " << custo_caminho << endl;
					fit += custo_caminho;
					pares_conectados++;
					//cout << endl <<"PARES CONECTADOS: " << pares_conectados ;
					pares[i] = 1;
				}
			}
		}
	}
	
	// adiciona penalidades pra cada capacidades ultrapassadas
	for(int i = 0; i < H->NEdges; i++){
		if(capacidade_arestas[i] > H->capacidade[map_int2edge[i]]){
			//cout << endl << "FIT:" << fit << endl;
			//cout << "ARESTA MERDADA: " << i << endl;
			//cout << capacidade_arestas[i] << " > " << H->capacidade[map_int2edge[i]];
			fit = 99999 * H->NPairs * 2;
			break;
		}
		// aresta foi incluida mas nao foi utilizada
		//else if(arestas[i] > 0 && capacidade_arestas[i] == 0){
			//cout << fit << " + 5000 aresta nao utilizada " << endl;
		//	fit += 500;
		//}
	}
	//cout << fit << endl;

	return fit;
}


SampleDecoder::SampleDecoder() { };

SampleDecoder::~SampleDecoder() { };


bool transmissoes(Problem_Data &G, long maxtime) {

	/*
	 * Código do Exemplo passado pelo professor
	 */
	H = &G;

	const unsigned n = G.NEdges;		// size of chromosomes
	const unsigned p = 100;		// size of population
	const double pe = 0.30;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 3;		// number of independent populations
	const unsigned MAXT = 2;	// number of threads for parallel decoding
	
	SampleDecoder decoder;			// initialize the decoder
	
	const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator
	
	// initialize the BRKGA-based heuristic
	BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);
	

	double bestFitness = std::numeric_limits< double >::max();
	
	std::vector< double > bestChromosome;

	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 1000;	// run for 500 gens

	unsigned relevantGeneration = 1; // last relevant generation: best updated or reset called
	unsigned generation = 0;		// current generation
	do {
		algorithm.evolve();	// evolve the population for one generation
		
		// Bookeeping: has the best solution thus far improved?
		if(algorithm.getBestFitness() < bestFitness) {
			// Save the best solution to be used after the evolution chain:
			relevantGeneration = generation;
			bestFitness = algorithm.getBestFitness();
			bestChromosome = algorithm.getBestChromosome();
			std::cout << "\t" << generation
					<< ") Improved best solution thus far: "
					<< bestFitness << std::endl;
			
		}

		// Evolution strategy: exchange top individuals among the populations
		if((++generation) % X_INTVL == 0 && relevantGeneration != generation) {
			algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
		}

		// Next generation?
		++generation;

		//cout << generation << "<" << MAX_GENS << endl;
	} while (generation < MAX_GENS);
	
	//std::cout << "Best solution found has objective value = "
	cout << algorithm.getBestFitness() << std::endl;

	 G.BestVal = algorithm.getBestFitness();
	
}



int main(int argc, char *argv[]) 
{
	ListGraph g;
	int NNodes,NEdges,NPairs;
	NodeName nodename(g);
	ListGraph::NodeMap<double> posx(g), posy(g);
	vector<Node> s;
	vector<Node> t;
	vector<double> Tmax;
	vector<double> q;
	EdgeWeight custo(g);
	EdgeWeight capacidade(g);
	EdgeWeight latencia(g);
	vector<vector<Node>> BestSol;

	double BestVal;
	
  int seed=0;
  srand48(1);

  // uncomment one of these lines to change default pdf reader, or insert new one
  //set_pdfreader("open");    // pdf reader for Mac OS X
  //set_pdfreader("xpdf");    // pdf reader for Linux
  //set_pdfreader("poppler");  // pdf reader for Linux
  //set_pdfreader("open -a Skim.app");
  // double cutoff;   // used to prune non promissing branches (of the B&B tree)
  if (argc!=2) {cout<<endl<<"Usage: "<< argv[0]<<"  <filename>"<< endl << endl;
    cout << "Example:      " << argv[0] << " arq1.in" << endl << endl;
    exit(0);}

  string filename = argv[1];

  ReadListGraph3(filename, g, NNodes, NEdges, NPairs, nodename, posx, posy, latencia, capacidade, custo, s, t, Tmax, q);
  
  Problem_Data dt(g, nodename, posx, posy, s, t, Tmax, q, custo, capacidade, latencia);
  
  dt.NNodes = NNodes;
  dt.NEdges = NEdges;
  dt.NPairs = NPairs;

 try {

	bool res = transmissoes(dt, 10000);
	cout << dt.BestVal << endl;
	/*
    double soma=0.0;
    for (ListGraph::NodeIt v(g);v!=INVALID;++v) vcolor[v]=BLUE; // all nodes BLUE

    for (ListGraph::EdgeIt e(g); e!=INVALID; ++e) ecolor[e]=BLUE;

    for (ListGraph::NodeIt v(g); v!=INVALID; ++v) {
      lpvar[v] = x[v].get(GRB_DoubleAttr_X);
      if (lpvar[v] > 1.0 - EPS){ soma += weight[v]; vcolor[v]=RED;}
    }
    cout << "vCover Tree Value = " << soma << endl;

    //-----------------------------------------------------------------

    ViewListGraph(g,vname,ename,px,py,vcolor,ecolor,
	"vCover cost in graph with "+IntToString(T.nnodes)+
	" nodes: "+DoubleToString(soma));
	*/
  } catch (...) {cout << "Error during callback..." << endl; }
}

