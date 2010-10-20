typedef struct __Value__ {
	int type;
	union {
		int boolValue;
		int integerValue;
		double floatValue;
		char *stringValue;
		char *symbolValue;
		char *openValue;
		char *closeValue;
		char *quoteValue;
	} val;
} Value;

typedef struct __Node__ {
   struct __Value__ *value;
   struct __Node__ *next;
} Node;

typedef struct __LinkedList__ {
   Node *head;
} LinkedList;


void create(LinkedList *list);

int push(LinkedList *list, Value *value);

Value* pop(LinkedList *list);

/*int *lookup(LinkedList *list, int index);

int length(LinkedList *list);

int isEmpty(LinkedList *list); */

void destroy(LinkedList *list);

void printList(LinkedList *list);
