/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Parte A: Ordenação Toplógica de Grafos Orientados Acíclicos
    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 200

typedef struct SVertex{
    int vert;
    struct SVertex *next;
}SVertex;

SVertex list[NMAX];

// functions 

void create_list_edge(int from,int to);
void print_list(int vertices);
void dfs(int node,int was_finished[],int was_visited[], int vertices);
void finish_node(int node,int was_finished[],int vertices);

int time = 0;

// main program
int main(){
	
    int vertices,edges;
    int i,from,to;	
    int was_finished[NMAX],was_visited[NMAX];

    for(i = 0;i < NMAX; i++){
        was_finished[i] =-1;
        was_visited[i] = 0;   
    }

    scanf("%d %d",&vertices,&edges);

    // read edges 
	for(i = 0; i < edges; i++){
        scanf("%d %d",&from,&to);
        create_list_edge(from,to);
    }

    // while there is a node not visited
    for(i = 0;i < vertices;i++){
        if(!was_visited[i]){
            dfs(i,was_finished,was_visited,vertices);
        }
    }
    // print the topological order 
    for(i = 0; i < vertices; i++){
        printf("%d",was_finished[i]);
        if(i + 1 != vertices ) printf(" ");
    }

    printf("\n");

    return 0;
} // end of main program

void dfs(int node,int was_finished[],int was_visited[], int vertices){

    SVertex *run;    

    // set the node as visited
    was_visited[node] = 1;
    
    // for every edge coming out of 'node'
    for(run = list[node].next; run != NULL; run = run->next){
        if(!was_visited[run->vert]){
            dfs(run->vert,was_finished,was_visited,vertices);
        }
    }
    
    // insert 'node' in the last position of the array
    finish_node(node,was_finished,vertices);

    return ;
}

void create_list_edge(int from,int to){
    SVertex *run,*aux;
    
    // increment vert degree
    list[from].vert += 1;

    aux = (SVertex *) malloc(sizeof(SVertex));
    aux->next = NULL;
    aux->vert = to;

    // run will point to the last element of the list      
    for(run = &list[from]; run->next != NULL; run = run->next);
    
    run->next = aux;
    
    return ;  
}
// print the adjaceny list 
void print_list(int vertices){

    int i;
    SVertex *run;

    for(i = 0;i < vertices; i++){
        printf("%d:",i);
        for( run = list[i].next; run != NULL; run = run->next){
            printf("%d->",run->vert);
        }
        printf("\n");
    }
    return ;
}
// insert the node in the last position of the array
void finish_node(int node,int was_finished[],int vertices){
    int i;
    // find the position to insert the node
    for(i = vertices - 1; was_finished[i] != -1; i--);
    was_finished[i] = node;    

    return;
}




