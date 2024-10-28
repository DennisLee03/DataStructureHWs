#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_EXPR_SIZE 101
#define OPERATORS "+-*/"
#define MALLOC(p,s) \
    if(!(p = malloc(s))) {\
        fprintf(stderr, "Insufficeint memory.");\
        exit(EXIT_FAILURE);\
    }

typedef struct {
    char *str;
} element;

typedef struct {
    element *items;
    int top;
    int size;
} Stack;

bool isOperator(char token);
void copyElement(const char *src, element *dst);

Stack *create(int size);
bool isFull(Stack *stack);
bool isEmpty(Stack *stack);
void push(Stack *stack, const char *str);
element pop(Stack *stack);
void freeStack(Stack *stack);

void prefix_to_infix(char *prefix, char *infix);

int main(void) {

    char prefix[MAX_EXPR_SIZE];
    char infix[MAX_EXPR_SIZE];
    if(fgets(prefix, sizeof(prefix), stdin)) {

        int idx = strcspn(prefix, "\n");
        int len = strlen(prefix);
        if(idx != len) {
            // remove '\n'
            prefix[idx] = 0;
        }

        prefix_to_infix(prefix, infix);
        printf("%s", infix);
    } else {
        printf("Input ERROR!\n");
        return 1;
    }

    return 0;
}

Stack *create(int size) {
    Stack *stack;

    MALLOC(stack, sizeof(Stack));
    MALLOC(stack->items, size*sizeof(element));

    stack->top = -1;
    stack->size = size;

    return stack;
}

bool isFull(Stack *stack) {
    return stack->top == stack->size - 1;
}

void push(Stack *stack, const char *str) {
    if(isFull(stack)) {
        printf("Stack is FULL!\n");
        return;
    }
    stack->top++;
    MALLOC(stack->items[stack->top].str, sizeof(char)*(strlen(str)+1));
    strcpy(stack->items[stack->top].str, str);
}

bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

element pop(Stack *stack) {
    if(isEmpty(stack)) {
        printf("Stack is empty\n");
        exit(EXIT_FAILURE);
    }

    element deleted;
    copyElement(stack->items[stack->top].str, &deleted);
    free(stack->items[stack->top].str);
    stack->top--;

    return deleted;
}

void copyElement(const char *src, element *dst) {
    MALLOC(dst->str, sizeof(char)*(strlen(src)+1));
    strcpy(dst->str, src);
}

/*
 * input: an prefix expression without parenthesis, space (最長可以到100個字元)
 *        operands: 單一字元作為一個個 operands
 *        operators: 只有 +, -, *, / 四種可能
 * 
 * output: an infix expression without parenthesis
 *         不用在意運算優先度，掃到 operators 就會先結合出一個子表達式而且不加任何括號
 */
void prefix_to_infix(char *prefix, char *infix) {
    Stack *stack;
    stack = create(MAX_EXPR_SIZE);
    int len = strlen(prefix);

    // 掃描 prefix
    for(int i = len-1; i >= 0; i--) {
        char token = prefix[i];
        if(isOperator(token)) {
            // 當遇到 operator 時，就 pop 出兩個 operand
            element op1 = pop(stack), op2 = pop(stack);
            char tmp[MAX_EXPR_SIZE];

            // 並將 operands 跟 operator 結合成新的複合 operand 然後放回 stack 中
            snprintf(tmp, sizeof(tmp), "%s%c%s", op1.str, token, op2.str);
            push(stack, tmp);
            free(op1.str);
            free(op2.str);
        }
        else {
            // 每每遇到 operand 都將其放入 stack 中
            char operand[2] = {token, '\0'};
            push(stack, operand);
        }
    }

    // stack 的 top 已經有處理好的 expression 了，接著取出它
    if(!isEmpty(stack)) {
        element result = pop(stack);
        strcpy(infix, result.str);
        free(result.str);
    }

    // free stack
    freeStack(stack);
}

bool isOperator(char token) {
    for(int i = 0; i < 4; i++) {
        if(token == OPERATORS[i]) {
            return true;
        }
    }
    return false;
}

void freeStack(Stack *stack) {
    free(stack->items);
    free(stack); 
}