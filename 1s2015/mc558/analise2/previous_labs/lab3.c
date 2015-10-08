/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Kruskal Algorithm
    Marcelo Azevedo Goncalves dos Santos
    RA:106140
*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 2000

typedef struct SVertex{
    int vert;
    int weight;
    struct SVertex *next;
}SVertex;

// linked list to order the edges
typedef struct SEdge{
    int from;
    int to;
    int weight;
    int in_tree;
    struct SEdge *next;
}Edge;

Edge ed[100000000];
SVertex list[NMAX];

// functions 
void create_list_edge(int from,int to,int color);
void print_list(int vertices);
Edge * insert_edge(Edge *head,int from,int to, int w);
void print_edges(Edge * head);
void free_edges(Edge * head);
void free_list(int vertices);
void kruskal(int node_tree[],int vertices,int edges);
void total_weight(int vertices);
void insert_edge_array(int from,int to,int weight,int pos);

int sort_edge(const void *a, const void *b) 
{ 
    Edge *p1 = (Edge *)a;
    Edge *p2 = (Edge *)b;
    return p1->weight - p2->weight;
} 

// main program
int main(){
	
    //int start = -1;
    //Edge * head = NULL;
    int vertices,edges;
    int from,to,weight;	
    int i;
    int node_tree[NMAX];

    scanf("%d %d",&vertices,&edges);    
    // initially every node is in it own's tree
    for(i = 0; i < vertices;i++){
        node_tree[i] = i;
    }

    // read edges 
	for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&weight);
        //create_list_edge(from,to,weight);
        //create_list_edge(to,from,weight);
        //head = insert_edge(head,from,to,weight);
        insert_edge_array(from,to,weight,i);
    }
    
    if(edges == 0) printf("0\n");
    else{
        /* sort array using qsort functions */ 
        qsort(ed,edges,sizeof(Edge), sort_edge);
        //for(i=0;i < edges;i++){
        //   printf("( %d->%d ):  %d IN:%d\n",ed[i].from,ed[i].to,ed[i].weight,ed[i].in_tree);
        //}
        //head = &ed[start];
        //printf("HEAD %d\n",start);
        kruskal(node_tree,vertices,edges);
        //print_list(vertices);
        //print_edges(head);
        total_weight(edges);
        //free_edges(head);
        //free_list(vertices);
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



// inserts the edge in order of increasing weight
Edge * insert_edge(Edge *head,int from,int to, int w){
    Edge *aux,*run,*prev;
    int not_inserted = 1;

    // create edge
    aux = (Edge *) malloc(sizeof(Edge));
    aux->next = NULL;
    aux->from = from;
    aux->to = to;
    aux->weight = w;
    aux->in_tree = 0;
    
    if(head == NULL) head = aux;
    // not the first insertion
    else{
        // case we need to change the head
        if(aux->weight < head->weight){
            aux->next = head;
            head = aux;
        }
        else{
            prev = head;
            while(not_inserted){
                run = prev->next;
                // insert in the last position
                if(run == NULL){
                    prev->next = aux;
                    not_inserted = 0;
                }
                // insert in the middle of the list
                else{
                    // insert case
                    if(aux->weight < run->weight){
                        prev->next = aux;
                        aux->next = run;
                        not_inserted = 0;
                    }
                    // update pointers
                    else{
                        prev = run;
                        run = run->next;
                    }
                }
            }       
        }
    }
    return head;  
}


void create_list_edge(int from,int to,int weight){
    SVertex *run,*aux;
    
    // increment vert degree
    list[from].vert += 1;

    aux = (SVertex *) malloc(sizeof(SVertex));
    aux->next = NULL;
    aux->vert = to;
    aux->weight = weight;

    // run will point to the last element of the list      
    for(run = &list[from]; run->next != NULL; run = run->next);
    
    run->next = aux;
    
    return ;  
}
// print the adjacency list 
void print_list(int vertices){
    int i;
    SVertex *run;

    for(i = 0;i < vertices; i++){
        printf("%d:",i);
        for( run = list[i].next; run != NULL; run = run->next){
            printf("(node:%d,weight:%d)->",run->vert,run->weight);
        }
        printf("\n");
    }
    return ;
}

void print_edges(Edge * head){
    Edge * run;

    for(run = head; run != NULL; run = run->next){
        printf("%d (%d,%d): %d\n",run->weight,run->from,run->to,run->in_tree);
    }
    printf("\n");
    //for(i = 0;i < 14; i++)
    //    printf("%d (%d,%d): %d\n",ed[i].weight,ed[i].from,ed[i].to,ed[i].in_tree);
    return ;
}

void free_edges(Edge * head){
    Edge *run,*next;
    for(run = head; run != NULL; run = next){
        next = run->next;
        free(run);
    }
}

void free_list(int vertices){
    int i;

    SVertex *run,*next;
    
    for(i = 0;i < vertices; i++){
        for( run = list[i].next; run != NULL; run = next){
            next = run->next;
            free(run);
        }
    }
    return ;
}

void total_weight(int edges){
    int i;
    int total = 0;
    for(i = 0; i < edges; i++){
        if(ed[i].in_tree) total += ed[i].weight;
    }
    printf("%d\n",total);
    return ; 

}

void insert_edge_array(int from,int to,int weight,int pos){

    ed[pos].from = from;
    ed[pos].to = to;
    ed[pos].weight = weight;
    ed[pos].in_tree = 0;
    ed[pos].next = NULL;
        
    return ;

}
    


