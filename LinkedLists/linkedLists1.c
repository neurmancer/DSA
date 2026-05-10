/*
    These  Code blocks at the top will be almost in every .c file in this repo to breifly explain the concept before folding our sleeves to code
    And yeah this repo will have a more serious tone than others since this is something I've been afraid of for a long long time > /// <

    The idea of linked lists(as far as I understood): is a data unit sequence holding 2 sections. First:Data(obvious duh...)
    (basicall the place will hold the data let's say an int to make it easier to grasp onto)
    and second: a pointer pointing the next node (which is the all the data units in linked lists called nodes) 
    or if it's the last node it points NULL waiting for next node to get created. 

    Each linked list has root...and if you can't reach to root(first node) you're fucked and just lost the linked list in ASLR RAM universe 
*/


#include <stdio.h> // for I/O
#include <stdlib.h> //For malloc and free()


struct n{
    int x;
    struct n *next;
};

typedef struct n node;

int main()
{
    node *root = NULL;
    node *iter = NULL; //Will be used for not to loose the root node 
    root = (node *) malloc(sizeof(node)); //malloc returns void * and I just cast (type *) as a muscle memory you don't have to as far as I know.
    if (root == NULL) {
        printf("Allocation failed\n");
        return(-1);
    }
    iter = root;

    for (int i = 1; i < 3;i++)
    {

        iter->x = i;
        printf("%d\n",i);
        iter->next =(node *)malloc(sizeof(node));
        iter = iter->next;
    }


    //This is an experiment to validate my thoughts about linked list not being sequencial like normal arrays I expect to see randomized memory addresses

    iter = root;
    printf("%zu\n",sizeof(node));
    for (int i = 1; i < 3;i++)
    {
        printf("%p\t%d\n",(void *) iter->next,iter->x); //Still...using casts because %p takes (void *) and iter->next is (node *)
        iter = iter->next;
    } //Just ran the code and they were 32 bytes apart for all runs...4 bytes padding maybe?
    //0x55ab996d3440  They seem sequential but I don't think so since malloc just doesn't care about sequences 
    //0x55ab996d3460
    iter = root;
    free(iter->next->next);
    free(iter->next); //This code will leak into memory probably since I couldn't figure out how to iter over nodes and free witohut freeing iter first 
    free(iter);


    //anways that was the first lecture if I made some mistakes I'll keep the file as it is and correct it in the next file so I can see what was the problem in the first place
    


    return(0);
}