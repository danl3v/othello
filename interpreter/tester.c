#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	pairType, closureType, primitiveType
};

/*
int main() {
	Value *value1 = mallocValue();
	value1->type = integerType;
	value1->val.integerValue = 1;
	Value *value2 = mallocValue();
	value2->type = integerType;
	value2->val.integerValue = 2;
	Value *value3 = mallocValue();
	value3->type = integerType;
	value3->val.integerValue = 3;
	Value *listTemp = cons(value1, value3);
	Value **list = &(listTemp);
	//printf("%d\n", (list)->type);
	printValue(*list);
	return 1;
}
*/

int main(int argc, char *argv[]) {
	char *expression = malloc(256 * sizeof(char));
	Value **tokens;
	while (fgets(expression, 255, stdin)) {
		tokens = tokenize(expression);
		if (tokens) {
		printValue(*tokens);
		//destroy(tokens);
		}
	}
	free(expression);
	return 0;
}