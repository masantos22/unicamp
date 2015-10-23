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
using namespace lemon;
using namespace std;
#define EPS 0.00001
#define MAX_VERTICES 100000

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

array<list<int>, MAX_VERTICES> get_vertices_arestas(Problem_Data &G){
       
	array<list<int>, MAX_VERTICES> vertexes;

	for(ListGraph::EdgeIt e(G.g); e != INVALID; ++e ) {
		vertexes.at(G.g.id(G.g.u(e))).push_back(G.g.id(e));
		vertexes.at(G.g.id(G.g.v(e))).push_back(G.g.id(e));    
	}

	return vertexes;
}


bool transmissoes(Problem_Data &G, long maxtime) {
	array<list<int>, MAX_VERTICES> table = get_vertices_arestas(G);
	int n_vertices = numero_vertices(G);
	int n_arestas = numero_arestas(G);
	int pares = G.s.size();

	/*
	for(ListGraph::NodeIt n(G.g); n != INVALID; ++n){
		cout << G.g.id(n) << "{ ";
		for (list<int>::iterator it = table.at(G.g.id(n)).begin(); it != table.at(G.g.id(n)).end(); it++){
			cout << *it << " ";
		}
		cout << "}\n";
	}
	*/
	/*
	cout << "PARES" << endl;
	for(int i = 0;  i < pares; i++){
		cout << G.g.id(G.s[i]) << " " << G.g.id(G.t[i]) << endl;
	}

	cout << "TEMPO" << endl;
	for(int i = 0;  i < pares; i++){
		cout << G.Tmax[i] << " " << endl;
	}

	cout << "Q" << endl;
	for(int i = 0;  i < pares; i++){
		cout << G.q[i] << " " << endl;
	}
	*/
	vector<vector<GRBVar>> arestas(pares,vector<GRBVar>(n_arestas));
	vector<vector<GRBVar>> vertices(pares,vector<GRBVar>(n_vertices));

	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env);
	// disable standard output from Gurobi
	//model.getEnv().set(GRB_IntParam_OutputFlag, 0);
	GRBLinExpr obj;
	// gives a name to the problem
	model.set(GRB_StringAttr_ModelName, "Skype"); 
	// says that lp is a maximization problem
	//model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
	
	// adiciona as arestas  
	for (int i = 0; i < pares ;i++) {
		GRBLinExpr expr;
		for(ListGraph::EdgeIt e(G.g); e != INVALID; ++e ) {
			arestas[i][G.g.id(e)] = model.addVar(0.0,1.0,1,GRB_BINARY,"");
			expr += G.latencia[e] * G.q[i] * arestas[i][G.g.id(e)];
			//expr += G.latencia[e] * arestas[i][G.g.id(e)];
			obj += G.q[i] * G.custo[e] * arestas[i][G.g.id(e)];
		}
		model.update();
		//cout << i << endl;
		cout << G.Tmax[i] << endl;
		model.addConstr(expr <= G.Tmax[i]);
		model.update();
		//model.addConstr(expr <= 300);
		//model.update();
	}
	model.update();
	// restringe a capacidade das areastas  
	for (ListGraph::EdgeIt e(G.g); e != INVALID; ++e) {
		GRBLinExpr expr;
		for(int i = 0; i < pares; i++) {
			expr += arestas[i][G.g.id(e)];
		}
		model.update();
		cout << G.capacidade[e] << endl;
		model.addConstr(expr <= G.capacidade[e]);
		model.update();
		
	}
	model.update();
	
	// adiciona os vertices
	for (int i = 0; i < pares ;i++) {
		for(ListGraph::NodeIt n(G.g); n != INVALID; ++n){
			GRBLinExpr expr;
			int k = G.g.id(n);
			if(k == G.g.id(G.s[i]) || k == G.g.id(G.t[i])){
				vertices[i][k] = model.addVar(0.0,1.0,1.0,GRB_BINARY,"");
				for (list<int>::iterator it = table.at(k).begin(); it != table.at(k).end(); it++){
					expr += arestas[i][*it];
				}
				model.update();
				model.addConstr(expr == 1);
				model.update();
			}
			else{
				vertices[i][k] = model.addVar(0.0,1.0,1.0,GRB_BINARY,"");
				GRBVar v = vertices[i][k];
				for (list<int>::iterator it = table.at(k).begin(); it != table.at(k).end(); it++){
					expr += arestas[i][*it];
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
	//model.addConstr(expr <= max_custo - custo);
	// Process any pending model modifications.
	//model.update(); 
	/*
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
	*/
	
	model.optimize();
	//double objvalP = model.get(GRB_DoubleAttr_ObjVal);
	//cout << objvalP << endl;
	/*
	for (int i = 0; i < pares;i++) {
		for (int j = 0; j < n_arestas;j++){
			if(arestas[i][j].get(GRB_DoubleAttr_X) > 0.999)
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
  //set_pdfreader("xpdf");    // pdf reader for Linux
  //set_pdfreader("poppler");  // pdf reader for Linux
  //set_pdfreader("open -a Skim.app");
  // double cutoff;   // used to prune non promissing branches (of the B&B tree)
  if (argc!=2) {cout<<endl<<"Usage: "<< argv[0]<<"  <filename>"<< endl << endl;
    cout << "Example:      " << argv[0] << " arq1.in" << endl << endl;
    exit(0);}

  string filename = argv[1];

  ReadListGraph3(filename, g, NNodes, NEdges, NPairs, nodename, posx, posy, latencia, capacidade, custo, s, t, Tmax, q);
  cout << NPairs << endl;
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

