typedef struct __Value__ {
	int type;
	int lineNumber;
	union {
		int booleanValue;
		int integerValue;
		double floatValue;
		char *stringValue;
		char *symbolValue;
		char *openValue;
		char *closeValue;
		char *quoteValue;
		struct __Pair__ *pairValue;
		struct __Closure__ *closureValue;
		struct __Value__* (*primitiveValue)(struct __Value__*);
	} val;
} Value;

typedef struct __Pair__ {
	struct __Value__ *car;
	struct __Value__ *cdr;
} Pair;

typedef struct __Environment__{
	struct __Environment__ *parentFrame;
	Pair *bindings;
} Environment;


/* TESTERS */
int tokenTester();

int parseTester();

int evaluatorTester();

/* MEMORY */

Pair *mallocPair();

Value *mallocValue();

Value **mallocValueStarStar();

void freeValue(Value *value);

/* LISTS */

Value *car(Value *value);

Value *cdr(Value *value);

Value *cons(Value *value1, Value *value2);

int isProper(Value **value);

Value **append(Value **value1, Value **value2);

Value **reverse(Value **value);

/* PRINTING */

void printTokens(Value *value);

void printParseTree(Value *value);

void printValue(Value *value);

void printValueHelper(Value *value);

/* TOKENIZE */

Value **tokenize (char *expression);

int consToken(Value **tokenList, int type, char *string, int lineNumber);

char *substr(char *string, int start, int end);

/* PARSE */

Value **parse(Value **tokenList, int* depth);

/* EVALUATE */