/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Parte B: Muitos Caminhos e um Destino
    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 200

typedef struct SVertex{
    int vert;
    int color;
    int all_incoming_edges[3];
    int paths;
    struct SVertex *next;
}SVertex;

SVertex list[NMAX]; 

// functions 

void create_list_edge(int from,int to,int color);
void print_list(int vertices);
void dfs(int node,int was_finished[],int was_visited[], int vertices);
void finish_node(int node,int was_finished[],int vertices);

// main program
int main(){
	
    int vertices,edges,start,end;
    int i;
    int from,to,color;	
    int was_finished[NMAX],was_visited[NMAX];
    SVertex *run;

    for(i = 0;i < NMAX; i++){
        was_finished[i] =-1;
        was_visited[i] = 0;
        list[i].all_incoming_edges[0] = 0;
        list[i].all_incoming_edges[1] = 0;   
        list[i].all_incoming_edges[2] = 0;      
        list[i].paths = 0;
    }

    scanf("%d %d %d %d",&vertices,&edges,&start,&end);
    
    list[start].all_incoming_edges[0] = 1;
    list[start].paths = 1;
    // read edges 
	for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&color);
        create_list_edge(from,to,color);
    }

    // while there is a node not visited
    for(i = 0;i < vertices;i++){
        if(!was_visited[i]){
            dfs(i,was_finished,was_visited,vertices);
        }
    }

    //for(i = 0; i < vertices; i ++){
    //    printf("%d G:%d Y:%d R:%d\n",i,list[i].all_incoming_edges[0],list[i].all_incoming_edges[1],list[i].all_incoming_edges[2]);
    //}

    for(i = 0; was_finished[i] != start; i++);
    
    //for(j = 0; was_finished[j] != end; j++);

    // there is no path from start to end nodes
    //if(j <= i ) printf("0\n");
    if( 1 > 2 ) printf("Capai memo\n");    
    else{
        // for each node in the topological order
        for(;was_finished[i] != end;i++){
            // for every edge of node i
            for(run = list[i].next; run != NULL; run = run->next){
                list[run->vert].paths += list[i].all_incoming_edges[0];
                // GREEN edge                
                if(run->color == 0){
                    list[run->vert].paths += list[i].all_incoming_edges[1];
                    list[run->vert].paths += list[i].all_incoming_edges[2];
                }   
                // YELLOW edge
                else if(run->color == 1){
                    list[run->vert].paths += list[i].all_incoming_edges[1];
                }
            }
        }
    
        printf("%d\n",list[end].paths);
    }
    

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

void create_list_edge(int from,int to,int color){
    SVertex *run,*aux;
    
    // increment vert degree
    list[from].vert += 1;

    aux = (SVertex *) malloc(sizeof(SVertex));
    aux->next = NULL;
    aux->vert = to;
    aux->color = color;
    list[to].all_incoming_edges[color] += 1;

    // run will point to the last element of the list      
    for(run = &list[from]; run->next != NULL; run = run->next);
    
    run->next = aux;
    
    return ;  
}
// print the adjaceny list 
void print_list(int vertices){

    int i;
    char color;
    SVertex *run;

    for(i = 0;i < vertices; i++){
        printf("%d:",i);
        for( run = list[i].next; run != NULL; run = run->next){
            if(run->color == 0) color = 'G';
            else if( run->color == 1) color = 'Y';
            else color = 'R';
            printf("(%d,%c)->",run->vert,color);
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




