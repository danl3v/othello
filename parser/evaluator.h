#include "parser.h"

Value *eval(Value *expr, Environment *env);

Value *evalEach(Value *args, Environment *env);

Value *envLookup(char *symb, Environment *env);

Value *apply(Value *f, Value *actualArgs);

Value *exponentiate(Value *args);

Environment *createTopFrame();

Value *makePrimitiveValue(Value* (*f)(Value *));