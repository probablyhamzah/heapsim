#include <stdio.h>
#include <stdlib.h>

typedef struct nodeTag
{
    int loc;
    int size;
    void* mem;
    struct nodeTag* next;
} Node;

void* heap;
Node* freeList, * allocList;

void print(Node* list);
void printLists();
void init();
void cleanUp();
void* allocate(int req);
void coalesce();
void deallocate(void* mem);

