#include "heapsim.h"

void print(Node* list)
{
    Node* ptr = list;
    printf("Location\tBlock size\tData\n");
    while(ptr != NULL)
    {
        printf("%d\t\t\t%d\n", ptr->loc, ptr->size);
        Node* curr = ptr;
        ptr = ptr->next;
    }
    printf("\n");
}

void printLists()
{
    printf("\n\n");
    printf("Free list\n");
    print(freeList);
    printf("Allocated list\n");
    print(allocList);

}

void init()
{
    heap = malloc(1024);
    
    freeList = (Node*) malloc(sizeof(Node)), allocList = NULL;
    freeList->next = NULL;
    freeList->loc = 0;
    freeList->size = 1024;    
}

void cleanUp()
{
    free(heap);

    Node* ptr = freeList;
    while(ptr != NULL)
    {
        Node* temp = ptr;
        ptr = ptr->next;
        free(temp);
    }

    ptr = allocList;
    while(ptr != NULL)
    {
        Node* temp = ptr;
        ptr = ptr->next;
        free(temp);
    }

}

void* allocate(int req)
{
    Node* ptr = freeList, * prev = NULL;
    while(ptr != NULL && req > ptr->size)
    {
        prev = ptr;
        ptr = ptr->next;
    }


    if(ptr->size > req)
    {
        Node* nptr = (Node*) malloc(sizeof(Node));
        nptr->next = NULL;
        nptr->size = req;
        ptr->size -= req;
        
        if(prev == NULL)
        {
            nptr->loc = ptr->loc;
            nptr->next = freeList;
            ptr->loc = nptr->loc + req;
            freeList = nptr;
        }
        else
        {            
            prev->next = nptr;
            nptr->next = ptr;            
            nptr->loc = ptr->loc;
            ptr->loc = ptr->loc + req;
        }
        ptr = nptr;
    }


    
    if(ptr == freeList)
        freeList = ptr->next;
    else
        prev->next = ptr->next;
    
    ptr->next = NULL;
    
    if(allocList == NULL)
        allocList = ptr;
    else
    {
        Node* temp = allocList;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = ptr;
    }

    ptr->mem = heap + ptr->loc;
    return ptr->mem;
}


void coalesce()
{
    Node* ptr = freeList, * prev = NULL;

    while(ptr->next != NULL)
    {
        if(ptr->loc + ptr->size == ptr->next->loc)
        {
            ptr->size += ptr->next->size;
            Node* temp = ptr->next;
            ptr->next = ptr->next->next;
            free(temp);
        }
        else
        {
            prev = ptr;
            ptr = ptr->next;
        }

        // handles the edge case where the current node has the same size as the previous node
        if(prev != NULL &&  prev->loc + prev->size == ptr->loc)
        {
            prev->size += ptr->size;
            Node* temp = ptr;
            prev->next = ptr->next;
            free(temp);
        }
        
    }
}

void deallocate(void* mem)
{
    Node* ptr1 = allocList, * prev1 = NULL;

    while(ptr1 != NULL && ptr1->mem != mem)
    {
        prev1 = ptr1;
        ptr1 = ptr1->next;
    }

    Node* ptr2 = freeList, * prev2 = NULL;
    while(ptr2 != NULL && ptr2->loc < ptr1->loc)
    {
        prev2 = ptr2;
        ptr2 = ptr2->next;
    }


    if(ptr1 == allocList)
    {
        allocList = (allocList)->next;
    }
    else
    {
        prev1->next = ptr1->next;
    }

    
    if(ptr2 == freeList)
    {
        ptr1->next = freeList;
        freeList = ptr1;
    }
    else
    {
        prev2->next = ptr1;
        ptr1->next = ptr2;
    }

    coalesce();

}

int main()
{
    init();

    // Practical use in a program
    /*
    int* a = (int*) allocate(sizeof(int));
    *a = 10;
    printf("%d\n\n", *a);

    int* b = (int*) allocate(sizeof(int) * 3); // array of 3 elements

    for(int i = 0; i < 3; i++)
    {
        b[i] = i;
        printf("%d\n", b[i]);

    }

    printLists();
    
    deallocate(a);
    deallocate(b);

    printLists();
    */

    // Menu driven program
    
    void* vars[20];
    int size = 0;

    int choice;
    do
    {
        printf("\nEnter your choice\n");
        printf("1. Allocate\n2. Deallocate\n3. Print free and allocated lists\n4. Quit\n");
        scanf("%d", &choice);
        
        if(choice == 1)
        {
            int amount;
            printf("Enter amount to be allocated\n");
            scanf("%d", &amount);

            vars[size] = allocate(amount);
            printf("It has been allocated at index %d\n", size);
            ++size;
        }
        else if(choice == 2)
        {
            int index;
            printf("Enter index of var to be deallocated\n");
            scanf("%d", &index);
            deallocate(vars[index]);
        }
        else if(choice == 3)
        {
            printLists();
        }
    } while(choice != 4);
    

    cleanUp();
}
