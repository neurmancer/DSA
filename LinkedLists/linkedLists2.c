#include <stdio.h>
#include <stdlib.h>

/*
    This file will be about iterating over linked lists since linked lists are sequential access points unlike arrays 
    (which is random acces Which means you can say I want 4th element and arr[3] will give it to you but in linked list each nodes only knows the next node's address)
    So in this file we'll see walking over a linked list
*/



struct n{
    int x; //If you can hold an int you can basically hold anything so that's the core
    struct n *next;

};

typedef struct n node; // making my job easier with names and defines

void printTheList(node *r); //Function to print every node'x value in a given linked list 

int addData(node *r,int x); //Adding another node to the end of linked list with assiging x to the 'data' section of node (which is also x in my case...yeah I've chosen the names poorly srry)

int main(void)
{


    node *root = NULL;  //Null init for avoiding possible UB
    root = (node *) malloc(sizeof(node));
    if (root == NULL) {
        printf("Something's wrong memory couldn't allocated");
        return(-1);
        /*Again this if block is not necessary but it's an habit of mine to code defensive even in lectures 
        As you can see at the beginning NULL initing root instead of leaving it pointing a garbage data in RAM*/
    }
//    root->x = 13; //root value assigning manually (tho I wouldn't advise touching root itself this much )
//    root->next = NULL;
//  Insted we can do 
    node *iter = root;
    iter->x = 13;
    iter->next = NULL;


    for (int i = 0;i < 5;i++) 
    {
        if(addData(root,i*5)) {return(-1);} //Creating 5 iterations and adding value to each of them one by one via the function
    /*   
        For this comment block...That was the first way I've done the assigning in main instead of the addData function and it's faster since it 
        adds over iter->next and next so on where addData circles back to root for each add and seek for the NULL pointer.
        But this is not a file about optimization nor O(n) (tho I don't even know what big O notation is for now) but if It's something like I think
        the nation will be something like O((n*n1)/2) for the addData function  
    

        iter->next = (node *) malloc(sizeof(node));
        if (iter->next == NULL) {
            printf("Shit went sideways with malloc\n"); //again...this is just me being me about 'never trust a computer to compute -Sun Tzu (or Linus Torvalds IDK)
            return(-1);
        }

        iter = iter->next;
        iter->next = NULL;
    */

    }

    printTheList(root);

    return(0);
}

void printTheList(node *r)
{

    while (r->next != NULL) {
        printf("%d\n",r->x); 
        r = r->next; //since this line for the final iter where r->next = NULL won't gonna work I added the line for manual printing the last line 
    }
    printf("%d\n",r->x);
}

int addData(node *r,int x)
{
    while (r->next != NULL) {
        r = r->next;
    }
    r->next = (node *) (malloc(sizeof(node)));
    if (r->next == NULL) {
        printf("Allocation for new node failed\n");
        return(-1);
    }
    r->next->x = x;
    r->next->next = NULL; //New end point

    return(0);
}