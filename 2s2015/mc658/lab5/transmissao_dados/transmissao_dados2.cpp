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
map<int,Node> map_int2node;
double max_custo; 

vector<vector<pair< double, int> > > split_chroms(vector<double> v, int NNodes, int NPairs){
 
    vector<vector<pair< double, int> > > matrix;
    vector <pair<double, int> > line;

    for(int i = 0; i < v.size(); i++) {
        pair <double, int> par(v[i], i%NNodes);
		line.push_back( par );
		
		if (line.size() == NNodes){
		        matrix.push_back(line);
		        line.clear();
		}
    }

    return matrix;
}

void print_matrix(vector<vector<pair< double, int> > > matrix){
	for (int i = 0; i < matrix.size(); i++){
		for (int j = 0; j < matrix[i].size(); j++ ) {
			cout << "(" << matrix[i][j].first << ", " << matrix[i][j].second << ") ";
		}
		cout << "\n";
	}
	return ;

}

bool comeca_com_pares(int par,Node n){
	if(n == H->s[par] || n == H->t[par]){
		//cout << H->g.id(n) << " == " << H->g.id(H->s[par]) << " || " << H->g.id(H->t[par]) << endl; 
		return true;
	}
	return false;
}


double custo_caminho(int par,vector< std::pair< double, unsigned > > ranking,vector<Edge> &E,int &caminho_completo){
	double custo = 0;
	double result = BC_INF/2;

		for(int i = 1; i < ranking.size(); i++){
			Node first = map_int2node[ranking[i-1].second];
			Node next = map_int2node[ranking[i].second];
			Edge e = findEdge(H->g,first,next);
		
		if(e == INVALID){
			//return max(result,99999.0*H->s.size());
			return result + 10000*custo;
		}
		else{
			E.push_back(e);
			//cout << "DIV By 2" << endl;
			// para cada aresta que conecta o grafo, melhoramos o fit em 10%
			result *= 0.5;
			custo += H->custo[e];
			// se o proximo vertice for um dos pares entao o caminho esta completo
			if(next == H->s[par] || next == H->t[par]){
				//cout << "Custo " << custo * H->q[par] << endl;
				//cout << H->g.id(map_int2node[ranking[0].second]) << " -- " << H->g.id(next) << endl;
				caminho_completo = 1;
				return custo * H->q[par];
			}
		}
	}
}



map<int,Node> int2node(Problem_Data &G){
	map<int,Node> m;
	int i = 0;
	for(ListGraph::NodeIt n(G.g); n != INVALID; ++n){
		m[i] = n;
		i++;
	}

	return m;
}

map<Edge,double> uso_capacidade(){
	map<Edge,double> m;
	for (ListGraph::EdgeIt e(H->g); e != INVALID; ++e) {
		m[e] = 0;
	}
	return m;
}

double get_max_custo(){
	double custo = 0;
	for (ListGraph::EdgeIt e(H->g); e != INVALID; ++e) {
		custo += H->custo[e];
	}
	return custo*H->s.size();

}
// second = node int
double SampleDecoder::decode(const std::vector< double >& chromosome) const {

	vector<vector<pair< double, int> > > matrix = split_chroms(chromosome,H->NNodes,H->s.size());
	//print_matrix(matrix);
	double result = 0;
	map<Edge,double> total_aresta = uso_capacidade();
	bool estoura_tempo = false;
	bool estoura_capacidade = false;
	int n = 0;


	for(int i = 0; i < H->s.size(); i++){
		vector< std::pair< double, unsigned > > ranking;
		for(int j = 0; j < H->NNodes; j++){
			ranking.push_back(std::pair< double, unsigned >(matrix[i][j].first,matrix[i][j].second));
		}
		std::sort(ranking.begin(), ranking.end());

		if(!comeca_com_pares(i,map_int2node[ranking[0].second])){
			result += BC_INF;
		}
		else{
			estoura_tempo = false;
			estoura_capacidade = false;

			int caminho_completo = 0;
			vector<Edge> e;
			double custo;

			// retorna o custo do caminho, se o caminho esta completo e as arestas utilizadas
			custo = custo_caminho(i,ranking,e,caminho_completo);

			if(caminho_completo == 1){
				n++;
				// calcula o tempo de transmissao dos dados
				double tempo = 0;
				//cout << "Custo:" << custo << endl;
				// itera sobre as arestas em uso e verifica se:
				// 1 - o tempo nao supera o limite permitido
				// 2 - a capacidade da aresta nao excede o limite
				// 3 - armazena o quanto de capacidade de uma aresta foi utilizado
				for(int k = 0; k < e.size(); k++){
					total_aresta[e[k]] += H->q[i];
					tempo += H->latencia[e[k]];
					// se estourar a capacidade de uma aresta
					if(H->q[i] > H->capacidade[e[k]]){
						estoura_capacidade = true;
						cout << "Estoura capacidade" << endl;
						//cout << custo << endl;
						custo *= 100; 
						//cout << custo << endl;
					}
				}
				// se estourar o tempo
				if(tempo > H->Tmax[i]){
					estoura_tempo = true;
					cout << "Estoura tempo" << endl;
					//cout << custo << endl;
					custo *= 100;
					//cout << custo << endl;
				}
				//cout << "Custo:" << custo << endl;
			
			}
			
			result += custo;
			
		}
		
		
	}

	for (ListGraph::EdgeIt e(H->g); e != INVALID; ++e) {
		//if(total_aresta[e] > 0)
			//cout << total_aresta[e] << " > " << H->capacidade[e] << endl;
		if(total_aresta[e] > H->capacidade[e]){
			cout << "Capacidade geral" << endl;
			//cout << result << endl;
			//cout << 10*result << endl << endl;
			//cout << H->capacidade[e] << " < " << total_aresta[e] << endl;
			//cout << result << " -> Estoura capacidade" << endl;
			//cout << 1000*result << endl;
			//result = result*5;
			result *= 100;
			break;
		}
			
	}
	
	//cout << endl;
	return result;

}


SampleDecoder::SampleDecoder() { };

SampleDecoder::~SampleDecoder() { };


bool transmissoes(Problem_Data &G, long maxtime) {

	/*
	 * Código do Exemplo passado pelo professor
	 */
	H = &G;
	max_custo = get_max_custo();
	cout << max_custo << endl;
	//for(int i = 0; i < H->s.size(); i++){
	//	cout << H->g.id(H->s[i]) << " " << H->g.id(H->t[i]) << endl;
	//}
	//cout << endl;

	map_int2node = int2node(G);

	const unsigned n = G.NNodes * G.s.size();		// size of chromosomes
	const unsigned p = 100;		// size of population
	const double pe = 0.20;		// fraction of population to be the elite-set
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
	const unsigned RESET_AFTER = 200;
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 5000;	// run for 1000 gens

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
	
	std::cout << "Best solution found has objective value = "
	 		<< algorithm.getBestFitness() << std::endl;

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

