#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType
};

void create(LinkedList *list) {
   list->head = NULL;
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


Value* pop(LinkedList *list) {
	Value *value = list->head->value;
	Node *head = list->head;
	list->head = list->head->next;
	free(head);
	return value;
}

void destroy(LinkedList *list) {
   Node *current = list->head;
   while(current) {
      Node *next = current->next;
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
