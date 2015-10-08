/*
    MC558 - Análise de Algoritmos II - Turmas A e B
    Parte B: Muitos Caminhos e um Destino
    Marcelo Azevedo Goncalves dos Santos
    RA:106140

*/

#include <stdio.h>
#include <stdlib.h>

#define NMAX 1001
#define INFINITY 100001


typedef struct SVertex{
    int vert;
    int weight;
}SVertex;

typedef struct minHeap{
    int vert;
}minHeap;


// functions 
void creat_list_edge(SVertex l[NMAX][NMAX],int from,int to,int weight);
void print_list(SVertex l[NMAX][NMAX],int vertices);
void insert_minheap(int vert,int source_dist[]);
void delete_minheap(int index, int source_dist[]);
void dijkstra(SVertex l[NMAX][NMAX],int vertices,int was_inserted[],int source_dist[]);
void print_heap(int source_dist[]);
void update_heap(int vert, int source_dist[]);

minHeap heap[NMAX];

// main program
int main(){
    static SVertex list[NMAX][NMAX]; // graph G
    static SVertex h[NMAX][NMAX]; // graph G^t
    int source_dist[NMAX],dest_dist[NMAX];
    int vertices,edges,source,dest,p;//,dest;
    int i,j;
    int from,to,weight;	
    int was_inserted[NMAX];
    int max_edge = -1;

    // initialize variables
    for(i = 0;i < NMAX; i++){
        was_inserted[i] = 0;
        source_dist[i] = INFINITY; 
        dest_dist[i] = INFINITY;
        heap[i].vert = -1;
    }
    //scanf("%d %d %d",&vertices,&edges,&source);
    scanf("%d %d %d %d %d",&vertices,&edges,&source,&dest,&p);
    
    for(i = 0;i < vertices; i++){
        list[i][0].vert = i;
        h[i][0].vert = i;
        list[i][1].vert = -1;
        h[i][1].vert = -1;
    }
    printf("Reading Edges...\n");
    // read edges 
	for(i = 0; i < edges; i++){
        scanf("%d %d %d",&from,&to,&weight);
        creat_list_edge(list,from,to,weight);
        creat_list_edge(h,to,from,weight);
    }
    heap[0].vert = source;
    source_dist[source] = 0;
    was_inserted[source] = 1;
    dijkstra(list,vertices,was_inserted,source_dist);
    printf("Dis\n");
    for(i = 0;i < NMAX; i++){
        was_inserted[i] = 0;
        heap[i].vert = -1;
    }
    heap[0].vert = dest;
    dest_dist[dest] = 0;
    was_inserted[dest] = 1;
    dijkstra(h,vertices,was_inserted,dest_dist);
    /*
    printf("SOURCE:%d\n",source);
    for(i = 0; i < vertices; i++){
        printf("%d:%d ",i,source_dist[i]);
    }
    printf("\nDESTTT:%d\n",dest);
    for(i = 0; i < vertices; i++){
        printf("%d:%d ",i,dest_dist[i]);
    }
    printf("\n");
    */

    //loop throw all edges 
    for(i = 0; i < vertices; i++){
        for(j = 0; list[i][j].vert != -1; j++){
            // if exist a path from source to vert and dest to vert
            if(source_dist[list[i][j].vert] < INFINITY && dest_dist[list[i][j].vert] < INFINITY){ 
                if(source_dist[i] + dest_dist[list[i][j].vert] + list[i][j].weight <= p){
                    if(max_edge < list[i][j].weight){
                        max_edge = list[i][j].weight;
                    }
                }
            }
        }
    }

    printf("%d\n",max_edge);

    return 0;
} // end of main program

void creat_list_edge(SVertex l[NMAX][NMAX],int from,int to,int weight){

    int i;

    for(i = 0;l[from][i].vert != -1; i++);
    l[from][i].vert = to;
    l[from][i].weight = weight;
    l[from][i+1].vert = -1;

}


