#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define NODE_SIZE sizeof(node)
#define SECOND 1000000 //Microseconds for usleep()
#define WIPE_SCREEN "\033[H\033[J" //Just an ANSI escape sequence to move to the home of terminal (1,1) and delete everything from there if I am not wrong you could use \033[2J


struct n {

    int x;
    struct n *next;
};

typedef struct n node;

int addData(node *r,int val)
{
    node *iter = r;

    while (iter->next != r) {
        iter = iter->next;
    }
    iter->next = (node *) malloc(NODE_SIZE);
    if (r->next == NULL) {printf("Allocation failed\n");return(-1);} //again just an habit 
    
    iter->next->x = val;
    iter->next->next = r; //Completing the cycle

    return(0);
}

node *addDataSequentialaly(node *r,int val) //and I forgot to mention why we're returning a node pointer as a value. Answer: because in edge cases we might change the address of the root to the temp and if we won't return the temp (now the actual root) it would be useless 
{
    if (r == NULL) { 
        //This block is basically an edge-case...if the function gets called for the first element in the linked list
        r = (node *) (malloc(NODE_SIZE));
        if (r == NULL) {printf("Malloc failed\n"); return(NULL);} //I am giving errnos accordingly to function so 1 for addData, 2 for main and NULL(since it's not returning an int and defining a struct for just delivering an error code and the pointer would be an overkill) for this one and the minus is basically sayign that some shit went sideways
        r->next = r; //That's litearlly a snake eating its own tail...(snake?Sneak? which one was the serpent one? IDK)
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


void printTheList(node *r)
{
    node *iter = r;
    printf("%d\n",iter->x);
    iter = iter->next;
    while (iter->next != r) {
        printf("%d\n",iter->x);
        iter = iter->next;  
    }
}

node *delete(node *r,int target)
{
    node *iter = r;
    
    if (r->x == target) {
        while (iter->next != r) {
            iter = iter->next;
        }
        iter->next = r->next;
        r = r->next;
        free(r); 
        return(iter->next);
    }

    while (iter->next != r && iter->next->x != target) {
        iter = iter->next; 
    }

    if (iter->next == r) {
        //You can add a printf("Error:404"); lol
        return(r);
    }
    node *temp = iter->next;
    iter->next =iter->next->next; 

    return(r); 
}



int main(void)
{
    node *root = malloc(NODE_SIZE);
    if (root == NULL) {return(-1);}
    root->x = 13;
    root->next = root; //Yeah...when I don't init valgrind starts filing for divorce 

    printTheList(root);


    for (int i = 1; i < 21;i+=3) {
        root = addDataSequentialaly(root,i%9);    
    }

    printTheList(root);

    root = delete(root,1);
    root = delete(root,1);
    root = delete(root,1);
    root = delete(root,4);
    root = delete(root,4);
    root = delete(root,7);
    root = delete(root,7);
    


    /*
        To test from now on I'll run the compiled bin with valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./a
        from now on in lectures

    */

    return(0);
}


/*
    Yeat again once again a broken code commit ... there is 1 less free which means there is a little leakage but worse is segfault which I will force me to re-write entire lecture
    since me not being able to find it with even valgrind means I couldn't understand what I am doing...but this will stay here


    ==87234== HEAP SUMMARY:
==87234==     in use at exit: 16 bytes in 1 blocks
==87234==   total heap usage: 9 allocs, 8 frees, 1,152 bytes allocated
==87234== 
==87234== Searching for pointers to 1 not-freed blocks
==87234== Checked 84,448 bytes
==87234== 
==87234== 16 bytes in 1 blocks are definitely lost in loss record 1 of 1
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400126E: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== LEAK SUMMARY:
==87234==    definitely lost: 16 bytes in 1 blocks
==87234==    indirectly lost: 0 bytes in 0 blocks
==87234==      possibly lost: 0 bytes in 0 blocks
==87234==    still reachable: 0 bytes in 0 blocks
==87234==         suppressed: 0 bytes in 0 blocks
==87234== 
==87234== ERROR SUMMARY: 32 errors from 32 contexts (suppressed: 0 from 0)
==87234== 
==87234== 1 errors in context 1 of 32:
==87234== Invalid read of size 8
==87234==    at 0x400142B: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3048 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x40014A5: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 2 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001413: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3578 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 3 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001403: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3578 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 4 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013E9: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3578 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 5 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013F5: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3578 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 6 of 32:
==87234== Invalid read of size 4
==87234==    at 0x40013DC: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015C3: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3570 is 0 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 7 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001413: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3668 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 8 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001403: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3668 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 9 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013E9: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3668 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 10 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013F5: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3668 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 11 of 32:
==87234== Invalid read of size 4
==87234==    at 0x40013DC: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x40015AE: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3660 is 0 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 12 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001413: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3528 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 13 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001403: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3528 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 14 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013E9: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3528 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 15 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013F5: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3528 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 16 of 32:
==87234== Invalid read of size 4
==87234==    at 0x40013DC: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001599: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3520 is 0 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 17 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001413: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3618 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 18 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001403: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3618 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 19 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013E9: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3618 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 20 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013F5: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3618 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 21 of 32:
==87234== Invalid read of size 4
==87234==    at 0x40013DC: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001584: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa3610 is 0 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 22 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001413: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa35c8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 23 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001403: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa35c8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 24 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013E9: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa35c8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 25 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013F5: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa35c8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 26 of 32:
==87234== Invalid read of size 4
==87234==    at 0x40013DC: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400156F: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa35c0 is 0 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 27 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001413: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa36b8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001545: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 28 of 32:
==87234== Invalid read of size 8
==87234==    at 0x4001403: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa36b8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001545: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 29 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013E9: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa36b8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001545: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 30 of 32:
==87234== Invalid read of size 8
==87234==    at 0x40013F5: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa36b8 is 8 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001545: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== 
==87234== 1 errors in context 31 of 32:
==87234== Invalid read of size 4
==87234==    at 0x40013DC: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400155A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Address 0x4aa36b0 is 0 bytes inside a block of size 16 free'd
==87234==    at 0x48529EF: free (vg_replace_malloc.c:989)
==87234==    by 0x4001426: delete (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x4001545: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==  Block was alloc'd at
==87234==    at 0x484F8A8: malloc (vg_replace_malloc.c:446)
==87234==    by 0x400130B: addDataSequentialaly (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234==    by 0x400151A: main (in /home/neuromancer/realHome/C/Projects/DSA/LinkedLists/a.out)
==87234== 
==87234== ERROR SUMMARY: 32 errors from 32 contexts (suppressed: 0 from 0)

*/