#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>
#include<stdbool.h>

#define MAX_INPUT_LEN 100
#define MAX_COMMANDS 100

#define LOG2(x) ((log((double)(x))) / (log(2.0)))
#define MALLOC(p,s)\
    if(!(p = malloc(s))) {\
        fprintf(stderr, "Insufficient memory.\n");\
        exit(EXIT_FAILURE);\
    }
#define REALLOC(p,s)\
    if(!(p = realloc(p, s))) {\
        fprintf(stderr, "Insufficient memory");\
        exit(EXIT_FAILURE);\
    }

// f-heap type design
typedef struct _FibNode {
    int key;
    int degree;
    struct _FibNode *left;
    struct _FibNode *right;
    struct _FibNode *parent;
    struct _FibNode *child;
    bool childCut;
} FibNode;
typedef struct _FibHeap {
    int n;
    int maxDegree;
    struct _FibNode *min;
} FibHeap;

// creations
FibHeap *createHeap();
FibNode *createNode(int key);
FibNode **createCons(int n);

// insertions
void insert(FibHeap *heap, int key);
void addNode(FibNode *node, FibNode *min);
void test_insertion_order();
void printRootList(FibHeap* heap);

// deletions
FibNode *extractMin(FibHeap *heap);
void removeNode(FibNode *node);
void joining(FibHeap *heap);
void link(FibNode *root, FibNode *node);
FibNode* searchNode(FibNode *root, int key);
void decreaseKey(FibHeap *heap, int key, int sub);
FibNode *delete(FibHeap *heap, int key);

// control
void operationPanel(FibHeap *heap);
void printHeap(FibHeap *heap);

int main() {
    FibHeap *heap;
    heap = createHeap();
    operationPanel(heap);
    printHeap(heap);
    return 0;
}

FibHeap *createHeap() {
    FibHeap *heap;
    MALLOC(heap, sizeof(FibHeap));

    heap->maxDegree = 0;
    heap->min = NULL;
    heap->n = 0;

    return heap;
}

FibNode *createNode(int key) {
    FibNode *node;
    MALLOC(node, sizeof(FibNode));

    node->degree = 0;
    node->key = key;

    node->child = NULL;
    node->parent = NULL;

    node->left = node;
    node->right = node;

    node->childCut = false;

    return node;
}

FibNode **createCons(int n) {
    FibNode **tree;
    MALLOC(tree, sizeof(FibNode*) * (n + 1));
    for(int i=0; i<(n + 1); i++) {
       tree[i] = NULL;
    }
    return tree;
}

FibNode *extractMin(FibHeap *heap) {

    if(heap->min == NULL) return NULL;

    FibNode *deleted = heap->min;

    while(deleted->child != NULL) {
        FibNode *child = deleted->child;
        removeNode(child);
        if(child->right == child) {
            deleted->child = NULL;
        } 
        else {
            deleted->child = deleted->child->right;
        }

        child->parent = NULL;
        addNode(child, heap->min);
        if(child->key <= heap->min->key) {
            heap->min = child;
        }
    }

    removeNode(deleted);

    if(deleted->right == deleted) {
        heap->min = NULL;
    }
    else {
        heap->min = heap->min->right;
        // joining
        joining(heap);
    }

    heap->n--;
    return deleted;
}

void removeNode(FibNode *node) {
    node->left->right = node->right;
    node->right->left = node->left;
}

void joining(FibHeap *heap) {
    int max_degree = LOG2(heap->n) + 1;
    FibNode **tree = createCons(max_degree + 1);

    while(heap->min != NULL) {
        FibNode *x = heap->min;
        if(heap->min == heap->min->right) {
            heap->min = NULL;
        } 
        else {
            // next min
            heap->min = heap->min->right;
        }

        removeNode(x);
        x->left = x;
        x->right = x;
        
        int degree;
        for(degree=x->degree; tree[degree]; degree++) {
            if(x->key > tree[degree]->key) {
                FibNode *tmp;
                tmp = tree[degree];
                tree[degree] = x;
                x = tmp;
            }
            link(x, tree[degree]);
            tree[degree] = NULL;
        }
        tree[degree] = x;
    }

    for(int i=0; i<(max_degree+1); i++) {
        if(tree[i] != NULL) {
            if(heap->min == NULL) {
                heap->min = tree[i];
            }
            else {
                addNode(tree[i], heap->min);
                if(tree[i]->key <= heap->min->key) {
                    heap->min = tree[i];
                }
            }
        }
    }
}

void link(FibNode *root, FibNode *node) {

    if(root->child == NULL) {
        root->child = node;
    }
    else {
        addNode(node, root->child);
        if(node->key <= root->child->key) {
            root->child = node;
        }
    }

    node->parent = root;
    root->degree++;
    node->childCut = false;
}

void operationPanel(FibHeap *heap) {
    char line[MAX_INPUT_LEN];
    char *command, *key_str, *value_str;

    int count = 0;

    while(fgets(line, sizeof(line), stdin)) {

        line[strcspn(line, "\n")] = 0;

        // #commands: 1~100
        command = strtok(line, " ");
        key_str = strtok(NULL, " ");
        value_str = strtok(NULL, " ");

        // 4 commands to operate: insert, delete, extract-min, decrease
        if (command != NULL) {
            if(strcmp(command, "insert") == 0) {
                int key = atoi(key_str);
                insert(heap, key);
            }
            else if(strcmp(command, "delete") == 0) {
                int key = atoi(key_str);
                FibNode *node = delete(heap, key);
                if(node != NULL) free(node);
            }
            else if(strcmp(command, "extract-min") == 0) {
                FibNode *node = extractMin(heap);
                if(node != NULL) free(node);
            }
            else if(strcmp(command, "decrease") == 0) {
                int key = atoi(key_str);
                int value = atoi(value_str);
                decreaseKey(heap, key, value);
            }
            
            else if(strcmp(command, "printH") == 0) {
                printHeap(heap);
            }

            // to exit
            else if(strcmp(command, "exit") == 0) {
                break;
            }

            else {
                // unknown commands
            }
        }
    }

    // print data we want
}

