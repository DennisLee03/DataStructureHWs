#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define INIT_HEAP_SIZE 64
#define MAX_INPUT_LEN 100
#define MALLOC(p,s)\
    if(!(p = malloc(s))) {\
        fprintf(stderr, "Insufficient memory.\n");\
        exit(EXIT_FAILURE);\
    }
#define REALLOC(p, size)\
    if(!(p=realloc(p,size))) {\
        fprintf(stderr, "Insufficient memory.\n");\
        exit(EXIT_FAILURE);\
    }

typedef struct {
    int *items;
    int size;
    int n;
} Heap;

Heap *create(int size);
bool isEmpty(Heap *heap);
bool isFull(Heap *heap);
void insert(Heap *heap, int item);
void delete(Heap *heap, int item);
int search(Heap *heap, int item);
void heapFull(Heap *heap);
void heap_traversal(Heap *heap);
void freeHeap(Heap *heap);

int main() {
    Heap *min_heap;
    min_heap = create(INIT_HEAP_SIZE);
    char line[MAX_INPUT_LEN];
    char *command, *num_str;
    int number;

    while(fgets(line, sizeof(line), stdin)) {
        command = strtok(line, " ");
        num_str = strtok(NULL, " ");

        if (command != NULL && num_str != NULL) {
            int num = atoi(num_str);
            if(strcmp(command, "insert") == 0) {
                //printf("INSERT: %d", num);
                insert(min_heap, num);
            }
            else if(strcmp(command, "delete") == 0) {
                //printf("DELETE: %d", num);
                delete(min_heap, num);
            }
            else {
                printf("UNKNOWN command!\n");
            }
        }
    }

    heap_traversal(min_heap);

    freeHeap(min_heap);

    return 0;
}

Heap *create(int size) {
    Heap *heap;
    MALLOC(heap, sizeof(Heap));
    MALLOC(heap->items, sizeof(int)*size);
    heap->n = 0;
    heap->size = size;

    return heap;
}

bool isEmpty(Heap *heap) {
    return heap->n == 0;
}

bool isFull(Heap *heap) {
    return heap->n == heap->size - 1;
}

// bubble up
void insert(Heap *heap, int item) {
    if(isFull(heap)) {
        heapFull(heap);
    }

    // 先定位到可能被加入的初始節點處
    int i = ++(heap->n);
    // 優先檢查是否為 root: 如果是就直接 insert 在 root, 如果不是在 root 就往上檢查其此處的 parent
    while(i != 1 && (item < heap->items[i/2])) {
        // 把 parent 往下拉
        heap->items[i] = heap->items[i/2];
        // position 移到原 parent 處
        i /= 2;
    }
    heap->items[i] = item;
}

// trickle down
void delete(Heap *heap, int item) {
    if(isEmpty(heap)) {
        fprintf(stderr, "Heap is EMPTY!\n");
        exit(EXIT_FAILURE);
    }

    int parent = search(heap, item);
    if(parent > 0) {
        int child = 2*parent;

        // the element to be deleted
        int deleted = heap->items[parent];
        
        // use last element to adjust heap
        int tmp = heap->items[(heap->n)--];

        while(child <= heap->n) {

            // 如果 child < n 代表此 parent 有 right child，選擇 left child, right child 之間較小的
            if((child < heap->n) && (heap->items[child] > heap->items[child+1])) child++;

            // 把 last element 與 selected child 做比較，如果 last element 比較大就要繼續下沉，反之就不潛入了直接放在這裡
            if(tmp <= heap->items[child]) break;

            heap->items[parent] = heap->items[child];
            parent = child;
            child *= 2;
        }

        heap->items[parent] = tmp;
    }
}

int search(Heap *heap, int item) {
    int idx = -1;
    // sequential search
    for(int i = 1; i <= heap->n; i++) {
        if(item == heap->items[i]) {
            idx = i;
            break;
        }
    }
    return idx;
}

void heapFull(Heap *heap) {
    REALLOC(heap->items, 2*heap->size*sizeof(int));
    heap->size *= 2;
}

void heap_traversal(Heap *heap) {
    for(int i = 1; i <= heap->n; i++) {
        printf("%d ", heap->items[i]);
    }
}

void freeHeap(Heap *heap) {
    free(heap->items);
    free(heap);
}