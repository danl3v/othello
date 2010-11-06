#include <stdio.h>
#include <stdlib.h>
#include "evaluator.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	listType, closureType, primitiveType, idType
};

Value *add(Value *args) {
	Value *result = malloc(sizeof(*result));
	Node *current = args->val.listValue->head;
	result->type = integerType;
	result->val.integerValue = 0;
	if (args->type == listType) {
		while (current) { // check our error checking. maybe not so good? (+) should be 0
			switch (args->val.listValue->head->value->type) {
				case integerType:
					if (result->type == integerType) {
						result->val.integerValue = result->val.integerValue + current->value->val.integerType;
					}
					else {
						result->val.floatValue = result->val.floatValue + current->value->val.integerValue;
					}
					break;
					
				case floatType:
					if (result->type == integerType) {
						result->val.floatValue = result->val.integerValue;
						result->type = floatType;
					}
					result.floatValue=result.floatValue + current->value->val.floatValue;
					break;
					
				default:
					printf("Error: Attempted to add non-number\n");
					free(result);
					return NULL;
			}
			current = current->next;
		}
		return result;
	}
	else {
		printf("Error: Argument not list\n");
		free(result);
		return NULL;
	}
}

Value *subtract(Value *args) {
	Value *result = malloc(sizeof(*result));
	Node *current = args->val.listValue->head;
	if (!current) {
		printf("-: expects at least 1 argument, given 0\n");
		free(result);
		return NULL;
	}
	if (args->type == listType) {
		if (current->next) {
			switch (current->value->type) {
				case integerType:
					result->type = integerType;
					result->val->integerValue = current->value->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val->floatValue = current->value->val.floatValue;
					break;
				default:
					printf("Error: Attempted to subtract non-number\n");
					free(result);
					return NULL;
			}
			current = current->next;
			while (current) { // check our error checking. maybe not so good?
				switch (args->val.listValue->head->value->type) {
					case integerType:
						if (result->type == integerType) {
							result->val.integerValue = result->val.integerValue - current->value->val.integerType;
						}
						else {
							result->val.floatValue = result->val.floatValue - current->value->val.integerValue;
						}
						break;
						
					case floatType:
						if (result->type == integerType) {
							result->val.floatValue = result->val.integerValue;
							result->type = floatType;
						}
						result.floatValue=result.floatValue - current->value->val.floatValue;
						break;
						
					default:
						printf("Error: Attempted to subtract non-number\n");
						free(result);
						return NULL;
				}
				current = current->next;
			}
		} else {
			switch (current->value->type) {
				case integerType:
					result->type = integerType;
					result->val->integerValue = -1 * current->value->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val->floatValue = -1 * current->value->val.floatValue;
					break;
				default:
					printf("Error: Attempted to subtract non-number\n");
					free(result);
					return NULL;
			}
		}
		return result;
	}
	else {
		printf("Error: Argument not list\n");
		free(result);
		return NULL;
	}
}

Value *multiply(Value *args) {
	Value *result = malloc(sizeof(*result));
	Node *current = args->val.listValue->head;
	result->type = integerType;
	result->val.integerValue = 1;
	if (args->type == listType) {
		while (current) { // check our error checking. maybe not so good? (*) should be 1
			switch (args->val.listValue->head->value->type) {
				case integerType:
					if (result->type == integerType) {
						result->val.integerValue = result->val.integerValue * current->value->val.integerType;
					}
					else {
						result->val.floatValue = result->val.floatValue * current->value->val.integerValue;
					}
					break;
					
				case floatType:
					if (result->type == integerType) {
						result->val.floatValue = result->val.integerValue;
						result->type = floatType;
					}
					result.floatValue = result.floatValue * current->value->val.floatValue;
					break;
					
				default:
					printf("Error: Attempted to multiply non-number\n");
					free(result);
					return NULL;
			}
			current = current->next;
		}
		return result;
	}
	else {
		printf("Error: Argument not list\n");
		free(result);
		return NULL;
	}
}
}

