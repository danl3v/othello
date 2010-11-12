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
	char *expression = malloc(256 * sizeof(char));
	Value **tokens;
	Value **leftoverTokens;
	Value **parseTree;
	
	while (fgets(expression, 255, stdin)) {
		tokens = tokenize(expression);
		if (!tokens) {
			printf("syntax error");
			return SYNTAX_ERROR_UNTOKENIZABLE;
		}
		
		parseTree = parse(tokens, &depth);
		
		if (depth < 0) {
			printf("syntax error: too many close parentheses\n");
			return SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES;
		}
		else if (depth > 0) {
			/*printf("we have leftovers\n");
			leftoverTokens = tokens;
			depth = 0;*/
			printf("leftovers");
		}
		else {
			if (parseTree) {
				printf("THE FINAL TREE\n");
				printValue(*parseTree);
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