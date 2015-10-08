/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Kruskal Algorithm
    Marcelo Azevedo Goncalves dos Santos
    RA:106140
*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 10001

typedef struct SVertex{
    int vert;
    int weight;
    struct SVertex *next;
}SVertex;


int global_line[NMAX];
int parent[NMAX];
int edge_cost[NMAX];
SVertex list[NMAX][NMAX];

// functions 
void create_list_edge(int from,int to,int weight);
void print_list(int vertices);
void free_list(int vertices);
void bfs(int end_point, int vertices);
int put_in_line(int node_number);
void remove_edge(int from,int to);
int update_mst(int from,int to,int weight,int cost);

// main program
int main(){
    
    int sites,new_edges;
    int edges,vertices;
    int from,to,weight; 
    int i,j;
    int cost = 0;
    
    scanf("%d",&sites);
    vertices = sites;
    edges = sites - 1;   
    
    // initialize list
    for(i = 0; i < vertices; i++){
        for(j = 0; j < vertices; j++){
            list[i][j].next = NULL;
        }
    }

    // read edges and update cost
    for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&weight);
        create_list_edge(from,to,weight);
        create_list_edge(to,from,weight);
        cost += weight;
    }
    //printf("OUT\n");
    // print_list(vertices);
    // read the amount of new edges
    scanf("%d",&new_edges);

    if(sites == 1) printf("0 0\n");
    else{
        printf("%d ",cost);
        // read the new edges   
       for(i = 0; i < new_edges; i++){
            scanf("%d %d %d",&from,&to,&weight);
            parent[from] = -1;
            global_line[0] = from;
            global_line[1] = -1;
            bfs(to,vertices);
            cost = update_mst(to,from,weight,cost);
       }
       printf("%d\n",cost);
    }
    
    return 0;
}

int update_mst(int from,int to,int weight,int cost){
    int max_weight = edge_cost[from];
    int start = from;
    int end = parent[from];
    int i;

    for(i = parent[from]; i != to; i = parent[i]){
        if(edge_cost[i] > max_weight ){
            max_weight = edge_cost[i];
            start = i ;
            end = parent[i];
        }
    }

    if(weight < max_weight){
        create_list_edge(from,to,weight);
        create_list_edge(to,from,weight);
        //printf("\nNEW EDGE %d %d\n",from,to);
        //print_list(7);
        remove_edge(start,end);
        remove_edge(end,start);
        //printf("\nREMOVE EDGE %d %d\n",start,end);
        //print_list(7);
        return (cost - max_weight + weight);
    }
    return cost;
}

void remove_edge(int from,int to){
    SVertex * run, *prev;
    prev = &list[from][from];
    for(run = list[from][from].next;run->vert != to;run = run->next){
        prev = run;
    }
    prev->next = run->next;
    return ;
}

void bfs(int end_point, int vertices){
    int i;
    int node_number;
    SVertex *run;

    for(i = 0; i < vertices; i++){
        node_number = global_line[i];
        if(node_number != end_point){
            for(run = list[node_number][node_number].next;run != NULL; run = run->next){
                if(put_in_line(run->vert)){
                    parent[run->vert] = global_line[i];
                    edge_cost[run->vert] = list[node_number][run->vert].weight;
                }
            }
        }
        else
            break;
    }
    return ;
}

int put_in_line(int node_number){
    int i;
    for(i = 0;global_line[i] != -1;i++){
        if(node_number == global_line[i])
            return 0;
    }
    global_line[i] = node_number;
    global_line[i + 1] = -1;
    return 1;
}

void create_list_edge(int from,int to,int weight){
    SVertex *run;
    list[from][to].weight = weight;
    list[from][to].vert = to;

    for(run = &list[from][from];run->next != NULL;run = run->next);
    run->next = &list[from][to];

    return ;
}
// print the adjacency list 
void print_list(int vertices){
    int i;
    SVertex *run;

    for(i = 0;i < vertices; i++){
        printf("%d:",i);
        for(run = list[i][i].next; run != NULL;run = run->next){
            printf("--(%d)-->%d",run->weight,run->vert);
        }
        printf("\n");
    }
    return ;
}



    


