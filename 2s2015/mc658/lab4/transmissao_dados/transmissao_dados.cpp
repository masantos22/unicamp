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
#include <map>
#include <lemon/preflow.h>
using namespace lemon;
using namespace std;
#define EPS 0.00001

int cutcount = 0;

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
	double BestLB;
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
	BestLB = 0;
}

int numero_vertices(Problem_Data &G){
	int i = 0;
	for(ListGraph::NodeIt n(G.g); n != INVALID; ++n){
		i +=1;
	}
	return i;
}

int numero_arestas(Problem_Data &G){
	int i = 0;

	for(ListGraph::EdgeIt n(G.g); n != INVALID; ++n){
		i +=1;
	}
	return i;
}

map<Edge,int> aresta2id(Problem_Data &G){
	map<Edge,int> m;
	int i = 0;
	for(ListGraph::EdgeIt n(G.g); n != INVALID; ++n){
		if(m.find(n) == m.end()){
			m[n] = i;
			i++;
		}
	}

	return m;
}

bool transmissoes(Problem_Data &G, long maxtime) {

	map<Edge,int> m = aresta2id(G);
	int n_vertices = numero_vertices(G);
	int n_arestas = numero_arestas(G);
	int pares = G.s.size();

	// variavel para salvar as arestas da solucao
	vector<vector<GRBVar>> arestas(pares,vector<GRBVar>(n_arestas));
	// variavel para salvar os vertices da solucao
	vector<vector<GRBVar>> vertices(pares,vector<GRBVar>(n_vertices));

	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env);
	// disable standard output from Gurobi
	//model.getEnv().set(GRB_IntParam_OutputFlag, 0);
	GRBLinExpr obj;
	// gives a name to the problem
	model.set(GRB_StringAttr_ModelName, "Skype"); 
	
	// adiciona as arestas  
	for (int i = 0; i < pares ;i++) {
		GRBLinExpr expr;
		for(ListGraph::EdgeIt e(G.g); e != INVALID; ++e ) {
			arestas[i][G.g.id(e)] = model.addVar(0.0,1.0,1,GRB_BINARY,"");
			//expr += G.latencia[e] * G.q[i] * arestas[i][G.g.id(e)];
			expr += G.latencia[e] * arestas[i][G.g.id(e)];
			obj += G.q[i] * G.custo[e] * arestas[i][G.g.id(e)];
		}
		model.update();
		model.addConstr(expr <= G.Tmax[i]);
		model.update();
	}
	model.update();

	// restringe a capacidade das areastas  
	for (ListGraph::EdgeIt e(G.g); e != INVALID; ++e) {
		GRBLinExpr expr;
		for(int i = 0; i < pares; i++) {
			expr += arestas[i][m[e]];
			//expr += arestas[i][G.g.id(e)];
		}
		model.update();
		model.addConstr(expr <= G.capacidade[e]);
		model.update();
		
	}
	model.update();
	
	// adiciona os vertices
	for (int i = 0; i < pares ;i++) {
		for(ListGraph::NodeIt n(G.g); n != INVALID; ++n){
			GRBLinExpr expr;
			// se o vertice for um dos pares, so uma de suas arestas esta na solucao
			int k = G.g.id(n);
			if(n == G.s[i] || n == G.t[i]){
				vertices[i][k] = model.addVar(0.0,1.0,1.0,GRB_BINARY,"");
				GRBLinExpr v = vertices[i][k];
				for(ListGraph::IncEdgeIt e(G.g,n); e != INVALID; ++e ){
					expr += arestas[i][m[e]];
				}
				model.update();
				model.addConstr(expr == 1);
				model.update();
				model.addConstr(v == 1);
				model.update();
			}
			// senao, duas arestas estao na solucao
			else{
				vertices[i][k] = model.addVar(0.0,1.0,1.0,GRB_BINARY,"");
				GRBVar v = vertices[i][k];
				
				for(ListGraph::IncEdgeIt e(G.g,n); e != INVALID; ++e ){
					expr += arestas[i][m[e]];
				}
				model.update();
				model.addConstr(expr == 2*v);
				model.update();
			}
		}
		model.update();
	}
	model.update();
	model.setObjective(obj, GRB_MINIMIZE);
	model.update();
	model.write("model.lp");
	
	model.optimize();
	G.BestVal = model.get(GRB_DoubleAttr_ObjVal);
	cout << G.BestVal << endl;
	
	G.BestSol.resize(pares);

	// guarda os vertices que fazem parte dos pares das conexoes
	for(ListGraph::NodeIt n(G.g); n != INVALID; ++n){
		for(int i = 0; i < pares; i++){
			if(vertices[i][G.g.id(n)].get(GRB_DoubleAttr_X) > 0.999){
				G.BestSol[i].push_back(n);
			}
		}
	}
	//cout << G.g.id(G.BestSol.at(0)[0]) << endl;
	/*
	cout << "PARES" << endl;
	for(int i = 0;  i < pares; i++){
		cout << G.g.id(G.s[i]) << " " << G.g.id(G.t[i]) << endl;
	}
	*/
	/*
	for(vector<vector<Node>>::iterator it = G.BestSol.begin(); it != G.BestSol.end();++it){
		for(vector<Node>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2){
			cout << G.g.id(*it2) << " ";
		}
		cout << endl;
	}
	cout << endl;
	*/
	/*
	for (int i = 0; i < pares;i++) {
		for (int j = 0; j < n_vertices;j++){
			if(vertices[i][j].get(GRB_DoubleAttr_X) > 0.999)
				cout << 1;
			else
				cout << 0;
			cout << " ";
		}
		cout << endl;
    } 
    */			
	return 0;
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
	double BestLB;
	
  int seed=0;
  srand48(1);

  // uncomment one of these lines to change default pdf reader, or insert new one
  //set_pdfreader("open");    // pdf reader for Mac OS X
  set_pdfreader("xpdf");    // pdf reader for Linux
  //set_pdfreader("poppler");  // pdf reader for Linux
  //set_pdfreader("open -a Skim.app");
  // double cutoff;   // used to prune non promissing branches (of the B&B tree)
  if (argc!=2) {cout<<endl<<"Usage: "<< argv[0]<<"  <filename>"<< endl << endl;
    cout << "Example:      " << argv[0] << " arq1.in" << endl << endl;
    exit(0);}

  string filename = argv[1];

  ReadListGraph3(filename, g, NNodes, NEdges, NPairs, nodename, posx, posy, latencia, capacidade, custo, s, t, Tmax, q);
  Problem_Data dt(g, nodename, posx, posy, s, t, Tmax, q, custo, capacidade, latencia);
  
 try {

	bool res = transmissoes(dt, 10000);
	cout << "Melhor resultado: " << dt.BestVal << endl;
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
  }	
  catch (GRBException gex) {
  	cout << "Error during callback..." << endl;
  	cout << gex.getMessage() << " " << gex.getErrorCode() << endl;

  }
}

