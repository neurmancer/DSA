#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*

    This lecture is based on lecture 3 and I am keeping some of old comments still (I did copy-pasted for faster progress)
    This lecture is about deleting a node from linked list and I am assuming that's gonan be a little cheesy since as I said 
    when you lost a node link the rest of the linked list becomes unreachable since it's sequential so if you loose root->next before doing 
    root->next = root->next->next (basically skipping and loosing the root->next node forever unless you have a backup) you are basically fucked
    
    And I hope this lecture will be my gateway to learn how to free the linked list before christening all the memory with garbage memory 
*/

#define SIZE sizeof(node)
#define SECOND 1000000 //Microseconds for usleep()
#define WIPE_SCREEN "\033[H\033[J" //Just an ANSI escape sequence to move to the home of terminal (1,1) and delete everything from there if I am not wrong you could use \033[2J


struct n {

    int x;
    struct n *next;
};

typedef struct n node;

int addData(node *r,int val)
{
    while (r->next != NULL) {
        r = r->next;
    }
    r->next = (node *) malloc(SIZE);
    if (r->next == NULL) {printf("Allocation failed\n");return(-1);} //again just an habit 
    
    r->next->x = val;
    r->next->next = NULL; //Prevents us from getting segfaults / dangling pointers

    return(0);
}

node *addDataSequentialaly(node *r,int val) //and I forgot to mention why we're returning a node pointer as a value. Answer: because in edge cases we might change the address of the root to the temp and if we won't return the temp (now the actual root) it would be useless 
{
    if (r == NULL) { 
        //This block is basically an edge-case...if the function gets called for the first element in the linked list
        r = (node *) (malloc(SIZE));
        if (r == NULL) {printf("Malloc failed\n"); return(NULL);} //I am giving errnos accordingly to function so 1 for addData, 2 for main and NULL(since it's not returning an int and defining a struct for just delivering an error code and the pointer would be an overkill) for this one and the minus is basically sayign that some shit went sideways
        r->next = NULL;
        r->x = val;

        return(r);
    }

    if (r->x > val) { //Second edge case : what if value we've provided to the func is smaller than the root?
        node *temp = (node *)malloc(SIZE);
        if (temp == NULL) {return(NULL);}
        temp->x = val;
        temp->next = r;
        
        return(temp);
    }

    node *iter = r;
    while (iter->next != NULL && iter->next->x < val) { //Note I used iter instead of iter-> next and got core dump since it was checking the current node instead of the next one and hence I tried to use a dangling pointer :/ took 15 seconds to figure it out
        iter = iter->next;
        //This while block goes on until either one of those happens first did it reach to end of the list (well...iter->next reaches there iter stays one behind so won't overshoot the place we wanna be)
        //or Second after checking if the root smaller than the value or not we're basically going to a trip to find the value bigger than value itself it may not occur and that means the new node gets to be added to the tail of the list 

    }

    node *temp = (node *)malloc(SIZE);
    if (temp == NULL) {return(NULL);}
    temp->x = val;
    temp->next = iter->next;
    iter->next = temp;
    
    return(r);
}


void printTheList(node *r)
{
    while (r->next != NULL) {
        printf("%d\n",r->x);
        r = r->next;  //Lol just forgot adding that line and got an infinite loop (yeah I am now copy-pasting from old file...that's how you achieve muscle memory)
    }
    printf("%d\n",r->x); //That's the manual labor part but it's still way better than iter->next->next->next->next->next->next->next->next->next->x
}

node *delete(node *r,int target)
{
    node *temp = NULL;
    node *iter = r;
    
    if (r->x == target) { //Edge case of target being the first node in the linked list so we hold the address in temp and assign root to the second element before freeign the temp(old root)
        temp = r;
        r = r->next;
        return(r);
    }

    while (iter->next != NULL && iter->next->x != target) {
        //pretty self-explanatory but in plain english: while the next node after iter is not null and not the target move on
        iter = iter->next; 
    }

    if (iter->next == NULL) {
        //You may add printf("Not in the list already\n") if you want
        return(r);
    }
    temp = iter->next;
    iter->next =iter->next->next; //or temp->next
    free(temp);
    return(r); 
}



int main(void)
{
    
    node *root = (node *) malloc(SIZE);
    if (root == NULL) {return(-1);}

    for (int i = 0;i < 10;i++) {
        root = addDataSequentialaly(root, i); //Still updating root so if we change first element we notice 
    }

    printTheList(root);
    for (int i = 5;i < 9;i++) {
        delete(root,i);
    }
    
    printTheList(root);
    usleep(SECOND*3);
    printf(WIPE_SCREEN);

    addData(root, 13);
    addData(root, 689);
    addData(root, 53);
    addData(root, 15);
    addData(root, 15);
    addData(root, 15);
    addData(root, 7);
    addData(root,701);

    printTheList(root);
    usleep(SECOND*3);
    printf(WIPE_SCREEN);

    root = delete(root,7); //Printing again and again to test different cases (first element, last,middle, non-existing etc.)
    printTheList(root);
    usleep(3*SECOND);
    printf(WIPE_SCREEN);

    root = delete(root,0);
    root = delete(root,0);
    printTheList(root);
    usleep(3*SECOND);
    printf(WIPE_SCREEN);

    root = delete(root,700);
    printTheList(root);
    usleep(3*SECOND);
    printf(WIPE_SCREEN);

    root = delete(root,15); //I assume it's gonna delete only one '15's since I programmed it to stop when it first encounters with the targetted value so rest 15s will be fine
    printTheList(root);
    usleep(3*SECOND);
    printf(WIPE_SCREEN);


    root = delete(root,701);
    printTheList(root);
    usleep(3*SECOND);
    printf(WIPE_SCREEN);

    while (root->next != NULL) {
        root = delete(root,root->next->x);
    }
    printTheList(root);
    free(root); //Yup with that I presume I cleared all the fucked up memory from the lecture 1 

    return(0);
}