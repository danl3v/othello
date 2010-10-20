#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

int main(int argc, char *argv[]) {
	char *expression = malloc(256 * sizeof(char));
	LinkedList *tokens;
	while (fgets(expression, 255, stdin)) {
		tokens = tokenize(expression);
		printList(tokens);
		destroy(tokens);
	}
	free(expression);
}