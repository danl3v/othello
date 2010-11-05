#include <stdio.h>
#include <stdlib.h>
#include "evaluator.h"

Value *eval(Value *expr, Environment *env) {
	switch (expr->type) {
		case boolType:
		case integerType:
		case floatType:
		case stringType:
			return expr;
		case symbolType:
			return envLookup(expr->val.symbolValue,env);
		case listType:
			Value *operator = car(expr);
			Value *args = cdr(expr);
			if (operator->type == idType) {
				if (!strcmp(operator->val.idValue, "define")) {return evalDefine(args, env);}
				if (...){};
			} else {
				Value *evaledOperator = eval(operator,env);
				Value *evaledArgs = evalEach(args,env);
				return apply(evaledOperator, evaledArgs);
			}
		default:
			return expr;
	}
}