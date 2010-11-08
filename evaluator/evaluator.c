#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
						result->val.integerValue = result->val.integerValue + current->value->val.integerValue;
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
					result->val.floatValue = result->val.floatValue + current->value->val.floatValue;
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
					result->val.integerValue = current->value->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val.floatValue = current->value->val.floatValue;
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
							result->val.integerValue = result->val.integerValue - current->value->val.integerValue;
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
						result->val.floatValue = result->val.floatValue - current->value->val.floatValue;
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
					result->val.integerValue = -1 * current->value->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val.floatValue = -1 * current->value->val.floatValue;
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
						result->val.integerValue = result->val.integerValue * current->value->val.integerValue;
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
					result->val.floatValue = result->val.floatValue * current->value->val.floatValue;
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
					result->val.integerValue = current->value->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val.floatValue = current->value->val.floatValue;
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
							result->val.integerValue = result->val.integerValue / current->value->val.integerValue;
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
						result->val.floatValue = result->val.floatValue / current->value->val.floatValue;
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
					result->val.integerValue = 1 / current->value->val.integerValue;
					break;
				case floatType:
					if (current->value->val.floatValue == 0) {
						printf("/: division by zero\n");
						free(result);
						return NULL;
					}
					result->type = floatType;
					result->val.floatValue = 1 / current->value->val.floatValue;
					break;
				default:
					printf("Error: Attempted to divide non-number\n");
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

Value *car(Value *args) {
	if (args->type == listType) {
		return args->val.listValue->head->value;
	}
	else {
		printf("Error: Cannot return car of a non-list\n");
		return NULL;
	}
}

Value *cdr(Value *args) { //deal with returning NULL being legitimate. (errorType for Value objects?)
	printValue(args);
	printValue(args);
	if (args->type == listType) {
		if (args->val.listValue->head) {
			if (args->val.listValue->head->next) {
				Value *newValue = malloc(sizeof(*newValue));
				newValue->type = listType;
				newValue->val.listValue = malloc(sizeof(newValue->val.listValue));
				newValue->val.listValue->head = args->val.listValue->head->next;
				newValue->val.listValue->tail = args->val.listValue->tail;
				printf("cdr returning not NULL\n");
				return newValue;
			}
			printf("cdr returning NULL\n");
			return NULL;
		}
		printf("Error: cdr of empty list\n");
		return NULL;
	}
	printf("Error: Cannot return cdr of a non-list\n");
	return NULL;
}

void bind(char *symbol, Value *value, Environment *environment){
	//printf("in bind\n");
	Value *binding = malloc(sizeof(*binding));
	binding->type = listType;
	binding->val.listValue = malloc(sizeof(binding->val.listValue));
	create(binding->val.listValue);
	//printf("binding created\n");
	Value *symbolValue = malloc(sizeof(*symbolValue));
	symbolValue->type = symbolType;
	symbolValue->val.symbolValue = symbol;
	push(binding->val.listValue, value);
	push(binding->val.listValue, symbolValue);
	push(environment->bindings, binding);
}

Value *makePrimitiveValue(Value* (*f)(Value *)){
	Value *resultValue = malloc(sizeof(resultValue));
	resultValue->type = primitiveType;
	resultValue->val.primitiveValue = f;
	return resultValue;
}

Environment* createFrame(Environment *parent) {
	Environment *frame = malloc(sizeof(*frame));
	frame->parentFrame = parent;
	//frame->bindings = NULL;
	frame->bindings = malloc(sizeof(frame->bindings));
	create(frame->bindings);
	return frame;
}

Environment* createTopFrame() {
	//printf("start createTopFrame\n");
	Environment *topFrame = createFrame(NULL);
	//printf("before bindings\n"); 
	bind("+", makePrimitiveValue(add), topFrame);
	//printf("+ bound\n");
	bind("-", makePrimitiveValue(subtract), topFrame);
	bind("*", makePrimitiveValue(multiply), topFrame);
	bind("/", makePrimitiveValue(divide), topFrame);
	bind("car", makePrimitiveValue(car), topFrame);
	bind("cdr", makePrimitiveValue(cdr), topFrame);
	return topFrame;
}

