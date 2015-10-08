/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Laboratório 5 - Parte B - Marcha dos Pinguins
    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NMAX 2002

int in_line = 0;
int total_flow = 0;

typedef struct SVertex{
    int vert;
    int flow;
    int line;
    int next;
    int x,y;
    int n_penguins;
}SVertex;

// functions
void create_block(SVertex l[NMAX][NMAX],int x,int y,int max_jumps,int from,int n_penguins);
int create_graph(SVertex l[NMAX][NMAX],int vertices,float jumps);
void bfs(SVertex **l,int line_pos,int line[],int parent[],int vertices,int dest);
int get_min_edge(SVertex **l,int dest,int parent[]);
void update_flow(SVertex **l,int min_edge,int parent[],int dest);
int edmonds_karp(SVertex **l,int source,int dest,int vertices);
void print_list2(SVertex **l,int vertices);


// main program
int main(){
    clock_t end,begin = clock();
    double time_spent;
    static SVertex list[NMAX][NMAX]; // graph G
    SVertex **g;
    int first = 1;
    int vertices,new_vertices;
    int i,j,k,x,y,p,m;
    int prev,flow,max_flow = 0;
    float jump;

    scanf("%d %f",&vertices,&jump);
    for(i = 0;i < NMAX;i++)
        for(j = 0;j < NMAX;j++){
            list[i][j].vert = -1;
            list[i][j].next = -1;
            list[i][j].line = -1;
        }

    // create ice blocks 
	for(i = 0; i < vertices; i++){
        scanf("%d %d %d %d",&x,&y,&p,&m);
        max_flow += p;
        create_block(list,x,y,m,i,p);
    }

    new_vertices = create_graph(list,vertices,jump);

    g = malloc(new_vertices * sizeof(SVertex*));
    for (i = 0; i < new_vertices; i++) {
        g[i] = malloc(new_vertices * sizeof(SVertex));
    }

    // link matriz
    for(i = 0; i < new_vertices;i++){
        prev = i;
        for(j = 0; j < new_vertices;j++){
            if(list[i][j].vert != -1){
                list[i][prev].next = j;
                prev = j;
            }
        }
    }
    
    // find all blocks
    for( i = 0; i < vertices; i ++){
        //clone graph
        for(k = 0; k < new_vertices; k++){
            for(j = k; j != -1; j = list[k][j].next){
                g[k][j] = list[k][j];
            }
        }
        //print_list2(g,new_vertices);
        //break;
        flow = edmonds_karp(g,new_vertices-1,i,new_vertices);
        //printf("%d:%d\n",i,flow);
        if(max_flow == flow){
            if(first){
                printf("%d",i);
                first = 0;
            }
            else
               printf(" %d",i);
        }
    }
    if(first) printf("-1");
    printf("\n");
    for (i = 0; i < new_vertices; i++) {
        free(g[i]);
    }
    free(g);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("%f\n",time_spent);
    return 0;
} // end of main program

void create_block(SVertex l[NMAX][NMAX],int x,int y,int max_jumps,int from,int n_penguins){
    l[from][from].flow = max_jumps;
    l[from][from].x = x;
    l[from][from].y = y;
    l[from][from].n_penguins = n_penguins;

    return ;
}

int create_graph(SVertex l[NMAX][NMAX],int vertices,float jumps){

    int i,j;
    float dist;
    int new_vertices = vertices;
    //printf("%d\n",new_vertices);
    // create a image for each ice block
    // to ensure flow limitarion
    for(i = 0; i < vertices;i++){
        l[i][new_vertices].vert = new_vertices;
        l[i][new_vertices].flow = l[i][i].flow;
        l[new_vertices][i].vert = i;
        l[new_vertices][i].flow = 0;
        
        new_vertices++;
    }
    //printf("%d\n",new_vertices);
    for(i = 0; i < vertices; i ++){
        for(j = i+1;j < vertices; j++){
            dist = pow(pow(l[j][j].x - l[i][i].x,2)+pow(l[j][j].y - l[i][i].y ,2),0.5);
            // create new vertice
            if(dist <= jumps ){
                //image to next
                l[vertices+i][j].vert = j;
                l[vertices+i][j].flow = l[i][i].flow;
                //back edge
                l[j][vertices+i].vert = vertices+i;
                l[j][vertices+i].flow = 0;
                
                //next to source
                l[vertices+j][i].vert = i;
                l[vertices+j][i].flow = l[j][j].flow;
                // back edge
                l[i][vertices+j].vert = vertices+j;
                l[i][vertices+j].flow = 0;
             }
        }
    }
    // source node
    for(i = 0; i < vertices;i++){
        l[new_vertices][i].vert = i;
        l[new_vertices][i].flow = l[i][i].n_penguins;

        l[i][new_vertices].vert = new_vertices;
        l[i][new_vertices].flow = 0;

    }

    return new_vertices + 1;
}


int edmonds_karp(SVertex **l,int source,int dest,int vertices){
    int parent[NMAX],line[NMAX];
    total_flow = 0;
    do{
        line[0] = source;
        line[1] = -1;
        in_line += 1;
        l[source][source].line = in_line;
        parent[dest] = -1;
        parent[source] = -1;
        bfs(l,0,line,parent,vertices,dest);
        if(parent[dest] != -1)
            update_flow(l,get_min_edge(l,dest,parent),parent,dest);
        
    } while(parent[dest] != -1);
   
    return total_flow;
}


void bfs(SVertex **l,int line_pos,int line[],int parent[],int vertices,int dest){
    int j;      
    int next_pos,next_node,current_node;

    next_pos = line_pos+1;
    while(line[line_pos] != -1){
        current_node = line[line_pos];
            for(j = l[current_node][current_node].next; j != -1; j = l[current_node][j].next){
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

void update_flow(SVertex **l,int min_edge,int parent[],int dest){
    int i;

    for(i = dest; parent[i] != -1; i = parent[i]){
        l[parent[i]][i].flow -= min_edge;
        l[i][parent[i]].flow += min_edge;
    }
    total_flow += min_edge;

    return ;
}

int get_min_edge(SVertex **l,int dest,int parent[]){
    int min = l[parent[dest]][dest].flow;
    int i;

    for(i = dest;parent[i]!=-1;i = parent[i]){
        if(min > l[parent[i]][i].flow)
            min = l[parent[i]][i].flow;
    }
    return min;
}

void print_list2(SVertex **l,int vertices){

    int i,j;

    for(i = 0;i < vertices;i++){
        printf("%d:->",i);
        for(j = l[i][i].next; j != -1; j = l[i][j].next){
            printf("%d:%d(%d)->",l[i][j].vert,l[i][j].flow,l[i][j].next);
        }
        printf("\n");
    
    }

    return ;
}