Value *divide(Value *args) {
	Value *result = malloc(sizeof(*result));
	Node *current = args->val.listValue->head;
	if (!current) {
		printf("/: expects at least 1 argument, given 0\n");
		free(result);
		return NULL;
	}
	if (args->type == listType) {
		if (current->next) {
			switch (current->value->type) {
				case integerType:
					result->type = integerType;
					result->val->integerValue = current->value->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val->floatValue = current->value->val.floatValue;
					break;
				default:
					printf("Error: Attempted to divide non-number\n");
					free(result);
					return NULL;
			}
			current = current->next;
			while (current) { // check our error checking. maybe not so good?
				switch (args->val.listValue->head->value->type) {
					case integerType:
						if (current->value->val.integerValue == 0) {
							printf("/: division by zero\n");
							free(result);
							return NULL;
						}
						if (result->type == integerType) {
							result->val.integerValue = result->val.integerValue / current->value->val.integerType;
						}
						else {
							result->val.floatValue = result->val.floatValue / current->value->val.integerValue;
						}
						break;
						
					case floatType:
						if (current->value->val.floatValue == 0) {
							printf("/: division by zero\n");
							free(result);
							return NULL;
						}
						if (result->type == integerType) {
							result->val.floatValue = result->val.integerValue;
							result->type = floatType;
						}
						result.floatValue = result.floatValue / current->value->val.floatValue;
						break;
						
					default:
						printf("Error: Attempted to divide non-number\n");
						free(result);
						return NULL;
				}
				current = current->next;
			}
		} else {
			switch (current->value->type) {
				case integerType:
					if (current->value->val.integerValue == 0) {
						printf("/: division by zero\n");
						free(result);
						return NULL;
					}
					result->type = integerType;
					result->val->integerValue = 1 / current->value->val.integerValue;
					break;
				case floatType:
					if (current->value->val.floatValue == 0) {
						printf("/: division by zero\n");
						free(result);
						return NULL;
					}
					result->type = floatType;
					result->val->floatValue = 1 / current->value->val.floatValue;
					break;
				default:
					printf("Error: Attempted to divide non-number\n");
					free(result);
					return NULL;
			}
		}
	}
	return result;
	else {
		printf("Error: Argument not list\n");
		free(result);
		return NULL;
	}
}
}

Value *car(Value *args) {
	//assert that value is list type
	//loop through list
	// addem up
	// malloc a value
	// return that value
}

Value *cdr(Value *args) {
	//assert that value is list type
	//loop through list
	// addem up
	// malloc a value
	// return that value
}

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
				if (!strcmp(operator->val.idValue, "lambda")) {return evalLambda(args, env);}
				if (!strcmp(operator->val.idValue, "let")) {return evalLet(args, env);}
				if (!strcmp(operator->val.idValue, "letrec")) {return evalLetRec(args, env);}
				if (!strcmp(operator->val.idValue, "if")) {return evalIf(args, env);}
				if (!strcmp(operator->val.idValue, "load")) {return evalLoad(args, env);}
				if (!strcmp(operator->val.idValue, "quote")) {return evalQuote(args, env);}
				if (!strcmp(operator->val.idValue, "'")) {return evalQuote(args, env);}
			} else {
				Value *evaledOperator = eval(operator,env);
				Value *evaledArgs = evalEach(args,env);
				return apply(evaledOperator, evaledArgs);
			}
		default:
			return expr;
	}
}

Value *apply(Value *f, Value *actualArgs) {
	if (f->type == primitiveType) {
		return f->val.primitiveValue(actualArgs);
	} else {
		assert(f->type == closureType);
		Value *formalArgs = f->val.closureValue->actualArgs;
		Environment *frame = createFrame(f->val.closureValue->environment);
		return eval(f->val.closureAvlue->body, frame);
	}
}

void printValue(Value* value) {
	switch (value->type) {
		case booleanType:
			if (head->value->val.boolValue) {
				printf("#t");
			}
			else {
				printf("#f");
			}
			break;			
		case integerType:
			printf("%d", head->value->val.integerValue);
			break;
			
		case floatType:
			printf("%f", head->value->val.floatValue);
			break;
		
		case openType:
			printf("%s", head->value->val.openValue);
			break;
			
		case closeType:
			printf("%s", head->value->val.closeValue);
			break;
			
		case symbolType:
			printf("%s", head->value->val.symbolValue);
			break;
			
		case quoteType:
			printf("%s", head->value->val.quoteValue);
			break;
			
		case stringType:
			printf("%s", head->value->val.stringValue);
			break;
			
		case listType:
			printParseTree(head->value->val.listValue->head);
			break;
		
		case closureType:
		case primitiveType:
			printf("#<procedure>");
		
		
		default:
			printf("invalid type for value structure\n");
			break;		
	}
}