void insert(FibHeap *heap, int key) {
    FibNode *node;

    if (heap==NULL) return;

    node = createNode(key);

    if(heap->n == 0) {
        heap->min = node;
    }
    else {
        addNode(node, heap->min);
        if(node->key <= heap->min->key) {
            heap->min = node;
        }
    }
    heap->n++;
}

void addNode(FibNode *node, FibNode *min) {
    FibNode *current = min;
    
    // one node
    if(current->right == current) {
        node->right = current;
        node->left = current->left;
        (current->left)->right = node;
        current->left = node;
    }

    // multiple nodes
    else {
        if(node->key < min->key || node->key > min->left->key) {
            node->left = min->left;
            node->right = min;
            node->right->left = node;
            node->left->right = node;
            return;
        }
        for(;;) {
            // left < node < right
            if(current->right->key > node->key && node->key > current->key) {
                node->right = current->right;
                node->left = current;
                node->right->left = node;
                node->left->right = node;
                break;
            }
            current = current->right;
            if(current == min) break;
        }
    }
}

void test_insertion_order() {
    int n;
    int origin_list[100];
    FibHeap *heap;
    heap = createHeap();

    //scanf("%d", &n);

    for(int i=0; i<n; i++) {
        int key = rand() % 100;
        origin_list[i] = key;
        insert(heap, key);
    }

    for(int i=0; i<n; i++) {
        // orignal sequence
        printf("%d ", origin_list[i]);
    } printf("\n\tV\n\tV\n\tV\n\tV\n");

    printRootList(heap);  
}

void printRootList(FibHeap* heap) {
    FibNode *current;
    for(current=heap->min;;) {
        // sorted
        printf("%d ", current->key);
        current = current->right;
        if(current == heap->min) break;
    } printf("\n");

    printf("#node=%d\n", heap->n);
}

FibNode* searchNode(FibNode *root, int key) {
    FibNode *t = root;
    FibNode *p = NULL;

    if (root==NULL)
        return root;

    do { // dfs
        if (t->key == key) {
            p = t;
            break;
        }
        else {
            if ((p = searchNode(t->child, key)) != NULL)
                break;
        }
        t = t->right;
    } while (t != root);

    return p;
}

void single_cut(FibHeap *heap, FibNode *node, FibNode *parent) {
    removeNode(node);
    parent->degree--;

    // node has no siblings
    if(node == node->right) {
        parent->child = NULL;
    }
    else {
        // point to the next child
        parent->child = node->right;
    }

    node->parent = NULL;
    node->left = node;
    node->right = node;
    node->childCut = false;

    addNode(node, heap->min);
    if(node->key <= heap->min->key) {
        heap->min = node;
    }
}

/**
 * Current node was originally a parent of a node which was cut recently.
 * So that we want to know whether we continue to cut the current node or not.
 */
void cascading_cut(FibHeap *heap, FibNode *node) {
    FibNode *parent = node->parent;

    // no parent of the node, we simply terminate cascadingCut()
    if(parent == NULL) return;

    // it has its parent, so we check "childCut" field
    if(node->childCut) {
        single_cut(heap, node, parent);
        cascading_cut(heap, parent);
    }
    else {
        node->childCut = true;
    }
}

void decreaseKey(FibHeap *heap, int key, int sub) {
    // find the node
    FibNode *node = searchNode(heap->min, key);
    FibNode *parent;

    if(heap==NULL || heap->min==NULL || node == NULL) return;

    node->key -= sub;
    parent = node->parent;

    if(parent != NULL && node->key < parent->key) {
        // cut current node, and add to root list
        single_cut(heap, node, parent);

        // continue to check to cut
        cascading_cut(heap, parent);
    }
    if(node->key <= heap->min->key) {
        heap->min = node;
    }
}

FibNode *delete(FibHeap *heap, int key) {
    if(heap == NULL || heap->min == NULL) return NULL;
    decreaseKey(heap, key, key);
    return extractMin(heap);
}

void level_order(FibNode *root) {
    FibNode *queue[1000];
    queue[0] = root;
    int front = 0;
    int rear = 1;

    while (front < rear) {
        FibNode *node = queue[front++];
        printf("%d ", node->key);

        if (node->child != NULL) {
            FibNode *child = node->child;
            do {
                queue[rear++] = child;
                child = child->right;
            } while (child != node->child);
        }
    }
    printf("\n");
}

void printHeap(FibHeap *heap) {
    int max_degree = LOG2(heap->n) + 1;
    int size = max_degree + 1;
    FibNode **tree;

    MALLOC(tree, sizeof(FibNode*)*(size));

    for (int i = 0; i < size; i++) {
        tree[i] = NULL;
    }

    for(FibNode *current=heap->min;;) {
        tree[current->degree] = current;
        current = current->right;
        if(current == heap->min) break;
    }
    
    for (int i = 0; i < size; i++) {
        if(tree[i] != NULL) {
            level_order(tree[i]);
        }
    }
}