#ifndef __DIJCPP_H
#define __DIJCPP_H

#include <vector>
#include <queue>
#include <stdlib.h>

#define INFINITO 999999999

using namespace std;

class Grafo {
	
	friend ostream& operator<<(ostream& output, const Grafo& p);

		public:

	struct Edge {
		int x;
		int y;
		int w;
	};

	// cria um grafo vazio
	Grafo() {
	}

	// cria um novo gravo com n vertices
	Grafo(int n) {
		for (int i = 0; i < n; i++)
			adj.push_back(vector<Edge>());
	}

	// adiciona um vertice ao grafo
	// devolve o indice do vertice adicionado
	int adicionaVertice() {
		adj.push_back(vector<Edge>());
		return adj.size()-1;
	}

	int size(void) {
		return adj.size();
	}

	// adiciona uma aresta de x a y com peso w
	// 0 <= x,y <= n-1
	void adicionaAresta(int x, int y, int w) {
		Edge e;
		e.x = x;
		e.y = y;
		e.w = w;

		adj[x].push_back(e);
	}

	// calcula caminho minimo de s a t
	// devolve o valor do caminho minimo
	// coloca o caminho em P
	int dijkstra(int s, int t, vector<Edge>& P) {
		int *d;
		Edge **pai;
		
		d = (int *) malloc(adj.size()*sizeof(int));
		pai = (Edge **) malloc(adj.size()*sizeof(Edge*));

		for (unsigned int i = 0; i < adj.size(); i++) {
			d[i] = INFINITO;
			pai[i] = NULL;
		}
		d[s] = 0;

		priority_queue<pair<int, int> > q;
		q.push(make_pair(0, s));

		while (!q.empty()) {
			int u = q.top().second;
			int w = -q.top().first;
			q.pop();
			if (w > d[u]) continue;

			for (vector<Edge>::iterator it = adj[u].begin(); it != adj[u].end(); it++) {
				int v = it->y;
				if (d[v] > d[u] + it->w) {
					pai[v] = &*it;
					d[v] = d[u] + it->w;
					q.push(make_pair(-d[v], v));
				}
			}
		}

		vector<Edge> Pinv;
		for (Edge *e = pai[t]; e != NULL; e = pai[e->x])
			Pinv.push_back(*e);
		for (vector<Edge>::reverse_iterator it = Pinv.rbegin(); it != Pinv.rend(); it++)
			P.push_back(*it);
		return d[t];
	}

		private:

	vector<vector<Edge> > adj;
};

ostream& operator<<(ostream& output, const Grafo& g) {
	output << "# vertices: " << g.adj.size() << endl;
	for (unsigned int i = 0; i < g.adj.size(); i++) {
		output << i << ": ";
		vector<Grafo::Edge> l = g.adj[i];
		for (vector<Grafo::Edge>::iterator it = l.begin(); it != l.end(); it++)
			output << "(" << it->x << ", " << it->y << ", " << it->w << ") ";
		output << endl;
	}
	return output;
}


#endif /* __DIJCPP_H */
