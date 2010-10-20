#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

int main(int argc, char *argv[]) {
	char *expression = malloc(256 * sizeof(char));
	
	while (fgets(expression, 255, stdin)) {
		tokenize(expression);
	}
	free(expression);
}