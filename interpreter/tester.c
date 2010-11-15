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
	
	printf("> ");
	while (fgets(expression, 255, stdin)) {
		tokens = append(leftoverTokens, tokenize(expression));
		if (tokens) { printf("\nTOKENS:\n"); printTokens(*tokens); }

		parseTree = parse(tokens, &depth);

		if (depth > 0) {
			leftoverTokens = tokens;
			depth = 0;
		}
		else {
			if (parseTree) { 
				printf("\nPARSE TREE:\n"); 
				printParseTree(*parseTree); 
				printf("\n");
			}
			else { depth = 0; }
			leftoverTokens = NULL;
			printf("> ");
		}
		
	}
	free(expression);
	return 0;
}

int evaluateTester() {
	int depth = 0;
	char *expression = malloc(256 * sizeof(char)); /* i dont think we need to malloc this */
	Value **tokens = NULL;
	Value **leftoverTokens = NULL;
	Value **parseTree = NULL;
	Value **value = NULL;
	Environment *topFrame = createTopFrame();
	/* http://www.daniweb.com/forums/thread23485.html - key up down functionality */
	printf("> ");
	while (fgets(expression, 255, stdin)) {
		tokens = append(leftoverTokens, tokenize(expression));
		if (tokens) { printf("\nTOKENS:\n"); printTokens(*tokens); }

		parseTree = parse(tokens, &depth);

		if (depth > 0) {
			leftoverTokens = tokens;
			depth = 0;
		}
		else {
			if (parseTree) {
				printf("\nPARSE TREE:\n"); 
				printParseTree(*parseTree); 
				printf("\n");
				value = evaluate(parseTree, topFrame);
				printf("\nVALUE:\n"); 
				printEvaluation(*value);
			}
			else { depth = 0; }
			leftoverTokens = NULL;
			printf("> ");
		}
	}
	free(expression);
	return 0;
}

int main(int argc, char *argv[]) {
	return evaluateTester();
}