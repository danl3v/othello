#include "tokenizer.h"

LinkedList* parse(LinkedList* tokenList, int* depth);

void printParseTree(Node* head);

Value* createValueObjectofListType(LinkedList *parseTree);