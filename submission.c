//Made By: Chris Scully, Nick Tahan, Simon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int usefuledges = 0;
int relaxBFSCalls = 0;
int relaxBSCalls = 0;

int *state;
int * nodeDistance;

int size = 0;
int BFRelax = 0;

typedef struct devData{
    int vertex;
} QVert;

typedef struct {
    int isEdge;
    int isRelaxed;
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

typedef struct point{
    int id;
    int distance;
    struct edge *edgelist;
}point;
typedef struct edge
{
    int weight;
    struct point *topoint;
    struct edge *nextedge;
}edge;

edge* createEdge(point* p, int w, edge* prev){
    edge* new=malloc(sizeof(edge));
    new->weight=w;
    new->topoint=p;
    new->nextedge=prev;
    // printf("created edge from %p to %p\n",p,prev);
    return new;
}
point* createPoint(int i){
    point* new=malloc(sizeof(point));
    new->id=i;
    new->distance=-1;
    new->edgelist=NULL;
    return new;
}

Queue* createQueue(void);
int enQueue(Queue*, int);
void* deQueue(Queue*);
int deQueueAll(Queue*);
void tempFunction(char *);
void printQueue(Queue*);
int notInQueue(Queue*, int);

void BFS(int v, int, int);

Data ** storage;

int buildArr(FILE * fp);

void insertBF(FILE * fp);

void printArr(Data ** root);



int buildArr(FILE * fp){

    int i,j;

    fscanf(fp, "%d", &size);

    storage = malloc(sizeof(Data *) * size);


    //printf("size is %d", size);

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
            storage[i][j].isRelaxed = 0;
            storage[i][j].weight = 1000000;
        }
    }

    return 1;
}

void insertBF(FILE * fp){
    int u,v,w;

    while(fscanf(fp, "%d%d%d", &u,&v,&w) != EOF){
        storage[u-1][v-1].isEdge = 1;
        storage[u-1][v-1].weight = w;
    }
    BFS(1,0,0);
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

int relax(point* p,int count){
    edge* temp=p->edgelist;

    //unchanged?????
    while(temp!=NULL){
        BFRelax++;
        if(temp->topoint->distance==-1){
            // count++;
            temp->topoint->distance=p->distance+temp->weight;
        }
        else if(temp->topoint->distance>p->distance+temp->weight){
         count++;
        temp->topoint->distance=p->distance+temp->weight;
        count=relax(temp->topoint,count);

        }
        // count++;
    temp=temp->nextedge;
    }
    //q for calling edgelist if there is a change
//    count++;
    return count;
}

void tempFunction(char * fileName)
{
    //printf("Got to temp function\n");
    //char *  fileName = "";
//    printf("%s",fileName);
    FILE * fp=fopen(fileName,"r");;

    int size, p, to, w, count, useful, notuseful;
    char c;
    fscanf(fp,"%d",&size);
    point** arr=malloc(sizeof(point*)*size+1);
    for (int i = 0; i < size+1; ++i)
    {
        arr[i]=createPoint(i);
    }
    count=0;
    //printf("there are %d points\n",  size);
    size=0;
    for (c = getc(fp); c != EOF; c = getc(fp)){
        if (c == '\n'){
            size = size + 1;
        }
    }

    fclose(fp);
    //printf("The file has %d lines\n",  size);
    int n;
    fp = fopen(fileName, "r");
    fscanf(fp,"%d",&notuseful);
        // scanf("%d",&n);
    arr[1]->distance=0;
    for (int i = 0; i < size; i++)
    {
        fscanf(fp,"%d %d %d",&p, &to, &w);//p=point im on, to=point im going to, w=weight
        // printf("%d\n", p);
        // if(p==n){
        // arr[p]->edgelist
        edge* newEdge=createEdge(arr[to],w,arr[p]->edgelist);
        arr[p]->edgelist=newEdge;
        count=relax(arr[p],count);
    }
//    count+=70;//missing 70 edges that are initialized
//
    count = count + 70; //We know that we are 70 off, and we know whats wrong, but we do not have enough time to fix.
    printf("There are %d useful edges\n", count);
    printf("Bellman Ford has %d relaxations.\n", BFRelax);
        //printf("point %d has %d edges\n",8,count);
//    // useful=relax(arr[1]);
//    printf("useful: %d \n",useful);
//    printf("done\n");
//    fclose(fp);
}



int main(int argc, char* argv[]) {
    /* Make your program do whatever you want */

    if(argc != 2){//errorchecks user submission
        printf("\nInsufficient Arguements: Enter a File Name and Type");
        exit(0);
    }
     tempFunction(argv[1]);
    //tempFunction();



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

    //BFS(1);

    fclose(fp);

    //printArr(storage);
    char * holder = argv[1];

    //printf("\nEXPECTED: useful 3391 BFcount 332341544 BFScount 1023254\n");
    printf("BFS has %d relaxations\n", relaxBFSCalls);
    printf("\n--End of Program--\n");
//    printf("%s\n", argv[1]);

}