void dijkstra(SVertex l[NMAX][NMAX],int vertices,int was_inserted[],int source_dist[]){
    int from,to;
    int i;

    from = heap[0].vert;
    heap[0].vert = -1;

    while(from != -1){
        printf("%d\n",from);
        //print_heap();
        for(i = 0; l[from][i].vert != -1; i++){
            to = l[from][i].vert;
            // update source dist
            if(source_dist[to] == INFINITY){
                source_dist[to] = source_dist[from] + l[from][i].weight;
                update_heap(to,source_dist);
            }
            else if(source_dist[to] > source_dist[from] + l[from][i].weight){
                source_dist[to] = source_dist[from] + l[from][i].weight;
                //parent[i] = from;
                update_heap(to,source_dist);
            }
            // insert in the heap if not visited yet
            if(!was_inserted[to]){
                insert_minheap(to,source_dist);
                // set node as visited
                was_inserted[to] = 1;
            }  
        }
        // extract new node
        from = heap[0].vert;
        delete_minheap(0,source_dist);
    }
    return ;
}
void update_heap(int vert,int source_dist[]){
    //printf("Update %d\n",vert);
    int i;
    int parent,son;
    int cond = 0;
    for(i = 0;heap[i].vert != -1;i++){
        if(heap[i].vert == vert){
            cond = 1;
            break;
        }
    }

    while( i != 0 && cond ){
        son = heap[i].vert;
        parent = heap[(i - 1)/2].vert;
        // swap if true
        if(source_dist[son] < source_dist[parent]){
            heap[i].vert = parent;
            heap[(i-1)/2].vert = son;
            i = (i-1)/2;
        }   
        else{
            i = 0;
        }
    }
}
// print the adjaceny list 
void print_list(SVertex l[NMAX][NMAX],int vertices){

    int i,j;

    for(i = 0;i < vertices;i++){
        printf("%d:->",i);
        for(j = 0; l[i][j].vert != -1;j++){
            printf("%d->",l[i][j].vert);
        }
        printf("\n");
    }

    return ;
}

void insert_minheap(int vert,int source_dist[]){

    int i;
    int parent,son;

    for(i = 0; heap[i].vert != -1; i++);
    heap[i].vert = vert;
    
    while( i != 0 ){
        son = heap[i].vert;
        parent = heap[(i - 1)/2].vert;
        // swap if true
        if(source_dist[son] < source_dist[parent]){
            heap[i].vert = parent;
            heap[(i-1)/2].vert = son;
            i = (i-1)/2;
        }   
        else{
            i = 0;
        }
    }

    return ;
}

void delete_minheap(int index,int source_dist[]){

    int i;
    int left_son,right_son,parent; 
    int update = 0;

    // find the last element in the heap
    for(i = 0; heap[i + 1].vert != -1; i++);
    heap[index].vert = heap[i].vert;
    heap[i].vert = -1;
    i = index;

    while(!update){
        parent = heap[i].vert;
        left_son = heap[i*2 + 1].vert;
        right_son = heap[i*2 + 2].vert;
        // both exists
        if(left_son != -1 && right_son != -1){
            // left son smaller then right son
            if(source_dist[left_son] < source_dist[right_son]){
                // left son smaller then parent
                if(source_dist[left_son] < source_dist[parent]){
                    //printf("%d < %d\n",source_dist[left_son],source_dist[parent]);
                    heap[i].vert = left_son;
                    heap[i*2 + 1].vert = parent;
                    i = i*2 + 1;
                }
                else
                    update = 1;
            }
            // right son smaller then left son
            else{
                // right son smaller then parent
                if(source_dist[right_son] < source_dist[parent]){
                    //printf("%d < %d\n",source_dist[right_son],source_dist[parent]);
                    heap[i].vert = right_son;
                    heap[i*2 + 2].vert = parent;
                    i = i*2 + 2;
                } 
                else 
                    update = 1;
            }
        }
        // only left exists
        else if(right_son == -1 && left_son != -1){
            if(source_dist[left_son] < source_dist[parent]){
                //printf("%d < %d\n",source_dist[left_son],source_dist[parent]);
                heap[i].vert = left_son;
                heap[i*2 + 1].vert = parent;
                i = i*2 + 1;
            }
            else
                update = 1;
        }
        // none exists
        else
            update = 1;
    }
    //printf("DEL ERROR\n");
    return ;
}

void print_heap(int source_dist[]){
    int i;

    for(i = 0; heap[i].vert != -1; i++){
        printf("%d ",source_dist[heap[i].vert]);
    }
    printf("\n\n");

    return ;
}


