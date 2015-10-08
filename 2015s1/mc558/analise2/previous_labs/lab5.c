/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Edmonds-Karp
    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 1001

int in_line = 0;
int total_flow = 0;

typedef struct SVertex{
    int vert;
    int flow;
    int is_residual;
    int line;
    int next;
}SVertex;

// functions 
void creat_list_edge(SVertex l[NMAX][NMAX],int from,int to,int flow,int is_residual);
void print_list(SVertex l[NMAX][NMAX],int vertices);
void print_list2(SVertex l[NMAX][NMAX],int vertices);
void bfs(SVertex l[][NMAX],int line_pos,int line[],int parent[],int vertices);
int get_min_edge(SVertex l[][NMAX],int dest,int parent[]);
void update_flow(SVertex l[][NMAX],int min_edge,int parent[],int dest);
void edmonds_karp(SVertex l[][NMAX],int source,int dest,int vertices);


// main program
int main(){
    static SVertex list[NMAX][NMAX]; // graph G
    int vertices,edges,source,dest;
    int i,j;
    int from,to,flow;	
    int prev = 0;

    scanf("%d %d %d %d",&vertices,&edges,&source,&dest);

    for(i = 0;i < vertices;i++)
        for(j = 0;j < vertices;j++){
            list[i][j].vert = -1;
            list[i][j].next = -1;
        }

    // read edges 
	for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&flow);
        creat_list_edge(list,from,to,flow,0);
        creat_list_edge(list,to,from,0,1);
    }
    // link matriz
    for(i = 0; i < vertices;i++){
        prev = 0;
        for(j = 1; j < vertices;j++){
            if(list[i][j].vert != -1){
                list[i][prev].next = j;
                prev = j;
            }
        }
    }
    //print_list2(list,vertices);
    edmonds_karp(list,source,dest,vertices);

    return 0;
} // end of main program

void edmonds_karp(SVertex l[][NMAX],int source,int dest,int vertices){
    int parent[NMAX],line[NMAX];

    do{
        line[0] = source;
        line[1] = -1;
        in_line += 1;
        l[source][source].line = in_line;
        parent[dest] = -1;
        parent[source] = -1;
        bfs(l,0,line,parent,vertices);
        if(parent[dest] != -1)
            update_flow(l,get_min_edge(l,dest,parent),parent,dest);
        
    } while(parent[dest] != -1);
    /*
    for(i = 0; i < vertices; i++)
        printf("%d:%d\n",i,parent[i]);
    
    for(i = dest; i != source;i = parent[i]){
        printf("%d ",parent[i]);
    }
    printf("\nMIN:%d\n",get_min_edge(l,dest,parent));
    */
    printf("%d\n",total_flow);
    return ;
}


void bfs(SVertex l[][NMAX],int line_pos,int line[],int parent[],int vertices){
    int j;      
    int next_pos,next_node,current_node;

    next_pos = line_pos+1;
    while(line[line_pos] != -1){
        current_node = line[line_pos];
        for(j = 0; j != -1; j = l[current_node][j].next){
            next_node = l[current_node][j].vert;
            
            // if flow is greater then 0 and next node is not in the line
            if(l[current_node][j].flow > 0 && l[next_node][next_node].line != in_line){
                l[next_node][next_node].line = in_line;
                line[next_pos] = next_node;
                parent[next_node] = current_node;
                next_pos += 1;
                line[next_pos] = -1;
                }
        }
        line_pos += 1;
    }   
    
    return;
}

void update_flow(SVertex l[][NMAX],int min_edge,int parent[],int dest){
    int i;

    for(i = dest; parent[i] != -1; i = parent[i]){
        l[parent[i]][i].flow -= min_edge;
        l[i][parent[i]].flow += min_edge;
    }
    total_flow += min_edge;

    return ;
}

int get_min_edge(SVertex l[][NMAX],int dest,int parent[]){
    int min = l[parent[dest]][dest].flow;
    int i;

    for(i = dest;parent[i]!=-1;i = parent[i]){
        if(min > l[parent[i]][i].flow)
            min = l[parent[i]][i].flow;
    }
    return min;
}

void creat_list_edge(SVertex l[NMAX][NMAX],int from,int to,int flow,int is_residual){

    l[from][to].vert = to;
    l[from][to].flow = flow;
    l[from][to].is_residual = is_residual;
    l[from][to].line = -1;
    l[from][to].next = -1;

}


void print_list(SVertex l[NMAX][NMAX],int vertices){

    int i,j;

    for(i = 0;i < vertices;i++){
        printf("%d:->",i);
        for(j = l[i][0].next; j != -1;j = l[i][j].next){
            printf("%d:%d->",l[i][j].vert,l[i][j].flow);
        }
        printf("\n");
    
    }

    return ;
}

void print_list2(SVertex l[NMAX][NMAX],int vertices){

    int i,j;

    for(i = 0;i < vertices;i++){
        printf("%d:->",i);
        for(j = 1; j < vertices; j++){
            if(l[i][j].vert != -1)
                printf("%d:%d->",l[i][j].vert,l[i][j].flow);
        }
        printf("\n");
    
    }

    return ;
}


