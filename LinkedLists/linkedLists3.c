#include <stdio.h>
#include <stdlib.h>

#define SIZE sizeof(node) //I don't wanna overtype the samething again and again and that will reduce confusion imo (unless we calculate another size than I might delete or name it differently later)

/*
            This file is about adding a new node in between two nodes instead of adding it to the end of the linked list
            And I am just as curious as you are how tf I'll achieve it

            Well...Hi from future (just a day future from and if you wonder the old version of the file I recommend you to check the last commit on this file) 
            coding this or at least trying to comprehend taught me something I haven't really tought about beforehand...coding or programming to be precise is just probabilty 
            but not the scary kind more like a highschool lecture one... you got a few (sometimes so much) possibilities and you gotta suit your expectations (therefore your code) to match with it
            in this file the core idea was squeezing nodes in the middle but in the end I managed to create a function that adds sequentially (lower first higher later kind of list) 
            and all I did was thinking about what I have and what I need to do accordingly 

            First -> I managed to visualize the nodes and how to squeeze one in the middle
            Second -> What if there is no data in the first? and we're adding to it
            Third-> What if the new data is smaller than the actual root
            Fourth -> What if new data is bigger than the root but and possibly a few more nodes but not all of them? and that calculations or wonder led me to the function I made


*/


struct n {

    int x;
    struct n *next;
};

typedef struct n node;
int addData(node *r,int val); //or struct n *r but since I am using typedef that's unnecessary but make sure you declared the typedef before the function since otherwise it pisses itself
node *addDataSequentilaly(node *r,int val);
void printTheList(node *r);


int main(void)
{

    node *root = malloc(SIZE);
    if (root == NULL) {return(-2);}

    root->next = NULL;

    root = addDataSequentilaly(root,13);
    root = addDataSequentilaly(root,53);
    root = addDataSequentilaly(root,23);
    root = addDataSequentilaly(root,113);
    root = addDataSequentilaly(root,689);
    root = addDataSequentilaly(root,45);
    root = addDataSequentilaly(root,26);
    root = addDataSequentilaly(root,4);
    root = addDataSequentilaly(root,12);
    root = addDataSequentilaly(root,13); //Yup it's reoccuring so we can check if it trips at reoccurence 
    root = addDataSequentilaly(root,13);

    printTheList(root);


/*
    root->x = 500;

    int i = 0;
    for (i = 0;i<5;i++) {
        addData(root,i*10);
    }

    printTheList(root);
    node *iter = root;

    for (i = 0;i<3;i++) {
        iter = iter->next; //moves iter 3 times
    }

    node *temp = (node *) malloc(sizeof(node));
    if (temp == NULL) { return(-2);}

    temp->next = iter->next; //gives you the node after iter and assigns it as temp's next as well 
    iter->next = temp; //and this removes the link between original sequence and squeeze the temp in the middle of iter and iter->next so the old iter next is now temp->next and iter->next is temp itself
    temp->x = 100;

    printTheList(root);

    My testing era before coding the addDataSequenctially and just realized while adding those comments I fucked up the spelling but yk it's me being me...


*/


    return(0);
}



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

node *addDataSequentilaly(node *r,int val)
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
        r = r->next;  //Lol just forgot adding that line and got an infinite loop (yeah I am not copy-pasting from old file...that's how you achieve muscle memory)
    }
    printf("%d\n",r->x); //That's the manual labor part but it's still way better than iter->next->next->next->next->next->next->next->next->next->x
}
