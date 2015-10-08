/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Kruskal Algorithm
    Marcelo Azevedo Goncalves dos Santos
    RA:106140
*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 100000

typedef struct SEdge{
    int from;
    int to;
    int weight;
    int in_tree;
}Edge;

Edge ed[9999999999];

// functions 
void kruskal(int node_tree[],int vertices,int edges);
void insert_edge_array(int from,int to,int weight,int pos);
void init_mst(int vertices,int node_tree[],int edges);
void sort_e(int edges,int pos);
int get_cost(int edges);
void print_edges(int edges);

int sort_edge(const void *a, const void *b) 
{ 
    Edge *p1 = (Edge *)a;
    Edge *p2 = (Edge *)b;
    return p1->weight - p2->weight;
} 

// main program
int main(){
    
    int vertices,edges,sites,new_edges;
    int from,to,weight; 
    int i,j;
    int node_tree[NMAX];
    int total_cost = 0;

    scanf("%d",&sites);
    vertices = sites;
    edges = sites - 1; 

    // read mst
    for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&weight);
        insert_edge_array(from,to,weight,i);
        ed[i].in_tree = 1;
        total_cost += weight;
    }
    scanf("%d",&new_edges);

    if(vertices == 1) printf("0 0\n");
    else{
        printf("%d ",total_cost);
        qsort(ed,edges,sizeof(Edge), sort_edge);
        //print_edges(edges);
        for(j = 0;j < new_edges;j++){
            scanf("%d %d %d",&from,&to,&weight);
            insert_edge_array(from,to,weight,i);
            //printf("NEW EDGE\n");
            edges++;
            i++;
        }
        init_mst(vertices,node_tree,edges);
        //qsort(ed,edges,sizeof(Edge), sort_edge);
        kruskal(node_tree,vertices,edges);
        printf("%d\n",get_cost(edges));
    }
    
    return 0;
} 
// end of main program

void kruskal(int node_tree[],int vertices,int edges){
    int from,to;
    int new_set,replace_set;
    int i,j;

    for(j = 0; j < edges;j++){
        from = ed[j].from;
        to = ed[j].to;
        if(node_tree[from] != node_tree[to]){
            ed[j].in_tree = 1;
            new_set = node_tree[from];
            replace_set = node_tree[to];
            for(i = 0;i < vertices;i++){
                if(node_tree[i] == replace_set) node_tree[i] = new_set;
            }
        }            
    }    

    return ;
}


void insert_edge_array(int from,int to,int weight,int pos){

    ed[pos].from = from;
    ed[pos].to = to;
    ed[pos].weight = weight;
    ed[pos].in_tree = 0;
        
    return ;

}
    

void init_mst(int vertices,int node_tree[],int edges){
    int i;

    for(i = 0;i < vertices; i++){
        node_tree[i] = i;
    }

    for(i = 0; i < edges; i++){
        ed[i].in_tree = 0;
    }
}

void sort_e(int edges,int pos){
    int i;
    Edge aux;
    for(i = 0; i < edges; i++){
        if(ed[pos].weight < ed[i].weight){
            aux = ed[i];
            ed[i] = ed[pos];
            ed[pos] = aux;
        }
    }
}

int get_cost(int edges){
    int i;
    int cost = 0;
    for(i = 0; i < edges; i++){
        if(ed[i].in_tree) cost += ed[i].weight;
    }
    return cost;
}

void print_edges(int edges){
    int i;
    for( i = 0; i < edges; i++){
        printf("%d: (%d,%d) %d: %d\n",i,ed[i].from,ed[i].to,ed[i].weight,ed[i].in_tree);
    }
}

