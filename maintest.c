#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int usefuledges = 0;
int relaxBFSCalls = 0;
int relaxBSCalls = 0;

int dummy = 0;
int edgeCount = 0;

int * nodeDistance;

int size = 0;

typedef struct devData{
    int vertex;
} QVert;

typedef struct {
    int isEdge;
    int weight;
    char color;
}Data;

typedef struct node {
    QVert* r;
    struct node* next;
}Node;

typedef struct {
    Node* head;
    Node* tail;
}Queue;

Queue* createQueue(void);
int enQueue(Queue*, int);
void* deQueue(Queue*);
int deQueueAll(Queue*);
void printQueue(Queue*);
int notInQueue(Queue*,int);
void Bellman_Ford(int S, int E, int*, int, int);

void BFS(int v, int, int);

Data ** storage;

int buildArr(FILE * fp);

void insertBF(FILE * fp);

void printArr(Data ** root);

int main(int argc, char** argv) {
	/* Make your program do whatever you want */
    if(argc != 2){//errorchecks user submission
        printf("\nInsufficient Arguements: Enter a File Name and Type");
        exit(0);
    }

    FILE* fp = fopen(argv[1], "r");

    if(!fp){//errorchecks fopen
        printf("\nUnable to Open file %s, Try checking your file\n", argv[1]);
        exit(0);//while not conventionally advised, due to it's unimpactful nature, exit functions are used to avoid looping scanfs
    }

    if(0 == buildArr(fp)){
        printf("Insufficient Storage");
        exit(0);
    }

    int c[size];
    nodeDistance = c;

    insertBF(fp);

    fclose(fp);

    printf("\nuseful %d BFcount %d BFScount %d\n",usefuledges,relaxBSCalls,relaxBFSCalls);
    printf("--End of Program--\n");
}


int buildArr(FILE * fp){

    int i,j;

    fscanf(fp, "%d", &size);

    storage = malloc(sizeof(Data *) * size);

    if(!storage){
        return 0;
    }

    for(i = 0; i < size; i++){
        storage[i] = malloc(sizeof(Data) * size);
        if(!storage[i]){
            return 0;
        }
    }

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            storage[i][j].isEdge = 0;
            storage[i][j].weight = 1000000;
        }
    }

    return 1;
}

void insertBF(FILE * fp){
    int u,v,w;
    int parent[size];

    for(int i=0;i<size;i++){
        nodeDistance[i] = 99999;
        parent[i] = -1;
    }
    nodeDistance[0] = 0;

    while(fscanf(fp, "%d%d%d", &u,&v,&w) != EOF){
        storage[u-1][v-1].isEdge = 1;
        storage[u-1][v-1].weight = w;
        edgeCount++;
        Bellman_Ford(1, edgeCount, parent, u - 1, v - 1);
    }

    BFS(1,0,0);
}

void Bellman_Ford(int S, int E, int * parent, int from, int to)
{
    int wasIncremented = 0;
    if(nodeDistance[from] + (storage[from][to]).weight < nodeDistance[to]){
    int i,j,u,v,k;
        for(i=0;i<size;i++)
        {
            for(j=0;j<size;j++)
            {
                if(((storage[i][j]).isEdge) == 1){
                u = i , v = j;
                if(nodeDistance[u] != 99999 && nodeDistance[u] + (storage[u][v]).weight < nodeDistance[v]){
                    nodeDistance[v] = nodeDistance[u] + (storage[u][v]).weight;
                    if(!wasIncremented){
                        usefuledges++;
                        wasIncremented++;
                    }
                }
                    relaxBSCalls++;
                }
            }
        }
    }
}

void BFS(int v, int from, int to)
{
	int i, j;
    int check = 1;

    QVert * node;
    Queue* queue = createQueue();

    if (queue == NULL){
        printf("\nFAILED\n");
        return;
    }

    v--;

    for(j=0; j<size; j++)
		nodeDistance[j] = 999999;

        nodeDistance[v] = 0;

    check = enQueue(queue, v);
        if (check == -1){
            printf("\nFAILED\n");
            return;
        }

    node = ((QVert*)deQueue(queue));

	while(node)//repeatedly extract vertex
	{
		v = node->vertex;

		for(i=0; i<size; i++)//looking at all areas of vertex
		{
            if(storage[v][i].isEdge == 1){//selects all outgoing edges
                if((nodeDistance[v] + storage[v][i].weight < nodeDistance[i])){//relaxes edges
                    nodeDistance[i] = nodeDistance[v] + storage[v][i].weight;
                    if(notInQueue(queue, i) == 1){
                        enQueue(queue, i);
                    }
                }
                relaxBFSCalls++;
            }
        }
        node = ((QVert*)deQueue(queue));
	}
    deQueueAll(queue);
}

