#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType
};

void create(LinkedList *list) {
   list->head = NULL; // why do we do it this way again??
}

int push(LinkedList *list, Value *value) {
      Node *node = malloc(sizeof(Node));
      if (node) {
	  	  node->value = value;
	 	  node->next = list->head;
		  list->head = node;
      }
      return (node != NULL);
}

Value *pop(LinkedList *list) {
	Value *value = list->head->value;
	Node *head = list->head;
	list->head = list->head->next;
	// free(head->value); need to free the malloc-ed data in the union
	free(head);
	return value;
}

void reverse(LinkedList *list) {
    LinkedList *new_list = malloc(sizeof(*new_list));
    create(new_list);
    
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        push(new_list, current->value);
        free(current); // frees the current node
        current = next;
    }
    list->head = new_list->head; // think about memory
}

void destroy(LinkedList *list) {
   Node *current = list->head;
   while(current) {
      Node *next = current->next; // also want to free the values
      free(current);
      current = next;
   }
   list->head = NULL;
}

void printList(LinkedList *list) {
   Node *current = (*list).head;
   while(current) {
   	  switch(current->value->type) {
 	  	case booleanType:
 	  		if (current->value->val.boolValue) {
 	  			printf("#t:boolean\n", current->value->val.boolValue);
 	  		}
 	  		else {
 	  			printf("#f:boolean\n", current->value->val.boolValue);
 	  		}
			break;
		case integerType:
			printf("%d:integer\n", current->value->val.integerValue);
			break;
		case floatType:
			printf("%f:float\n", current->value->val.floatValue);
			break;
		case stringType:
			printf("%s:string\n", current->value->val.stringValue);
			break;
		case symbolType:
			printf("%s:symbol\n", current->value->val.symbolValue);
			break;
   	  	case openType:
   	  		printf("%s:open\n", current->value->val.openValue);
   	  		break;
   	  	case closeType:
   	  		printf("%s:close\n", current->value->val.closeValue);
   	  		break;
   	  	case quoteType:
   	  		printf("%s:quote\n", current->value->val.quoteValue);
			break;
		default:
			printf("invalid type for value structure");
			break;
   	  }
      current = (*current).next;
   }
}


// have a lst of malloced values and then free all that at the end
// freeing values after we hit enter
// in destroy we want to free the values

/* void freeValue(Value *value) { */
/*    switch (value->type) { */
/*    case stringType: */
/*       free("the string"); */
/*       break; */
/*    case symbolType: */
/*       free("the name"); */
/*       break; */
/*    ... */
/*    } */
   
/*    free(value); */
/* } */

