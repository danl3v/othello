#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	listType, closureType, primitiveType, idType
};

Value* createValueObjectofListType(LinkedList *parseTree) {	
	LinkedList *list = malloc(sizeof(*list));
	Value *value = malloc(sizeof(*value));
	create(list);
	
	while (parseTree->head->value->type != openType) {
		push(list, pop(parseTree));
		if (!parseTree->head) {
			//destroy(list);
			//free(value)
			return NULL;
		}
	}
	pop(parseTree); // get rid of the open paren
	
	value->type = listType;
	value->lineNumber = -1;
	value->val.listValue = list;
	return value;
}

LinkedList* parse(LinkedList* tokenList, int* depth) {
	//printf("Entered Parse\n");
	Value *valueObject;
	Node* current;
	LinkedList* parseTree = malloc(sizeof(*parseTree));
	create(parseTree);
	
	if (tokenList && parseTree) {
		current = tokenList->head;
		while (current) {
			push(parseTree, current->value);
			if (current->value->type == closeType) {
				pop(parseTree);
				valueObject = createValueObjectofListType(parseTree);
				if (valueObject) {
					push(parseTree, valueObject);
					(*depth)--;
				}
				else {
					(*depth)--;
					return NULL;
				}
			}
			else if (current->value->type == openType){
				(*depth)++;
			}
			current = current->next;
		}
		return parseTree;
	}
	else {
		return NULL;
	}
	//printf("Done parsing");
}

void printParseTree(Node* head) {
	while (head) {
		switch (head->value->type) {
		
			case booleanType:
				if (head->value->val.boolValue) {
					printf("#t ");
				}
				else {
					printf("#f ");
				}
				break;
				
			case integerType:
				printf("%d", head->value->val.integerValue);
				break;
				
			case floatType:
				printf("%f", head->value->val.floatValue);
				break;
			
			case openType:
				printf("%s", head->value->val.openValue);
				break;
				
			case closeType:
				printf("%s", head->value->val.closeValue);
				break;
				
			case symbolType:
				printf("%s", head->value->val.symbolValue);
				break;
				
			case quoteType:
				printf("%s", head->value->val.quoteValue);
				break;
				
			case stringType:
				printf("%s", head->value->val.stringValue);
				break;
				
			case listType:
				printf("(");
				printParseTree(head->value->val.listValue->head);
				printf(")");
				break;
				
			default:
				printf("invalid type for value structure\n");
				break;
				
		}
		
		if (head->next) {
			printf(" ");
		}
		
		head = head->next;
	}
	
}