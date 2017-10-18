// Prac9_hash.c

 
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#define H_SIZE 10 /* the size of the h-table */
#define P_SIZE 12 /* the size of the page table */

struct page {
   int p_num; /* the page number */
   int frame; /* the corresponding frame address in the memory */
   struct page *next;
};

int collisionNumber = 0;

typedef struct page node;

node *htable[H_SIZE];  /* the heads of linked-lists in the h-table*/
node *tail[H_SIZE];  /* the corresponding tails */

void print_list(node*); /* print a linked-list */

void insq(int h, node *new);

void create_htable( ) 
{
	int p_address[P_SIZE] = {100,200,300,400,500,600,700,800,900,1000,1100,1200}; /* the list of physical addresses of pages*/
    int l_address[P_SIZE] = {1,4,3,2,5,10,7,8,9,6,12,11}; /* the list of logical addresses of pages */

    int i, h_value;
	node *p;
	
	/* initialize the h-table */
	for(i=0;i<=H_SIZE-1;i++) {
		htable[i] = NULL;
		tail[i] = NULL;
	}
	
	/* insert all pages to the h-table */
	for(i=0;i<=P_SIZE-1;i++){
		/* get new page new_page */
        p = (node *)malloc(sizeof(node));
        p -> p_num = l_address[i];
        p -> frame = p_address[i];
        p->next= NULL;
		
        h_value = l_address[i] % H_SIZE; /* hash function*/ 
        /* Alternate hash function */
       float A = (sqrt(5) - 1) / 2;
        h_value = (int)(H_SIZE * (A * l_address[i] - (int)(A * l_address[i]))); 
		insq(h_value, p); /* insert to the h-table */
	}
}



/*insert function for the ready queue */
void insq(int h, node *new)
{
     if (tail[h] == NULL ) {/* empty list */
        htable[h] = new;
     } else {
         tail[h]->next = new;
         collisionNumber++;
     }
     tail[h] = new;
}


void main() {
   int i;
   create_htable(); 
 
   /* print the hashed page table*/ 
   printf("The hash table is: \n");
   printf(" __ \n");
   for(i=0;i<=H_SIZE-1;i++) {
      printf("|__|--> ");
      print_list(htable[i]);
	}
	printf("The number of collisions in the Hash Table is %d\n", collisionNumber);
 }
 

 void print_list(node *p) {
   if (p == NULL)
      printf("^\n");
   else 
   {
        while(p->next) {
                 printf("[%d, %d] --> ", p -> p_num, p -> frame);
                 p = p -> next ;
        }
        printf("[%d, %d] ^\n", p->p_num, p->frame);
   }
}


