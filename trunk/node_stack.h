/**********************************************************
node_stack.h

@author Alex Tough (07-41604)
@author Daniel Bruzual ()

Implements stack for implication graph nodes.
************************************************************/
#include <stdio.h>
#include "data_structures.h"
#define STACK_SIZE 100001

// Last position of the stack must never be ocuppied.
node *base,*top, stack[STACK_SIZE];

// Initializes base to point to the bottom of the stack,
// and top to point to the first free element of the stack.
void initialize_stack(){
    base = stack;
    top = stack;
}

void push(node x){
    if(top == (base + STACK_SIZE)){
	printf("Stack Overflow.\n");
	exit(1);
    }
    *top =  x;
    top += sizeof(node); 
}

node pop(){
    node t;
    if(top == base){
	printf("Stack Underflow.\n");
	exit(1);
    }
    top -= sizeof(node);
    t = *top;
    top = NULL;
    return t;
}


