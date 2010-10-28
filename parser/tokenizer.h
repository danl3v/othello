#include "lists.h"

LinkedList* tokenize (char *expression);

int pushToken(LinkedList *tokenList, int type, char *string, int lineNumber);

char *substr(char *string, int start, int end);