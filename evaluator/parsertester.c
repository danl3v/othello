#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

enum ERROR_TYPE {
	SYNTAX_ERROR_UNTOKENIZABLE, SYNTAX_ERROR_UNTERMINATED_INPUT, SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES
};

int main(int argc, char *argv[]) {
	int depth = 0;
	LinkedList *tokens = NULL;
	LinkedList *parseTree = NULL;
	LinkedList *leftoverTokens = NULL;
	
	char *expression = malloc(256 * sizeof(char));
	while (fgets(expression, 255, stdin)) {
		tokens = tokenize(expression);
		//printf("Tokenized\n");
		if (!tokens) { // store a tail, if last thing in token list is null
			printf("syntax error\n"); // get the line number somehow
			return SYNTAX_ERROR_UNTOKENIZABLE;
		}
	
		//printf("tokens:\n");
		//printList(tokens);
		
		
		tokens = append(leftoverTokens, tokens);
		
		//printf("appended tokens\n");
		//printList(tokens);
		
		parseTree = parse(tokens, &depth);
		
		if (depth < 0) {
			printf("syntax error: too many close parentheses\n");
			return SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES;
		}
		else if (depth > 0) {
			//printf("we have leftovers\n");
			leftoverTokens = tokens;
			depth = 0;
		}
		else {
			if (parseTree) {
				printf("THE TREE:\n");
				printParseTree(parseTree->head);
			}
			else {
				printf("null parse tree");
			}
			printf("\n");
			leftoverTokens = NULL;
		}
	}
	free(expression);
	// think about destroying tokens at the end
	return 0;
}