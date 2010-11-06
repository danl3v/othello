#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	listType, closureType, primitiveType, idType
};

void create(LinkedList *list) {
   list->head = NULL; // why do we do it this way again??
   list->tail = NULL;
}

int push(LinkedList *list, Value *value) {
	Node *node = malloc(sizeof(Node));
	if (node) {
		if (list->head == NULL && list->tail == NULL) {
			//printf("setting tail for first time\n");
			list->tail = node;
		}
		node->value = value;
		node->next = list->head;
		list->head = node;
    }
    return (node != NULL);
}

Value *pop(LinkedList *list) {
	Value *value = list->head->value;
	Node *head = list->head;
	if (list->head == list->tail) {
		list->tail = NULL;
	}
	list->head = list->head->next; // should be null when we remove the last item
	//free(head);
	return value; // does not free the value since we are returing a pointer to it
}

// we can probs delete this function soon
void printLast(LinkedList *list) {
	Node *current = list->tail;
	switch(current->value->type) {
			case booleanType:
				if (current->value->val.boolValue) {
					printf("#t:boolean\n");
				}
				else {
					printf("#f:boolean\n");
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


}

LinkedList *append(LinkedList *list0, LinkedList *list1) {
	if (list0) {
		if (list1) {
			//printf("setting head\n");
			list0->tail->next = list1->head;
			//printf("setting tail\n");
			list0->tail = list1->tail;
			//free(list1);
			return list0;
		}
		else {
			//printf("list1 NULL\n");
			return list0;
		}
	}
	else if (list1) {
		//printf("list0 NULL\n");
		return list1;
	}
	else {
		//printf("both lists NULL\n");
		return NULL;
	}
}

LinkedList* reverse(LinkedList *list) {
    LinkedList *new_list = malloc(sizeof(*new_list));
    create(new_list);
    //printf("reversing\n");
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        push(new_list, current->value);
        free(current);
        current = next;
    }
    list->head = new_list->head; // think about memory, what do we need to free
    list->tail = new_list->tail;
    return list;
}

// have a lst of malloced values and then free all that at the end
// freeing values after we hit enter
// in destroy we want to free the values
void freeValue(Value *value) {
	switch (value->type) {
	
		case stringType:
		   free(value->val.stringValue);
		   break;
		   
		case symbolType:
		   free(value->val.symbolValue);
		   break;
		   
		case openType:
		   free(value->val.openValue);
		   break;
		   
	    case closeType:
		   free(value->val.closeValue);
		   break;
		   
	    case quoteType:
		   free(value->val.quoteValue);
		   break;
		
		case listType:
			destroy(value->val.listValue);
			break;
			
		default:
			break;
    }
    free(value);
}

void destroy(LinkedList *list) {
   Node *current = list->head;
   while(current) {
      Node *next = current->next;
      freeValue(current->value);
      free(current);
      current = next;
   }
   // also want to free the values -- want to loop through value list and free each value
   list->head = NULL; // think about memory
   list->tail = NULL;
}

void printList(LinkedList *list) {
   if (list) {
	   Node *current = (*list).head;
	   while(current) {
		  switch(current->value->type) {
			case booleanType:
				if (current->value->val.boolValue) {
					printf("#t:boolean\n");
				}
				else {
					printf("#f:boolean\n");
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
		  current = (*current).next; // be consistent with all this
	   }
	}
}

Value* car(LinkedList *list){
	return list->head->value;
}

LinkedList* cdr(LinkedList *list){
	if(list->head->next){
		LinkedList *newList;
		create(newList);
		newList->head=list->head->next;
		newList->tail=list->tail;
		return newList;
	}
	printf("Error: cdr of empty list");
	return NULL;
}