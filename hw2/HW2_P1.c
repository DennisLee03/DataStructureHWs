#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_INPUT_LEN 100
#define MAX_COMMANDS 100

#define MALLOC(p,s)\
    if(!(p = malloc(s))) {\
        fprintf(stderr, "Insufficient memory.\n");\
        exit(EXIT_FAILURE);\
    }

typedef struct position {
    int b;
    int s;
} position;


typedef struct element {
    int key;
    // other fields
} element;

typedef struct HashTable {
    int b;
    int s;
    element **pairs;
} HashTable;

HashTable* create(int b, int s);
void freeTable(HashTable *ht);
int h(int k, int b);
position search(HashTable *ht, int key);
void insert(HashTable *ht, int key);
void delete(HashTable *ht, int key);
void printTable(HashTable *ht);
void setupTableSize(int *bucket, int *slot);
void operationPanel(HashTable *ht);

int main(void) {
    char line[MAX_INPUT_LEN];
    char *command, *num_str;

    int b=0, s=0;

    setupTableSize(&b, &s);
    HashTable *ht;
    ht = create(b, s);

    operationPanel(ht);

    freeTable(ht);

    return 0;
}

void setupTableSize(int *bucket, int *slot) {
    char line[MAX_INPUT_LEN];
    char *command, *num_str;
    int b=0, s=0;
    
    while((b <= 0 || s <= 0) && fgets(line, sizeof(line), stdin)) {
        command = strtok(line, " ");
        num_str = strtok(NULL, " ");

        line[strcspn(line, "\n")] = 0;

        // 2 commands to create hash table
        if (command != NULL && num_str != NULL) {
            int num = atoi(num_str);
            if(strcmp(command, "bucket") == 0) {
                //printf("BUCKET: %d\n", num);
                b = num;
            }
            else if(strcmp(command, "slot") == 0) {
                //printf("SLOT: %d\n", num);
                s = num;
            }
        }
    }

    *bucket = b;
    *slot = s;
}

void operationPanel(HashTable *ht) {
    char line[MAX_INPUT_LEN];
    char *command, *num_str;

    position searched[MAX_COMMANDS];
    int count = 0;

    while(fgets(line, sizeof(line), stdin)) {

        line[strcspn(line, "\n")] = 0;

        // #commands: 1~100
        command = strtok(line, " ");
        num_str = strtok(NULL, " ");

        // 3 commands to operate hash table: search, insert, delete
        if (command != NULL && num_str != NULL) {
            int num = atoi(num_str);
            if(strcmp(command, "search") == 0) {
                position p = search(ht, num);
                searched[count++] = p;
            }
            else if(strcmp(command, "insert") == 0) {
                insert(ht, num);
            }
            else if(strcmp(command, "delete") == 0) {
                delete(ht, num);
            }
        }

        // exit operation
        else if(strcmp(command, "exit") == 0) {
            break;
        }
    }

    for(int i=0; i<count; i++) {
        if(searched[i].b == -1 || searched[i].s == -1) continue;
        printf("%d %d\n", searched[i].b, searched[i].s);
    }
}

HashTable* create(int b, int s) {
    HashTable *ht;
    
    MALLOC(ht, sizeof(HashTable));
    ht->b = b;
    ht->s = s;

    MALLOC(ht->pairs, b*sizeof(element*));

    for(int i=0; i<b; i++) {
        MALLOC(ht->pairs[i], s*sizeof(element));
    }

    for(int i=0; i<b; i++) {
        for(int j=0; j<s; j++) {
            ht->pairs[i][j].key = -1;
        }
    }

    return ht;
}

void freeTable(HashTable *ht) {
    if(ht == NULL) return;

    for(int i=0; i<ht->b; i++) {
        free(ht->pairs[i]);
    }
    free(ht->pairs);
    free(ht);
}

int h(int k, int b) {
    return k % b;
}

// 由於 slot 由(index)小放到大，遇到 -1 後面肯定肯定沒東西?
// 還是之後 delete 不用往前面的 slot 搬? 重點是 clustering?
position search(HashTable *ht, int key) {
    int homeBucket, currentBucket;
    homeBucket = h(key, ht->b);

    position p;
    p.b = -1;
    p.s = -1;

    // linear probing every bucket
    for(currentBucket = homeBucket;;){
        // traverse slots of the current bucket
        for(int i=0; i<ht->s; i++) {         
            if(ht->pairs[currentBucket][i].key == key) {
                p.b = currentBucket;
                p.s = i;
                return p;
            }
        }
        currentBucket = (currentBucket+1) % ht->b;
        if(currentBucket == homeBucket) {
            break;
        }
    }
    return p;
}

// no duplicate keys
void insert(HashTable *ht, int key) {
    int homeBucket, currentBucket;
    homeBucket = h(key, ht->b);

    // linear probing every bucket
    for(currentBucket = homeBucket;;){

        // traverse slots of the current bucket
        for(int i=0; i<ht->s; i++) {
            // find an avaliable slot
            if(ht->pairs[currentBucket][i].key == -1) {
                ht->pairs[currentBucket][i].key = key;
                return;
            }
        }
        currentBucket = (currentBucket+1) % ht->b;
        if(currentBucket == homeBucket) {
            break;
        }
    }
}

void delete(HashTable *ht, int key) {
    int homeBucket, currentBucket;
    homeBucket = h(key, ht->b);

    // linear probing every bucket
    for(currentBucket = homeBucket;;){

        // traverse slots of the current bucket
        for(int i=0; i<ht->s; i++) {
            // find an avaliable slot
            if(ht->pairs[currentBucket][i].key == key) {
                ht->pairs[currentBucket][i].key = -1;
                return;
            }
        }
        currentBucket = (currentBucket+1) % ht->b;
        if(currentBucket == homeBucket) {
            break;
        }
    }
}

void printTable(HashTable *ht) {

    printf("    |");
    for(int i=0; i<ht->s; i++) {
        printf("%5d|", i);
    }
    printf("\n");

    printf("----|");
    for(int i=0; i<ht->s; i++) {
        printf("-----|");
    }
    printf("\n");

    for(int i=0; i<ht->b; i++) {
        printf("%3d |", i);
        for(int j=0; j<ht->s; j++) {
            printf("%5d|", ht->pairs[i][j].key);
        }
        printf("\n----|");
        for(int j=0; j<ht->s; j++){
            printf("-----|");
        }
        printf("\n");
    }
}