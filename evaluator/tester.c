#include <stdio.h>
#include <stdlib.h>
#include "evaluator.h"

enum ERROR_TYPE {
	NO_ERROR, SYNTAX_ERROR_UNTOKENIZABLE, SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES
};

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	listType, closureType, primitiveType, idType
};

int main(int argc, char *argv[]) {
	Value *valueTree = malloc(sizeof(*valueTree)); // are you sure we need to malloc here
	valueTree->type = listType;
	int depth = 0;
	Value *exprValue = malloc(sizeof(*exprValue)); // are you sure we need to malloc here
	LinkedList *tokens = malloc(sizeof(*tokens)); // are you sure we need to malloc here
	//LinkedList *parseTree = NULL;
	LinkedList *leftoverTokens = malloc(sizeof(*leftoverTokens)); // are you sure we need to malloc here!
	//printf("before createTopFrame\n");
	Environment *topFrame = createTopFrame();
	//printf("after createTopFrame\n");
	//create(tokens);
	//create(leftoverTokens);
	char *expression = malloc(256 * sizeof(char));
	printf("> "); // can we put this into the fets??
	while (fgets(expression, 255, stdin)) {
		//printf("fgets\n");
		tokens = tokenize(expression);
		//printf("tokenized\n");
		if (!tokens) { // store a tail, if last thing in token list is null
			printf("syntax error\n"); // get the line number somehow
			return SYNTAX_ERROR_UNTOKENIZABLE;
		}

		//printf("tokens:\n");
		//printList(tokens);
		//printf("about to append\n");
		//printList(leftoverTokens);
		tokens = append(leftoverTokens, tokens);
		
		//printf("appended tokens\n");
		//printList(tokens);
		valueTree->val.listValue = parse(tokens, &depth);
		//printf("parsed\n");
		//printf("THE TREE:\n");
		//printParseTree(valueTree->val.listValue->head);
		//printf("\n");
		//printf("xxxxx: %s\n", valueTree->val.listValue->head->value->val.symbolValue);

		if (depth < 0) {
			printf("syntax error: too many close parentheses\n");
			return SYNTAX_ERROR_TOO_MANY_CLOSE_PARENTHESES;
		}
		else if (depth > 0) {
			//printf("we have leftovers\n");
			//leftoverTokens = tokens;
			depth = 0;
		}
		else {
			if (valueTree) {
				evalAll(valueTree, topFrame);
				printf("evaluated\n");
			}
			else {
				printf("null parse tree\n");
			}
			//printf("\n");
			create(leftoverTokens); //Deal with memory stuff.
		}
		printf("> "); // do not print this out if we have leftover tokens
	}
	free(expression);
	// think about destroying tokens at the end
	return 0;
}