#include "parser.h"

Value *eval(Value *expr, Environment *env);

Value *evalEach(Value *args, Environment *env);

Value *evalDefine(Value *args, Environment *environment);

Value *evalLambda(Value *args, Environment *environment);

Value *evalLet(Value *args, Environment *environment);

Value *evalLetRec(Value *args, Environment *environment);

Value *evalIf(Value *args, Environment *environment);

Value *evalQuote(Value *args, Environment *environment);

Value *envLookup(char *symbol, Environment *environment);

Value *localEnvLookup(char *symbol, Environment *environment);

Value *apply(Value *f, Value *actualArgs);

Value *exponentiate(Value *args);

Environment *createTopFrame();

Environment *createFrame(Environment* parent);

Value *makePrimitiveValue(Value* (*f)(Value *));

Value *add(Value *args);

Value *subtract(Value *args);

Value *multiply(Value *args);

Value *divide(Value *args);

void bind(char *symbol, Value *value, Environment *environment);

void printValue(Value* value);

void evalAll(Value *expr, Environment *env);