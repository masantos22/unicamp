#include <stdio.h>
#include <stdlib.h>

#define NMAX 1000

typedef struct SVertex{
    int vert;
    struct SVertex *next;
}SVertex;

int matriz[NMAX][NMAX];
int aux_matriz[NMAX][NMAX];
int dom_matriz[NMAX][NMAX];

SVertex list[NMAX];

void create_matriz(int matriz[][],int vertices);
void create_list(int vertices);

void create_matriz_egde(int from,int to);
void create_list_edge(int from,int to);

void print_matriz(int vertices);
void print_list(int vertices);

void bfs_matriz(int current_size,int node_number,int line_pos);

int put_in_line(int j);

void free_list(int vertices);

int dist[NMAX];
int vertices;
int global_line[NMAX];

int main(){
	
    int edges,s; // 0/1 direct or not
    int i;
    int from,to;    

    scanf("%d %d %d",&vertices,&edges,&s);

    create_matriz(matriz,vertices);
    create_matriz(aux_matriz,vertices);
    //create_list(vertices);


	// for(i = 0; i < vertices + 1; i++){
	// 	dist[i] = vertices;
	// 	global_line[i] = -1;
	// }

    for(i = 0; i < edges; i++){
        scanf("%d %d",&from,&to);
        //create_list_edge(from,to);
        create_matriz_egde(from,to);
    }
		
	//print_matriz(vertices);
	//printf("\n");

	// initialize line
	global_line[0] = s;
	// initialize distance
	dist[s] = 0;
	bfs_matriz(0,s,0);	

	for(i = 0;i < vertices; i++){
			printf("%d",dist[i]);
			if(i + 1 == vertices ) printf("\n");
			else printf(" ");
	}

    //free_list(vertices);
	return 0;
}

void bfs_matriz(int current_size,int node_number,int line_pos){
		int j;		
		int next_node;

		//printf("BFS:%d\n",node_number);
		//printf("SIZE:%d\n",current_size);

		for(j = 0;j < vertices; j++){
				if(matriz[node_number][j] == 1 ){
						if(put_in_line(j)){
								dist[j] = current_size + 1;
								//printf("%d:%d %d\n",node_number,j,dist[j]);
						}
				}	
		}

		line_pos++;

		if(global_line[line_pos] != -1){
				next_node = global_line[line_pos];
				bfs_matriz(dist[next_node],next_node,line_pos);
		}
}

int put_in_line(int j){
		int i;
		
		for(i = 0; global_line[i] != -1; i++){
				if(global_line[i] == j) return 0;
		}
		//printf("Fila:%d\n",j);
		global_line[i] = j;
		return 1;
}

// initializate matriz with empty egdes
void create_matriz(int vertices){
    
    int i,j;
        
    for(i = 0; i < vertices; i++)
        for(j = 0; j < vertices;j++)
            matriz[i][j] = 0;        

    return;
}

// initialize list with empty egdes
void create_list(int vertices){
    int i;

    for(i = 0;i < vertices; i++){
        list[i].next = NULL;
        list[i].vert = 0;       
    }

    return ;
}

void create_matriz_egde(int from,int to){
    matriz[from][to] = 1;
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

void print_matriz(int vertices){
    int i,j;
    
    for(i = 0;i < vertices; i++){
        for(j = 0; j < vertices; j++){
            printf("%d ",matriz[i][j]);
        }
        printf("\n");
    }
    
    return ;
}

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

void free_list(int vertices){
    int i = 0;
    SVertex *run,*aux;

    for(i = 0; i < vertices; i++)
        for(run = list[i].next; run != NULL;run = aux){
            aux = run->next;
            free(run);
        }

    return ; 
}