int notInQueue(Queue* queue, int i){
    if(!queue){
        return 0;//kills the method if the pointer is invalid
    }
    if(!queue->head){
        return 1;//kills the method if the queue is empty
    }
    Node * marker = queue->head;
    while(marker){//loops until no more elements
        if(marker->r->vertex == i){
            return 0;
        }
        marker = marker->next;//advances elements
    }
    return 1;
}

void printArr(Data ** root){
    int i,j;

    printf("\n");
    for(i = 0; i < size; i++){
        printf("%d: ", i+1);
        for(j = 0; j < size; j++){
            printf("%d ",storage[i][j].isEdge);
        }
        printf("\n");
    }
}


Queue* createQueue(void){
    Queue * origin = malloc(sizeof(Queue));
    if(!origin){//ensures malloc succeeded
        printf("failed");
        return NULL;
    }
    origin->head = NULL;//avoids any access data from interferring
    origin->tail = NULL;
    return origin;
}

//we malloc a queue element and return the pointer upon success

int enQueue(Queue* queue, int v){
    if(!queue){
        return -1;
    }
    Node * insert = malloc(sizeof(Node));
    QVert * rectmal = malloc(sizeof(QVert));
    if(!insert || !rectmal){//checks mallocs
        return -1;
    }
    rectmal->vertex = v;

    insert->r = rectmal;//binds the nodes

    if(!queue->head){//if its the first element we take somemore steps to fill
        queue->head = insert;
        queue->tail = insert;//bind both head and tail to queue
        insert->next = NULL;//ensures next is null incase
    }
    else {
        queue->tail->next = insert;//ties in both pointers to the tail if its not the first element
        queue->tail = insert;
    }
    return 1;
}

//taking a pointer of the queue, we insert the value of the node tied to a rect struct randomly seeded between 1-10 and attach it to the tail of the list, returns -1 on fail and 1 on success

void* deQueue(Queue* queue){
    if(!queue || !queue->head){//verifies if pointer is valid and elements are in the list
        return NULL;
    }
    Node * hold = queue->head;
    if(queue->head == queue->tail){//deals with edgecase of being the only element in the list
        queue->head = NULL;
        queue->tail = NULL;

    }
    else{
    queue->head = queue->head->next;//sets the new head of list and kicks out old one
    }
    return hold->r;
}

//we a pointer of the queue and validate it, afterwards we remove the next element in the queue and readfust the head of the queue, we return NULL on fail and the pointer of the rectangle if success

int deQueueAll(Queue* queue){
    if(!queue || !queue->head){//verifies if pointer is valid and elements are in the list
        return -1;
    }
    while(queue->head){//loops until there are no more elements
        deQueue(queue);
    }
        return 1;
}

//after validating the taken queue, we scan through all elements of the queue and dequeue each one, returning -1 on fail and 1 on success

void printQueue(Queue* queue){
    if(!queue){
        printf("invalid pointer\n");
        return;//kills the method if the pointer is invalid
    }
    if(!queue->head){
        printf("\nQueue is Empty\n");
        return;//kills the method if the queue is empty
    }
    printf("\n");
    Node * marker = queue->head;
    while(marker){//loops until no more elements
        printf("%d, ",marker->r->vertex);//takes the area of the rectangle and prints it
        marker = marker->next;//advances elements
    }
    printf("\n");
}

//function takes a pointer for a queue and verifies its validity, once done we scan through each element and print the value starting at the head of the queue and print out queue is empty if its empty

void freeQueue(Queue* queue){
    Node * marker = queue->head;
    while(marker){//loops until there's no more elements ie(NULL)
        free(marker);
        marker = marker->next;//advances element
    }
    free(queue);
}

//takes a queue struct and frees any data inside it, once complete, we free the queue
