#include "parser.h"

Value *eval(Value *expr, Environment *env);

Value *evalEach(Value *args, Environment *env);

Value *evalDefine(LinkedList *args, Environment *environment);

Value *evalLambda(LinkedList *args, Environment *environment);

Value *evalLet(LinkedList *args, Environment *environment);

Value *evalLetRec(LinkedList *args, Environment *environment);

Value *evalIf(LinkedList *args, Environment *environment);

Value *evalQuote(LinkedList *args, Environment *environment);

Value *envLookup(char *symbol, Environment *environment);

Value *apply(Value *f, Value *actualArgs);

Value *exponentiate(Value *args);

Environment *createTopFrame();

Value *makePrimitiveValue(Value* (*f)(Value *));

Value *add(Value *args);

Value *subtract(Value *args);

Value *multiply(Value *args);

Value *divide(Value *args);