Value *envLookup(Value *symbol, Environment *environment) {
	Node *current = malloc(sizeof(*current));
	printf("In envLookup\n");
	current = environment->bindings->head;
	while (current) {
		//printf("In while\n");
		//printf("binding: %s\n", current->value->val.listValue->head->value->val.symbolValue);
		//printf("lookup: %s\n", symbol->val.symbolValue);
		if (!strcmp(current->value->val.listValue->head->value->val.symbolValue, symbol->val.symbolValue)) {
			printf("found %s!\n", symbol->val.symbolValue);
			return current->value->val.listValue->head->next->value;
		}
		current = current->next;
	}
	if (environment->parentFrame) {
		envLookup(symbol, environment->parentFrame);
	}
	printf("reference to undefined identifier: %s\n", symbol->val.symbolValue);
	return NULL;
}

Value *evalEach(Value *args, Environment *env) {
	Value *evaluated = malloc(sizeof(*evaluated));
	evaluated->type = listType;
	evaluated->val.listValue = malloc(sizeof(evaluated->val.listValue));
	create(evaluated->val.listValue);
	printf("asd\n");
	if (args->type == listType) {
		printf("asdf\n");
		Node *current = args->val.listValue->head;
		while (current) {
			printf("current value: %d", current->value->val.integerValue);
			push(evaluated->val.listValue, eval(current->value, env));
			current = current->next;
		}
		evaluated->val.listValue = reverse(evaluated->val.listValue);
		return evaluated;
	}
	else {
		printf("error: in evalEach, args is not a LinkedList\n");
		free(evaluated);
		return NULL;
	}
}

Value *eval(Value *expr, Environment *env) {
	Value *operator;
	Value *args;
	switch (expr->val.listValue->head->value->type) {
		case booleanType:
		case integerType:
		case floatType:
		case stringType:
			printf("Returning literal\n");
			return expr;
		case symbolType:
			printf("It's a symbol! : %s\n", expr->val.listValue->head->value->val.symbolValue);
			return envLookup(expr->val.listValue->head->value, env);
		case listType:
			printf("Are you here?\n");
			operator = car(expr);
			args = cdr(expr);
			if (operator->type == idType) {
				NULL;
				/*
				if (!strcmp(operator->val.idValue, "define")) {return evalDefine(args, env);}
				if (!strcmp(operator->val.idValue, "lambda")) {return evalLambda(args, env);}
				if (!strcmp(operator->val.idValue, "let")) {return evalLet(args, env);}
				if (!strcmp(operator->val.idValue, "letrec")) {return evalLetRec(args, env);}
				if (!strcmp(operator->val.idValue, "if")) {return evalIf(args, env);}
				if (!strcmp(operator->val.idValue, "load")) {return evalLoad(args, env);}
				if (!strcmp(operator->val.idValue, "quote")) {return evalQuote(args, env);}
				if (!strcmp(operator->val.idValue, "'")) {return evalQuote(args, env);}
				*/
			} else {
				Value *evaledOperator = eval(operator,env);
				printf("Not a special form\n");
				//printList(args->val.listValue);
				Value *evaledArgs = evalEach(args,env);
				printf("Eval each works\n");
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
		if (f->type == closureType) {
			LinkedList *formalArgs = f->val.closureValue->actualArgs;
			Environment *frame = createFrame(f->val.closureValue->environment);
			return eval(f->val.closureValue->body, frame);
		} else {
			printf("procedure application: expected procedure\n");
		}
	}
}

void printValue(Value* value) {
	if (!value) {
		return;
	}
	switch (value->type) {
		case booleanType:
			if (value->val.boolValue) {
				printf("#t");
			}
			else {
				printf("#f");
			}
			break;			
		case integerType:
			printf("%d", value->val.integerValue);
			break;
			
		case floatType:
			printf("%f", value->val.floatValue);
			break;
		
		case openType:
			printf("%s", value->val.openValue);
			break;
			
		case closeType:
			printf("%s", value->val.closeValue);
			break;
			
		case symbolType:
			printf("%s", value->val.symbolValue);
			break;
			
		case quoteType:
			printf("%s",value->val.quoteValue);
			break;
			
		case stringType:
			printf("%s", value->val.stringValue);
			break;
			
		case listType:
			printParseTree(value->val.listValue->head);
			break;
		
		case closureType:
		case primitiveType:
			printf("#<procedure>");
			break;
		
		default:
			printf("invalid type for value structure\n");
			break;		
	}
}