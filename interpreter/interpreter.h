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

Pair *mallocPair();

Value *mallocValue();

Value **mallocValueStarStar();

Value *car(Value *value);

Value *cdr(Value *value);

Value *cons(Value *value1, Value *value2);

int isProper(Value **value);

Value **reverse(Value **value);

void printTokens(Value *value);

void printValue(Value *value);

void printValueHelper(Value *value);

Value **tokenize (char *expression);

int consToken(Value **tokenList, int type, char *string, int lineNumber);

char *substr(char *string, int start, int end);