typedef struct __Value__ {
	int type;
	int lineNumber;
	//int viewers; ADD THIS!
	union {
		int boolValue;
		int integerValue;
		double floatValue;
		char *stringValue;
		char *symbolValue;
		char *openValue;
		char *closeValue;
		char *quoteValue;
		struct __LinkedList__ *listValue;
	} val;
} Value;

typedef struct __Node__ {
   struct __Value__ *value;
   struct __Node__ *next;
} Node;

typedef struct __LinkedList__ {
   Node *head;
   Node *tail;
} LinkedList;

void create(LinkedList *list);

int push(LinkedList *list, Value *value);

Value *pop(LinkedList *list);

LinkedList* append(LinkedList *list0, LinkedList *list1);

LinkedList* reverse(LinkedList *list);

void destroy(LinkedList *list);

void freeValue(Value *value);

void printList(LinkedList *list);
