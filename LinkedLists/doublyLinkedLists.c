#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*In this lecture we'll see doubly linked lists (yeah I said it's bidirectional but turns out appropriate term is doubly)...(tho in my dream it'll remain as bidirectional or bisexual for the lulz)*/

#define NODE_SIZE sizeof(node)

struct n{

    int x;
    struct n *next; //Next element 
    struct n *prev; // Previous element


};

typedef struct n node;

int printTheList(node *r);
node *sortedAdd(node *r,int val);


int main(void)
{







    return(0);
}


int printTheList(node *r)
{
    if (r == NULL) {return(-1);}

    while (r->next != NULL) {
        printf("%d\n",r->x);
        r = r->next;
    }
    printf("%d\n",r->next->x);
    return(0);
}   


node *sortedAdd(node *r,int val)
{
    if (r == NULL) {
        r = (node *) malloc(NODE_SIZE);
        if (r == NULL) {return(NULL);}
        r->x = val;
        
        r->next = NULL;
        r->prev = NULL; //New shiny variable in the node struct
        
        return(r);
    }

    if (r->x > val) {
        node *temp = (node *) malloc(NODE_SIZE);
        if (temp == NULL) {return(NULL);}

        temp->x = val;
        r->prev = temp;
        
        temp->next = r;
        temp->prev = NULL;

        return(temp);
    }
    node *iter = r;
    while (iter != NULL && val < iter->x) {
        iter = iter->next;
    }



}
