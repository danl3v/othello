/* STRUCTS */

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
	Value **bindings;
} Environment;

typedef struct __Closure__{
	Value *formalArguments;
	Value *body;
	Environment *environment;
} Closure;

/* TESTERS */
int tokenTester();

int parseTester();

int evaluateTester();

/* MEMORY */

Pair *mallocPair();

Closure *mallocClosure();

Value *mallocValue();

Value **mallocValueStarStar();

void freeValue(Value *value);

/* LISTS */

Value *car(Value *value);

Value *fakeCar(Value *value);

Value *cdr(Value *value);

Value *fakeCdr(Value *value);

Value *cons(Value *value1, Value *value2);

Value *fakeCons(Value *value);

int isProper(Value **value);

Value **append(Value **value1, Value **value2);

Value **reverse(Value **value);

/* PRINTING */

void printTokens(Value *value);

void printParseTree(Value *value);

void printEvaluation(Value *value);

void printValue(Value *value);

void printValueHelper(Value *value);

/* TOKENIZE */

Value **tokenize (char *expression);

int consToken(Value **tokenList, int type, char *string, int lineNumber);

char *substr(char *string, int start, int end);

/* PARSE */

Value **parse(Value **tokenList, int* depth);

/* PRIMITIVES */

Value *add(Value *args); /* special form for + */

Value *subtract(Value *args); /* special form for - */

Value *multiply(Value *args); /* special form for * */

Value *divide(Value *args); /* special form for / */

Value *equalPointer(Value *args); /* special form for eq? */

Value *isNull(Value *args, Environment *environment); /* special form for null? */

Value *isPair(Value *args, Environment *environment); /* special form for pair? */

Value *equalContent(Value *args, Environment *environment); /* special form for equal? */

Value *equalNumber(Value *args, Environment *environment); /* special form for = */

Value *lessThan(Value *args, Environment *environment); /* special form for < */

Value *__or__(Value *args, Environment *environment); /* special form for or */

Value *__and__(Value *args, Environment *environment); /* special form for and */

Value *__setBang__(Value *args, Environment *environment); /* special form for set! */

/* SPECIAL FORMS */

Value *evalDefine(Value *args, Environment *environment);

Value *evalIf(Value *args, Environment *environment);

Value *evalLambda(Value *args, Environment *environment);

Value *evalLet(Value *args, Environment *environment);

/* EVALUATE */

Environment* createTopFrame();

Environment* createFrame(Environment *parent);

Value *environmentLookup(char *symbol, Environment *environment, int local);

void bind(char *symbol, Value *value, Environment *environment);

Value **evaluate(Value **parseTree, Environment *environment);

Value **evalTop(Value **tree, Environment *environment);

Value **evalEach(Value **tree, Environment *environment);

Value *eval(Value *value, Environment *environment);

Value *apply(Value *f, Value **actualArgs);