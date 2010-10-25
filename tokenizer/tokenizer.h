#include "lists.h"

LinkedList* tokenize (char *expression);

int pushToken(LinkedList *tokenList, int type, char *string);

char *substr(char *string, int start, int end);