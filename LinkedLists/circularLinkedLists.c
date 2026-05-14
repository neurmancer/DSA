#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define NODE_SIZE sizeof(node)
#define SECOND 1000000 //Microseconds for usleep()
#define WIPE_SCREEN "\033[H\033[J" //Just an ANSI escape sequence to move to the home of terminal (1,1) and delete everything from there if I am not wrong you could use \033[2J

/*
    In this lecture we'll see circular linked lists where the case is litearlly a snake?sneake? sneak? which one was the animal...you know the serpant Reptilian creatures
    eating their own tail basically the nth node of the list will point root in it's nth->next instead of NULL which we've been doing so far so code and logic will be slightly changed 
    Let's figure shit out...
*/

/*

it's 1.52AM and I've been struggling with this for 1.5+ hours but finally I made it...without any exterior sources descriptions and shit will be added but for now...just

==163286== HEAP SUMMARY:
==163286==     in use at exit: 0 bytes in 0 blocks
==163286==   total heap usage: 11 allocs, 11 frees, 1,184 bytes allocated
==163286== 
==163286== All heap blocks were freed -- no leaks are possible
==163286== 
==163286== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

*/


struct n{

    int x; //Again an integer is all you need to open the gates of Data Structures rest is upon your imaginary and logical-thinking skills
    struct n *next;

};

typedef struct n node;

/*In this file all the functions will be rewritten/modified accordingly to suit cycling linked list structure*/

node *sortedAdd(node *r,int val);
node *delete(node *r,int target); //Yup in this lecture I am picking parameter names more carefully
int printTheList(node *r);


int main()
{   
    node *root = NULL; // always init your shit or segfault will turn into segassult
    
    root = sortedAdd(root,3);
    root = sortedAdd(root,5);
    root = sortedAdd(root,7);
    root = sortedAdd(root,7);
    root = sortedAdd(root,4);
    root = sortedAdd(root,42);
    root = sortedAdd(root,11);
    root = sortedAdd(root,-2);
    root = sortedAdd(root,INT_MAX);
    root = sortedAdd(root,INT_MIN);

    root = delete(root,11);
    root = delete(root,INT_MAX);
    root = delete(root,INT_MIN);
    

    printTheList(root);
    

    while (root != NULL) {
        root = delete(root,root->x);
    }


    return(0);
}


int printTheList(node *r)
{
    if (r == NULL) {
        printf("No nodes have found\n");
        return(1);
    }
    node *iter = r;

    while (iter->next != r)  //To check if we looped back to beginning or not since there is no NULL at the end (unless a NULL assigned by user which would break the all reasoning behind the cycling linked lists yk) 
    {
        printf("%d\n",iter->x);
        iter = iter->next;
    }
    //And the manual labor part
    printf("%d\n",iter->x);
    //That should print the ~~array~~ list(yeah old habbits) without a problem
    return(0);
}

node *sortedAdd(node *r,int val)
{
    if (r == NULL ) { 
        
        r = (node *) (malloc(NODE_SIZE));
        if (r == NULL) {printf("Malloc failed\n"); return(NULL);} 
        r->x = val;

        r->next = r;


        return(r);
    }

    if (r->x > val) { 
        
        node *temp = (node *)malloc(NODE_SIZE);
        node *iter = r;
        if (temp == NULL) {return(NULL);}
        while (iter->next != r) {
            iter = iter->next;
        }
        temp->x = val;
        iter->next = temp;
        temp->next = r;





        return(temp);
    }

    node *iter = r;
    while (iter->next != r && iter->next->x < val) { 
        iter = iter->next;
    }

    node *temp = (node *)malloc(NODE_SIZE);
    if (temp == NULL) {return(NULL);}
    temp->x = val;
    temp->next = iter->next;
    iter->next = temp;
    
    return(r);
}


node *delete(node *r,int target)
{
    if (r == NULL){return(NULL);}

    if (r->x == target) {
        if (r->next == r) {
            free(r);
            printf("There is no linked list nor any nodes anymore\n");
            return(NULL);
        }
        
        node *temp = r->next;
        node *iter = r;
        while (iter->next != r) {
            iter = iter->next;
        }
        iter->next = temp;
        free(r);
        
        return(temp);
    }

    node *iter = r;
    while (iter->next != r && iter->next->x != target) { 
        iter = iter->next;
    }

    if (iter->next->x == target) {//Bug's here...
        node *temp = iter->next;
        iter->next = temp->next;
        free(temp);



        return(r);
    }
    return(r);

}