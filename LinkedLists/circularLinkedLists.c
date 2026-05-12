#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define NODE_SIZE sizeof(node) // this is better than just SIZE ig

/*
    First disclamier: Don't fucking expect me to spell circular or cycling linked lists correct every time cuz we both know I can't

    Yup messed it up again and shipping like this (even without working) before resting for a bit these 30 lines of code took my 1 hour 
    and I am exhausted :/


*/


struct n{
    
    int x; //Same as before 
    struct n *next;
};


typedef struct n node;

void printTheListForCycling(node *r);
void printTheList(node *r); //btw I never said it out I guess but r stands for root 

node *addSequentially(node *r,int val); //and fixed the root->x = x conflict with val lol...took me long enough
node *delete(node *r,int val);



int main(void)
{

    node *root = (node *)malloc(NODE_SIZE);
    root = NULL;

    root = addSequentially(root, 53);
    root = addSequentially(root, 13);
    root = addSequentially(root, 9);
    root = addSequentially(root, 7);
    root = addSequentially(root, 7);
    root = addSequentially(root, 35);
    


    printTheListForCycling(root);




    return(0);
}



void printTheList(node *r)
{
    if (r == NULL) {
        printf("List's empty bruh\n");
    }
    while (r->next != NULL) {
        printf("%d\n",r->x);
        r = r->next;
    }
    printf("%d\n",r->x);
}

void printTheListForCycling(node *r)
{
    node *iter = r;

    do {
        printf("%d\n",iter->x);
        iter = iter->next;
    }while (iter != r);


}

node *addSequentially(node *r,int val)
{
    node *iter = r;

    if (r == NULL) { 
        r = (node *) (malloc(NODE_SIZE));
        if (r == NULL) {printf("Malloc fucked up\n"); return(NULL);} 
        r->next = r;
        r->x = val;

        return(r);
    }

    if (r->x > val) {
        node *temp = (node *)malloc(NODE_SIZE);
        if (temp == NULL) {return(NULL);}
        temp->x = val;
        temp->next = r;
        node *iter = r;
        
        while (iter->next != r) {
            iter = iter->next;
        }
        iter->next = temp;

        return(temp);
    }

    while (iter->next != r && iter->next->x < val) {
        iter = iter->next;
    }

    node *temp = (node *)malloc(NODE_SIZE);
    if (temp == NULL) {return(NULL);}
    temp->x = val;
    temp->next = iter->next;
    iter->next = temp;
    


}

node *delete(node *r,int val)
{

    if(r == NULL)
    {
        return(NULL);
    }

    node *temp = NULL;

    if (r->x == val)
    {
        temp = r;
        r = r->next;
    }

    node *iter = r;
    while (iter->next != r && iter->next->x != val) {
        iter = iter->next;
    }

    if (iter->next == r) {
        return(r);
    }

    temp = iter->next;
    iter->next = temp->next;
    
    free(temp);
    return(r);
}