#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MAX_LEN 256
#define CALLOC(p,n,s)\
    if(!((p)=calloc(n,s))) {\
        fprintf(stderr, "Insufficient memory");\
        exit(EXIT_FAILURE);\
    }

/* --- Graph Data Type --- */
typedef struct {
    int **adjMatrix;
    int n;
} Graph;
Graph *createGraph(int n);
void bfs(Graph *graph, int v);
void freeGraph(Graph *graph);
void printGraph(Graph *graph);


/* --- Queue Data Type --- */
typedef struct {
    int *items;
    int front;
    int rear;
} Queue;
Queue *createQueue(int n);
void push(Queue *queue, int item);
int pop(Queue *queue);
void freeQueue(Queue *queue);


/* --- other functions --- */
int readSize();
void readData(Graph *graph);

int main() {
    Graph *graph;
    int n;

    // first input to determine size
    n = readSize();
    if(n <= 0) {
        printf("ERROR of n");
        exit(EXIT_FAILURE);
    }

    // create an empty set
    graph = createGraph(n);

    // read adjacent matrix data
    readData(graph);

    bfs(graph, 0);

    freeGraph(graph);

    return 0;
}

Graph *createGraph(int n) {
    Graph *graph;
    CALLOC(graph, 1, sizeof(Graph));
    CALLOC(graph->adjMatrix, n, sizeof(int*));
    for(int i = 0; i < n; i++) {
        CALLOC(graph->adjMatrix[i], n, sizeof(int));
    }
    graph->n = n;

    return graph;
}

void freeGraph(Graph *graph) {
    for(int i = 0; i < graph->n; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
}

void printGraph(Graph *graph) {
    int n = graph->n;
    printf("------ MATRIX ------\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d ", graph->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

void readData(Graph *graph) {
    char line[MAX_LEN];
    int n = graph->n;

    // iterate every row
    for(int i = 0; i < n; i++) {
        if(fgets(line, sizeof(line), stdin)) {
            // get the content of the row, iterate with characters of the line
            for(int j = 0, k = 0; j < n && line[k] != '\0'; k++) {
                if(line[k] == '0' || line[k] == '1') {
                    graph->adjMatrix[i][j] = line[k] - '0';
                    j++;
                }
            }
        }
    }
}

void bfs(Graph *graph, int v) {
    bool *visited;
    Queue *queue;

    // init queue and visited
    CALLOC(visited, graph->n, sizeof(bool));
    queue = createQueue(graph->n);

    // proccess the initial vertex
    push(queue, v);
    visited[v] = true;
    printf("%d ", v+1);

    // bfs
    while(queue->front != queue->rear) {
        v = pop(queue);
        for(int i = 0; i < graph->n; i++) {
            // if edge exists and i is not visited
            if(graph->adjMatrix[v][i] && !visited[i]) {
                push(queue, i);
                visited[i] = true;
                printf("%d ", i+1);
            }
        }
    }

    free(visited);
    freeQueue(queue);
}

int readSize() {
    int n = -1;
    char size_str[MAX_LEN];

    if(fgets(size_str, sizeof(size_str), stdin)) {
        int len = strlen(size_str);
        int idx = strcspn(size_str, "\n");
        if(idx != len) {
            //remove '\n'
            size_str[idx] = 0;

            // get the size of matrix
            n = atoi(size_str);
        }
    } else {
        printf("Input size ERROR!\n");
    }

    return n;
}

Queue *createQueue(int n) {
    Queue *queue;
    CALLOC(queue, 1, sizeof(Queue));
    CALLOC(queue->items, n, sizeof(int));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

void push(Queue *queue, int item) {
    (queue->rear)++;
    queue->items[queue->rear] = item;
}

int pop(Queue *queue) {
    (queue->front)++;
    return queue->items[queue->front];
}

void freeQueue(Queue *queue) {
    free(queue->items);
    free(queue);
}