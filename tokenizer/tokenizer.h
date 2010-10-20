#include "lists.h"

LinkedList* tokenize (char *expression);

void freeValue(Value *value);

int pushToken(LinkedList *tokenList, int type, char *string);

char *substr(char *string, int start, int end);