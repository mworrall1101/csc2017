/* linked_list.h
  
  Declarations and structure for a doubly linked list containing an int as an element.

  Adapted by Mark Worrall from code written by Bill Bird - 09/26/17.

*/

const int DEBUG;

typedef struct LLNode{
	int element;
	struct LLNode* next;
  struct LLNode* previous;
} LLNode;

typedef struct{
	
	LLNode* head;
  LLNode* tail;
	
} LL;


/* initList(L)
   Initialize the provided LinkedList instance.
   
   Pre-conditions:
     L is a pointer to a LinkedList struct.
   Post-conditions:
     The head and tail pointers of L have been
     initialized to NULL.
*/ 
void initList(LL* L);



/* destroyList(L)
   Free all storage associated with each node of the LinkedList
   object and reset all pointers to NULL.
   
   Pre-conditions:
     L is a pointer to a LinkedList which has been
	   initialized with initList and has not already
	   been destroyed.
   Post-conditions:
     Each node of L, has been freed.
	   The head and tail pointers of L have been set to NULL.
*/ 
void destroyList(LL* L);


/* getSize(L)
   Return the number of nodes in the list L.
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
   Return value:
     The number of nodes in L.
*/ 
int getSize(LL* L);


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
LLNode* addNode(LL* L, int D);



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
int removeElement(LL* L, int D);

/* removeNode(L, node)
   Remove the provided node from the list L.
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
	   node points to a valid node in L.
   Post-conditions:
     node has been removed from L and storage has been freed.
*/ 
void removeNode(LL* L, LLNode* node);


/* inList(L, D)
   Search the list L for a node containing the provided D
   and return a pointer to the first node containing D if it is found.
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
   Return value:
     A pointer to the first node containing D if it was found in the list.
	   NULL otherwise.
*/ 
LLNode* inList(LL* L, int D);


/* getIndex(L, i)
   Return the node at the provided index i (where index 0 is 
   the node at the head of the list).
   
   Pre-conditions:
     L is a pointer to an initialized LinkedList.
   Return value:
     A pointer to node i if the index is in the list.
	   NULL if i is not a valid index into the list.
*/ 
LLNode* getIndex(LL* L, int i);