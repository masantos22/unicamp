/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Parte B: Reconectando
    Marcelo Azevedo Goncalves dos Santos
    RA:106140
*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 10001

typedef struct SEdge{
    int from;
    int to;
    int weight;
    int in_tree;
}Edge;

Edge ed[50000000];
Edge new_eds[50000000];
Edge all_edges[50000000];

// functions 
void kruskal(Edge eds[],int node_tree[],int vertices,int edges);
void insert_edge_array(Edge ed[],int from,int to,int weight,int pos);
void sort_all_e(int edges);
int get_cost(Edge eds[],int edges);
void print_edges(Edge eds[],int edges);

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

    for(i = 0; i < NMAX; i++)
        node_tree[i] = i;

    // read mst
    for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&weight);
        insert_edge_array(ed,from,to,weight,i);
        ed[i].in_tree = 1;
        total_cost += weight;
    }
    ed[i].in_tree = -1;

    scanf("%d",&new_edges);

    if(vertices == 1) printf("0 0\n");
    else{
        printf("%d ",total_cost);
        qsort(ed,edges,sizeof(Edge), sort_edge);
        //print_edges(edges);
        for(j = 0;j < new_edges;j++){
            scanf("%d %d %d",&from,&to,&weight);
            insert_edge_array(new_eds,from,to,weight,j);
            edges++;
            i++;
        }
        new_eds[j].in_tree = -1;
        qsort(new_eds,new_edges,sizeof(Edge), sort_edge);
        sort_all_e(edges);
        kruskal(all_edges,node_tree,vertices,edges);
        printf("%d\n",get_cost(all_edges,edges));
    }
    
    return 0;
} 
// end of main program

void kruskal(Edge eds[],int node_tree[],int vertices,int edges){
    int from,to;
    int new_set,replace_set;
    int i,j;

    for(j = 0; j < edges;j++){
        from = eds[j].from;
        to = eds[j].to;
        if(node_tree[from] != node_tree[to]){
            eds[j].in_tree = 1;
            new_set = node_tree[from];
            replace_set = node_tree[to];
            for(i = 0;i < vertices;i++){
                if(node_tree[i] == replace_set) node_tree[i] = new_set;
            }
        }            
    }    

    return ;
}


void insert_edge_array(Edge ed[],int from,int to,int weight,int pos){

    ed[pos].from = from;
    ed[pos].to = to;
    ed[pos].weight = weight;
    ed[pos].in_tree = 0;
        
    return ;

}

void sort_all_e(int edges){
    int i = 0,j = 0,pos = 0;

    while(pos < edges){
        //printf("POS: %d ",pos);
        if(ed[i].in_tree == -1){
            //printf("INSERT %d\n", new_eds[j].weight);
            all_edges[pos] = new_eds[j];
            j++;
        }
        else if(new_eds[j].in_tree == -1){
            //printf("INSERT %d\n", ed[i].weight);
            all_edges[pos] = ed[i];
            i++;
        }
        else{
            if(new_eds[j].weight < ed[i].weight){
                //printf("INSERT %d\n", new_eds[j].weight);
                all_edges[pos] = new_eds[j];
                j++;
            }
            else{
                //printf("INSERT %d\n", ed[j].weight);
                all_edges[pos] = ed[i];
                i++;     
            }
        }
        all_edges[pos].in_tree = 0;
        pos++;
    }
    return ;
}

int get_cost(Edge eds[],int edges){
    int i;
    int cost = 0;
    for(i = 0; i < edges; i++){
        if(eds[i].in_tree) cost += eds[i].weight;
    }
    return cost;
}

void print_edges(Edge eds[],int edges){
    int i;
    for( i = 0; i < edges; i++){
        printf("%d: (%d,%d) %d: %d\n",i,eds[i].from,eds[i].to,eds[i].weight,eds[i].in_tree);
    }
}

