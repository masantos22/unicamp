/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Laboratório 1 - Parte B - Dominadores em Grafos Orientados

    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 200

void create_graph(int graph[][NMAX]);

void copy_graph(int from[][NMAX],int to[][NMAX]);

void create_graph_edge(int graph[][NMAX],int from,int to);

void print_graph(int graph[][NMAX]);

void bfs_graph(int graph[][NMAX],int node_number,int line_pos,int line[],int path[]);

int put_in_line(int line[],int j);

void create_line(int line[]);

void remove_edges(int graph[][NMAX],int node);

int g_line[NMAX],h_line[NMAX];
int g_path[NMAX],h_path[NMAX];

int vertices;

int main(){
	
    int edges;
    int graph[NMAX][NMAX],graph_H[NMAX][NMAX],dom_matriz[NMAX][NMAX];
    int i,j;
    int from,to;    

    scanf("%d %d",&vertices,&edges);

    create_graph(graph);
    create_graph(graph_H);
    create_graph(dom_matriz);

    create_line(g_line);

    create_line(g_path);

    // initialize dominator matriz assuming every node dominates every other node
    for(i = 0; i < vertices; i++){
        for(j = 0; j < vertices; j++){
            dom_matriz[i][j] = 1;
        }
    }

    // create graphs G and H
    for(i = 0; i < edges; i++){
        scanf("%d %d",&from,&to);
        create_graph_edge(graph,from,to);
    }
		
    // initialize line
	g_line[0] = 0;
    // initialize G path
    g_path[0] = 1;
    // graph,node,lines position
	bfs_graph(graph,0,0,g_line,g_path);	

    for( i = 0; i < vertices; i++ ){
        // clean H line
        create_line(h_line);
        // initialize H path from
        create_line(h_path);
        // initialize H line
        h_line[0] = 0;
        // initialize H path
        h_path[0] = 1;
        // H <= G 
        copy_graph(graph,graph_H);
        // remove all edges from H of node i
        remove_edges(graph_H,i);
        bfs_graph(graph_H,0,0,h_line,h_path);

        for(j = 0; j < vertices; j++){
            if(g_path[j] == -1 || (i != j && h_path[j] == 1) ){
                dom_matriz[i][j] = 0;
            }
        }
        
    }

    print_graph(dom_matriz);


	return 0;
}

void bfs_graph(int graph[][NMAX],int node_number,int line_pos,int line[],int path[]){
    int j;      

    while(line[line_pos] != -1){
        for(j = 0;j < vertices; j++){
            if(graph[node_number][j] == 1 ){
                if(put_in_line(line,j)){
                    //printf("CHEGA:%d\n",j);
                    path[j] = 1;
                }
            }   
        }
        line_pos++;
        node_number = line[line_pos];
    }   
    
    return;
}

void remove_edges(int graph[][NMAX],int node){

    int i;

    for( i = 0; i < vertices; i++)
        graph[node][i] = 0;

    return ;
}

// initializate graph with empty egdes
void create_graph(int graph[][NMAX]){
    
    int i,j;
        
    for(i = 0; i < vertices; i++)
        for(j = 0; j < vertices;j++)
            graph[i][j] = 0;        

    return;
}

void create_graph_edge(int graph[][NMAX],int from,int to){
    graph[from][to] = 1;
    return ;
}

int put_in_line(int line[],int j){
	int i;
	
	for(i = 0; line[i] != -1; i++){
		if(line[i] == j) return 0;
	}
	line[i] = j;
	
    return 1;
}


void print_graph(int graph[][NMAX]){
    int i,j;
    
    for(i = 0;i < vertices; i++){
        for(j = 0; j < vertices; j++){
            printf("%d",graph[i][j]);
            if(j + 1 < vertices) printf(" ");
        }
        printf("\n");
    }
    
    return ;
}

void copy_graph(int from[][NMAX],int to[][NMAX]){
    int i,j;

    for(i = 0; i < vertices; i++){
        for(j = 0; j < vertices; j++){
            to[i][j] = from[i][j];
        }
    }

    return ;

}

void create_line(int line[]){
    
    int i;

    for(i = 0; i < vertices + 1; i++){
        line[i] = -1;
    }

    return ;
}










