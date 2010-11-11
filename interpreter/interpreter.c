#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	pairType, closureType, primitiveType
};

Value *mallocValue() {
	Value *value = malloc(sizeof(*value));
	return value;
}

Pair *mallocPair() {
	Pair *pair = malloc(sizeof(*pair));
	return pair;
}

Value *car(Value *value) {
	if (value->type == pairType) {
		return value->val.pairValue->car;
	} else {
		return NULL;
	}
}

Value *cdr(Value *value) {
	if (value->type == pairType) {
		return value->val.pairValue->cdr;
	} else {
		return NULL;
	}
}

Value *cons(Value *value1, Value *value2) {
	Value *result = mallocValue();
	Pair *pair = mallocPair();
	pair->car = value1;
	pair->cdr = value2;
	result->type = pairType;
	result->val.pairValue = pair;
	printf("cons complete\n");
	return result;
}

void printValue(Value *value) {
	if (value->type == pairType) {
		printf("(");
		printValueHelper(value);
		printf(")\n");
	} else {
		printValueHelper(value);
		printf("\n");
	}
}

void printValueHelper(Value* value) {
	if (!value) {
		return;
	}
	switch (value->type) {
		case booleanType:
			if (value->val.booleanValue) {
				printf("#t");
			}
			else {
				printf("#f");
			}
			break;	
			
		case integerType:
			printf("%d", value->val.integerValue);
			break;
			
		case floatType:
			printf("%f", value->val.floatValue);
			break;
		
		case openType:
			printf("%s", value->val.openValue);
			break;
			
		case closeType:
			printf("%s", value->val.closeValue);
			break;
			
		case symbolType:
			printf("%s", value->val.symbolValue);
			break;
			
		case quoteType:
			printf("%s",value->val.quoteValue);
			break;
			
		case stringType:
			printf("%s", value->val.stringValue);
			break;
			
		case pairType:
			if (value->val.pairValue->car->type == pairType) {
				printValue(value->val.pairValue->car);
			} else {
				printValueHelper(value->val.pairValue->car);
			}
			if (value->val.pairValue->cdr) {
				printf(" ");
				if (value->val.pairValue->cdr->type == pairType) {
					printValueHelper(value->val.pairValue->cdr);
				} else {
					printf(". ");
					printValueHelper(value->val.pairValue->cdr);
				}
			}
			break;

		case closureType:
			printf("#<closure>");
			break;
			
		case primitiveType:
			printf("#<procedure>"); // maybe have some sort of label
			break;
		
		default:
			printf("i don't know what type of value i am");
			break;		
	}
}