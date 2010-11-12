#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	pairType, closureType, primitiveType
};


enum ERROR_TYPE {
	SYNTAX_ERROR_UNTOKENIZABLE, SYNTAX_ERROR_UNTERMINATED_INPUT, SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES
};

int tokenTester() {
	char *expression = malloc(256 * sizeof(char));
	Value **tokens;
	
	while (fgets(expression, 255, stdin)) {
		tokens = tokenize(expression);
		if (tokens) {
			printTokens(*tokens);
		/*destroy(tokens);*/
		}
	}
	free(expression);
	return 0;
}

int parseTester() {
	int depth = 0;
	char *expression = malloc(256 * sizeof(char)); /* i dont think we need to malloc this */
	Value **tokens = NULL;
	Value **leftoverTokens = NULL;
	Value **parseTree = NULL;
	
	while (fgets(expression, 255, stdin)) {
		printf("aaa\n");
		tokens = tokenize(expression);
		if (tokens) {
			printTokens(*tokens);
		}
		
				printf("aaa22\n");
		if (leftoverTokens) {
		printTokens(*leftoverTokens);
		}
		printf("aaa33\n");
		if (leftoverTokens) {
			*tokens = append(*leftoverTokens, *tokens);
		}
		
		parseTree = parse(tokens, &depth);
		
		if (depth > 0) {
			printf("leftovers with depth: %d\n", depth);
			leftoverTokens = tokens;
			depth = 0;
		}
		else {
			if (parseTree) {
				printf("THE FINAL TREE:\n");
				printValueHelper(*parseTree);
			}
			else {
				printf("null parse tree");
			}
			printf("\n");
			leftoverTokens = NULL;
		}
		
	}
	free(expression);
	return 0;
}

int main(int argc, char *argv[]) {
	return parseTester();
}