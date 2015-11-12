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
#include "mygraphlib.h"
#include <string>
#include "myutils.h"
#include <lemon/concepts/digraph.h>
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

vector<vector<pair< double, int> > > split_chroms(vector<double> v, int NNodes, int NPairs){
 
    vector<vector<pair< double, int> > > matrix;
    vector <pair<double, int> > line;

    for(int i = 0; i < v.size(); i++) {
        pair <double, int> par(v[i], i);
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

bool comeca_com_pares(int par,int node){
	if( H->g.id(H->s[par]) == node || H->g.id(H->t[par]) == node )
		return true;

	return false;
}

bool existe_caminho(int par,vector< std::pair< double, unsigned > > ranking){
	Node atual = H->s[par];
	int last = H->g.id(H->t[par]);
	int next = 1;

	if( ranking[0].second == last){
		atual = H->t[par];
		last = H->g.id(H->s[par]);
	}

	bool go = true;
	while(go){
		//cout << "GO" << endl;
		go = false;
		// itera sobre as arestas do vertice atual
		for (ListGraph::IncEdgeIt e(H->g, atual); e != INVALID; ++e) {
			//cout << "ARESTAS" << endl;
			// se existir uma arestar entre atual e o ultimo par
			if(H->g.id(H->g.u(e)) == last || H->g.id(H->g.v(e)) == last){
				cout << "CAMINHO EXISTE" << endl;
				return true;
			}
			if(H->g.id(H->g.u(e)) == ranking[next].second || H->g.id(H->g.v(e)) == ranking[next].second){
				next += 1;
				go = true;
				break;
			} 
		}
	}

	return false;
}

bool respeita_tempo(int par,vector< std::pair< double, unsigned > > ranking){
	return false;
}

bool respeita_capacidade(int par,vector< std::pair< double, unsigned > > ranking){
	return false;
}

double custo_caminho(int par,vector< std::pair< double, unsigned > > ranking){
	return 10.0;
}

double SampleDecoder::decode(const std::vector< double >& chromosome) const {
	
	vector<vector<pair< double, int> > > matrix = split_chroms(chromosome,H->NNodes,H->s.size());
	int result = 0;

	for(int i = 0; i < H->s.size(); i++){
		vector< std::pair< double, unsigned > > ranking;
		for(int j = 0; j < H->NNodes; j++){
			ranking.push_back(std::pair< double, unsigned >(matrix[i][j].first,matrix[i][j].second));
		}
		std::sort(ranking.begin(), ranking.end());
		
		if(!comeca_com_pares(i,ranking[i].second)){
			result += BC_INF;
		}
		
		else if(!existe_caminho(i,ranking)){
			result += BC_INF/2.0;
		}

		else if(!respeita_tempo(i,ranking)){
			result += BC_INF/4.0;
		}

		else if(!respeita_capacidade(i,ranking)){
			result += BC_INF/8.0;
		}

		else{
			custo_caminho(i,ranking);
		}
	}

	//for(unsigned i = 0; i < chromosome.size(); ++i) {
	//	ranking[i] = std::pair< double, unsigned >(chromosome[i], i);
	//}

	// Here we sort 'permutation', which will then produce a permutation of [n] in pair::second:
	//std::sort(ranking.begin(), ranking.end());

	//permutation[i].second is in {0, ..., n - 1}; a permutation can be obtained as follows
	//std::list< unsigned > permutation;
	//for(std::vector< std::pair< double, unsigned > >::const_iterator i = ranking.begin();
	//		i != ranking.end(); ++i) {
	//	permutation.push_back(i->second);
	//}


	//return chromosome.front();

	return result;
}
SampleDecoder::SampleDecoder() { };

SampleDecoder::~SampleDecoder() { };


bool transmissoes(Problem_Data &G, long maxtime) {
	/*
	 * Código do Exemplo passado pelo professor
	 */
	H = &G;

	cout << "\n\n CODIGO \n\n\n\n";
	cout << G.s.size() << endl;
	cout << G.NNodes << endl;
	const unsigned n = G.NNodes * G.s.size();		// size of chromosomes
	const unsigned p = 256;		// size of population
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
	const unsigned MAX_GENS = 100000;	// run for 1000 gens

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
			
		}

		//  Evolution strategy: restart
		if(generation - relevantGeneration > RESET_AFTER) {
			algorithm.reset();	// restart the algorithm with random keys
			relevantGeneration = generation;
			
		}

		// Evolution strategy: exchange top individuals among the populations
		if((++generation) % X_INTVL == 0 && relevantGeneration != generation) {
			algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
		}

		// Next generation?
		++generation;


	} while (generation < MAX_GENS);
	
	std::cout << "Best solution found has objective value = "
	 		<< algorithm.getBestFitness() << std::endl;
	
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

  cout << NNodes << " " << NEdges << " " << NPairs << endl;
  
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

