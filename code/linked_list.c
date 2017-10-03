#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"

const int DEBUG = 0;

void initList(LL* L) {
	L->head = NULL;
	L->tail = NULL;
}

void destroyList(LL* L){
	/*LLNode* curr;
	while(L->head != NULL) {
		curr = L->head;
		removeNode(L, curr);
	}*/
}

/* getSize(L)
   Return the number of nodes in the list L.
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
   Return value:
     The number of nodes in L.

     ******Tested******
*/ 
int getSize(LL* L){
	LLNode* ptr = L->head;
	int counter = 0;
	if(ptr==NULL)
		return 0;
	while(1) {
		counter++;
		if(ptr->next==NULL) {
			break;
		} else {
			ptr = ptr->next;
		}
	}
	return counter;
}

/* addNode(L, D)
   Create a new list node containing the int D and
   add the node to the front (head) of the list L.
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
	   D is an int.
   Post-conditions:
     A new node containing D has been attached
	   to the beginning of the list L.
   Return value:
     A pointer to the created node.
*/ 
LLNode* addNode(LL* L, int D){
	LLNode* new_node = (LLNode*)malloc( sizeof(LLNode) );
	new_node->element = D;
	if(L->head == NULL) {
		if(DEBUG==1) printf("%s\n", "addNode: Adding first node.");
		L->tail = new_node;
		L->head = new_node;
		new_node->next = NULL;
		new_node->previous = NULL;
	} else if (L->head!=NULL) {
		if(DEBUG==1) printf("%s\n", "addNode: Adding another node.");
		new_node->next = L->head;
		new_node->previous = NULL;
		L->head->previous = new_node;
		L->head = new_node;
	}
	return L->head;
}

/* removeElement(L, D)
   Search the list L for a node containing the provided int D
   and remove the first such node if it is present.
  
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
   Post-conditions:
     If a node containing D was found, it has been removed
	   from the list L.
   Return value:
     1 if a node containing D was found.
	   0 otherwise.
*/ 
int removeElement(LL* L, int D){
	//get index of node containing first occurrence of D.
	LLNode* node = inList(L, D);
	if(node!=NULL) {
		removeNode(L, node);
		return 1;
	}

	return 0;
}

/* removeNode(L, node)
   Remove the provided node from the list L.
   
   Note that the removal process should free all storage associated
   with the node and the string it contains.
   
   Pre-conditions:
     L is a pointer to an initialized LL struct.
	 node points to a valid node in L.
   Post-conditions:
     node has been removed from L.
*/ 
void removeNode(LL* L, LLNode* node){
	//LLNode* delete = node;
	if(DEBUG==1) printf("%s\n",  "In removeNode function.");
	if(L->head==NULL) {
		printf("%s\n", "ERROR in removeNode: List is empty.");
	}
	if((node->previous == NULL) && (node->next == NULL)) {
		if(DEBUG==1) printf("%s\n", "Removing only element.");
		L->head = NULL;
		L->tail = NULL;

	}else if(node->previous == NULL) {
		if(DEBUG==1) printf("%s\n", "removeNode: Removing first element.");
		L->head = node->next;
		node->next->previous = NULL;
	}else if(node->next == NULL) {
		if(DEBUG==1) printf("%s\n", "removeNode: Removing last element.");
		L->tail = node->previous;
		node->previous->next = NULL;
	} else {
		if(DEBUG==1) printf("%s\n", "removeNode: Removing middle element.");
		node->next->previous = node->previous;
		node->previous->next = node->next;
		node->next = NULL;
		node->previous = NULL;
	}
	free(node);
}

/* inList(L, D)
   Search the list L for a node containing the provided D
   and return a pointer to the first node containing D if it is found.
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
   Return value:
     A pointer to the first node containing D if it was found in the list.
	   NULL otherwise.
*/ 
LLNode* inList(LL* L, int D){
	LLNode* ptr = L->head;
	if(ptr==NULL)
		return NULL;
	if(DEBUG==1) printf("%s, ptr->element = %d\n", "In inList function.", ptr->element);

	while(1) {
		if(DEBUG==1) printf("%s, ptr->element = %d\n", "In inList while loop.", ptr->element);
		if(ptr->element == D) {
			return ptr;
		}
		if(DEBUG==1) printf("%s\n", "Trying to move to next node.");

		if(ptr->next==NULL) {
			break;
		} else {
			ptr = ptr->next;
		}
	}
	if(DEBUG==1) printf("%s\n", "inList: not in list.");
	return NULL;
}

LLNode* getIndex(LL* L, int i){
	LLNode* ptr = L->head;
	int counter = i;
	if(ptr==NULL)
		return NULL;
	while(counter>0) {
		if(ptr->next==NULL) {
			return NULL;
		} else {
			ptr = ptr->next;
		}
		counter--;
	}
	return ptr;
}


/*
// **********Main only used for testing Linked List***********

int main(){
	LL S;
	initList(&S);
	LLNode* first_added = addNode(&S, 100);
	if(DEBUG==1) printf("\n%d is in list of size %d\n", inList(&S,100)->element, getSize(&S));
	LLNode* second_added = addNode(&S, 200);
	if(DEBUG==1) printf("\n%d is in list of size %d\n", inList(&S,200)->element, getSize(&S));
	LLNode* third_added = addNode(&S, 300);
	if(DEBUG==1) printf("\n%d is in list of size %d\n", inList(&S,100)->element, getSize(&S));
	LLNode* fourth_added = addNode(&S, 400);
	if(DEBUG==1) printf("\n%d is in list of size %d\n", inList(&S,400)->element, getSize(&S));

	if(DEBUG==1) printf("%s\n", "Nodes added.\n");
	
	if(DEBUG==1) printf("\n%d is at index %d\n", getIndex(&S,0)->element, 0);
	if(DEBUG==1) printf("\n%d is at index %d\n", getIndex(&S,1)->element, 1);
	if(DEBUG==1) printf("\n%d is at index %d\n", getIndex(&S,3)->element, 3);

	if(DEBUG==1) printf("\n%s\n", "Main: Removing last node.");
	removeNode(&S, first_added);
	if(DEBUG==1) printf("\n%s\n", "Main: Removing middle node.");
	removeNode(&S,third_added);
	if(DEBUG==1) printf("\n%s\n", "Main: Removing first node.");
	removeNode(&S, fourth_added);
	if(DEBUG==1) printf("\n%s\n", "Main: Removing only node.");
	removeNode(&S, second_added);

	if(removeElement(&S, 300) && (inList(&S,300)==NULL)) {
		printf("%s\n", "Removed successfully.");
	}

	if(DEBUG==1) printf("\n\n%s, size of list is %d\n", "All nodes removed.", getSize(&S));

	return 0;
}
*/
