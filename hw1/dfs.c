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
void dfs(Graph *graph, int v, bool *visited);
void freeGraph(Graph *graph);
void printGraph(Graph *graph);

/* --- other functions --- */
int readSize();
void readData(Graph *graph);

int main() {
    Graph *graph;
    int n;
    bool *visited;
    CALLOC(visited, n, sizeof(bool));

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

    // search
    dfs(graph, 1, visited);

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

void dfs(Graph *graph, int v, bool *visited) {
    // beginning point
    visited[v] = true;
    printf("%d ", v+1);

    //dfs 
    for(int i = 0; i < graph->n; i++) {
        if(graph->adjMatrix[v][i] && !visited[i]) {
            dfs(graph, i, visited);
        }
    }
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