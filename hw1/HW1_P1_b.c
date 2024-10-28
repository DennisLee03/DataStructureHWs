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
    int top;
    int size;
    char *elements;
} Stack;

Stack *create(int size);
bool isEmpty(Stack *stack);
bool isFull(Stack *stack);
void push(Stack *stack, char operator);
char pop(Stack *stack);
char top(Stack *stack);
void freeStack(Stack *stack);

void infix_to_postfix(char *infix, char *postfix);
bool isOperator(char token);
int precedence(char token);

int main() {

    char infix[MAX_EXPR_SIZE];
    char postfix[MAX_EXPR_SIZE];

    if(fgets(infix, sizeof(infix), stdin)) {
        int idx = strcspn(infix, "\n");
        int len = strlen(infix);
        if(idx != len) {
            infix[idx] = 0;
        }

        // here to start
        infix_to_postfix(infix, postfix);
        printf("%s", postfix);
    } else {
        printf("Input ERROR!\n");
        return 1;
    }

    return 0;
}

Stack *create(int size) {
    Stack *stack;

    MALLOC(stack, sizeof(Stack));
    MALLOC(stack->elements, (size+1)*sizeof(char));

    for(int i = 0; i <= size; i++) {
        stack->elements[i] = 0;
    }

    stack->top = -1;
    stack->size = size;

    return stack;
}

bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

char pop(Stack *stack) {
    if(isEmpty(stack)) {
        printf("Stack is EMPTY!\n");
        exit(1);
    }
    char deleted = stack->elements[stack->top];
    stack->elements[stack->top] = 0;
    stack->top--;
    return deleted;
}

bool isFull(Stack *stack) {
    return stack->top == stack->size-1;
}

void push(Stack *stack, char operator) {
    if(isFull(stack)) {
        printf("Stack is FULL!\n");
        return;
    }
    stack->top++;
    stack->elements[stack->top] = operator;
}

char top(Stack *stack) {
    if(isEmpty(stack)) {
        printf("Stack is EMPTY!\n");
        exit(1);
    }
    return stack->elements[stack->top];
}
/*
 * input: simple infix expression without parentheses and contains only single-char variables, operators +/-*
 * output: postfix expression
 */
void infix_to_postfix(char *infix, char *postfix) {
    strcpy(postfix, "");
    int len = strlen(infix);
    Stack *stack;
    stack = create(len + 1);

    // left-to-right scan of infix expression
    for(int i = 0; i < len; i++) {
        char token = infix[i];
        // encounter operator: check its precedence to pop or push
        if(isOperator(token)) {
            // if it is an empty stack: push
            if(isEmpty(stack)) {
                push(stack, token);
                continue;
            }

            // precendence of top element < incoming operator: push incoming operator
            if(precedence(top(stack)) < precedence(token)) {
                push(stack, token);
            }

            // greater than or equal: pop and then push incoming operator
            else {
                // continue to compare until the stack is empty or we can pop because of precedence
                while(!isEmpty(stack) && precedence(top(stack)) >= precedence(token)) {
                    char buf = pop(stack);
                    strncat(postfix, &buf, 1);
                }

                push(stack, token); 
            }

        }
        // encounter operand: add to postfix
        else {
            strncat(postfix, &token, 1);
        }
    }

    while(!isEmpty(stack)) {
        char tmp = pop(stack);
        strncat(postfix, &tmp, 1);
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

int precedence(char token) {
    int p ;

    switch (token) {
        case '+': case '-': return 0;
        case '*': case '/': return 1;
        default: break;
    }

    return 0;
}

void freeStack(Stack *stack) {
    free(stack->elements);
    free(stack);
}