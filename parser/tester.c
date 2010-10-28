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
	LinkedList *leftOvers = NULL;
	
	char *expression = malloc(256 * sizeof(char));
	while (fgets(expression, 255, stdin)) {
		tokens = tokenize(expression);
		printf("Tokenized\n");
		if (!tokens) { // store a tail, if last thing in token list is null
			printf("syntax error\n"); // get the line number somehow
			return SYNTAX_ERROR_UNTOKENIZABLE;
		}
	
		printf("abt to append\n");
		tokens = append(leftOvers, tokens);

		printf("abot to parse\n");
		parseTree = parse(tokens, &depth);
		
		if (depth < 0) {
			printf("syntax error\n");
			return SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES;
		}
		else if (depth > 0) {
			printf("we have leftovers\n");
			leftOvers = tokens;
			depth = 0;
		}
		else {
			if (parseTree) {
				printf("THE TREE\n");
				printParseTree(parseTree->head);
			}
			else {
				printf("null parse tree");
			}
			printf("\n");
			leftOvers = NULL;
		}
	}
	free(expression);
	// think about destroying tokens at the end
	return 0;
}