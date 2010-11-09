#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evaluator.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	listType, closureType, primitiveType
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
	if (args->type == listType) {
		if (args->val.listValue->head) {
			if (args->val.listValue->head->next) {
				Value *newValue = malloc(sizeof(*newValue));
				newValue->type = listType;
				newValue->val.listValue = malloc(sizeof(LinkedList));
				create(newValue->val.listValue); // i think we don't need this
				newValue->val.listValue->head = args->val.listValue->head->next;
				newValue->val.listValue->tail = args->val.listValue->tail;
				return newValue;
			}
			printf("cdr returning null");
			return NULL;
		}
		printf("Error: cdr of empty list\n");
		return NULL;
	}
	printf("Error: Cannot return cdr of a non-list\n");
	return NULL;
}

Value *envLookup(char *symbol, Environment *environment) {
	printf("begin evnironment lookup on %s\n", symbol);
	if (!environment) {
		printf("No environment\n");
	}
	Node *current = environment->bindings->head;
	printf("next evnironment lookup\n");
	while (current) {
		printf("looping in envLookup\n");
		//printf("In while\n");
		//printf("binding: %s\n", current->value->val.listValue->head->value->val.symbolValue);
		//printf("lookup: %s\n", symbol->val.symbolValue);
		if (!strcmp(current->value->val.listValue->head->value->val.symbolValue, symbol)) {
			return current->value->val.listValue->head->next->value;
			printf("found!");
		}
		current = current->next;
	}
	if (environment->parentFrame) {
		printf("going to higher frame\n");
		return envLookup(symbol, environment->parentFrame);
	} else {
		printf("not found :_(\n");
		return NULL;
	}
}

Value *localEnvLookup(char *symbol, Environment *environment) {
	Node *current;
	current = environment->bindings->head;
	while (current) {
		//printf("In while\n");
		//printf("binding: %s\n", current->value->val.listValue->head->value->val.symbolValue);
		//printf("lookup: %s\n", symbol->val.symbolValue);
		if (!strcmp(current->value->val.listValue->head->value->val.symbolValue, symbol)) {
			return current->value->val.listValue->head->next->value;
			printf("found!");
		}
		current = current->next;
	}
	return NULL;
}

void bind(char *symbol, Value *value, Environment *environment){
	printf("binding in progress\n");
	Value *v = localEnvLookup(symbol, environment);
	printf("checked if variable already bound\n");
	if (v) {
		v->type = value->type;
		v->val = value->val;
	} else {
		Value *binding = malloc(sizeof(*binding));
		binding->type = listType;
		binding->val.listValue = malloc(sizeof(binding->val.listValue));
		create(binding->val.listValue);
		Value *symbolValue = malloc(sizeof(*symbolValue));
		symbolValue->type = symbolType;
		symbolValue->val.symbolValue = symbol;
		push(binding->val.listValue, value);
		push(binding->val.listValue, symbolValue);
		push(environment->bindings, binding);
	}
	printf("bound\n");
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

Value *evalEach(Value *args, Environment *env) {
	Value *evaluated = malloc(sizeof(*evaluated));
	evaluated->type = listType;
	evaluated->val.listValue = malloc(sizeof(evaluated->val.listValue));
	create(evaluated->val.listValue);
	if (args) {
		if (args->type == listType) {
			Node *current = args->val.listValue->head;
			while (current) {
				Value *v = eval(current->value, env);
				if (v) {
					push(evaluated->val.listValue, v);
					current = current->next;
				} else {
					printf("error in eval each, could not evaluate the argument!!!!");
					return NULL;
				}
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
	else { // if there are no args, we want to return an empty list
		return evaluated;
	}
}

void evalAll(Value *expr, Environment *env) {
	Node *current = malloc(sizeof(*current)); // are you sure we need to malloc here
	current = expr->val.listValue->head;
	while (current) {
		Value *evaluated = eval(current->value, env);
		printValue(evaluated);
		//printf("\n");
		current = current->next;
	}
}

Value *eval(Value *expr, Environment *env) {
	Value *operator;
	Value *args;
	Value *v;
	switch (expr->type) { //->val.listValue->head->value->type
		case booleanType:
		case integerType:
		case floatType:
		case stringType:
			return expr;
		case symbolType:
			v = envLookup(expr->val.symbolValue, env);
			if (v) {
				return v;
			} else {
				printf("reference to undefined identifier: '%s'\n", expr->val.symbolValue);
				return NULL;
			}
		case listType:
			operator = car(expr);
			args = cdr(expr);
			if (operator->type == symbolType) {
				if (!strcmp(operator->val.symbolValue, "define")) { return evalDefine(args, env); }
				if (!strcmp(operator->val.symbolValue, "lambda")) { return evalLambda(args, env); }
				/*
				if (!strcmp(operator->val.symbolValue, "let")) {return evalLet(args, env);}
				if (!strcmp(operator->val.symbolValue, "letrec")) {return evalLetRec(args, env);}
				if (!strcmp(operator->val.symbolValue, "if")) {return evalIf(args, env);}
				if (!strcmp(operator->val.symbolValue, "load")) {return evalLoad(args, env);}
				if (!strcmp(operator->val.symbolValue, "quote")) {return evalQuote(args, env);}
				if (!strcmp(operator->val.symbolValue, "'")) {return evalQuote(args, env);}
				*/
				printf("Evaluating Operator: ");
				printValue(operator);
				Value *evaledOperator = eval(operator, env);
				printf("Operator Evaluated: ");
				printValue(evaledOperator);
				printf("evaluating argmuents\n");
				Value *evaledArgs = evalEach(args,env);
				printf("evaluated arguments\n");
				return apply(evaledOperator, evaledArgs);
			}
			else if (operator->type == closureType || operator->type == primitiveType || operator->type == listType) {
				printf("harro");
				Value *evaledOperator = eval(operator, env);
				Value *evaledArgs = evalEach(args,env);
				if (evaledArgs) {
					return apply(evaledOperator, evaledArgs);
				} else {
					printf("procedure application: expected procedure, given: ");
					printValue(operator);
					return NULL;
				}
			}
			else {
				printf("procedure application: expected procedure, given2: ");
				printValue(operator);
				return NULL;
			}
			
		case closureType:
		case primitiveType:
			return expr;
		/*
			else if (operator->type == closureType || operator->type == primitiveType) {
				printf("harro");
				Value *evaledArgs = evalEach(args,env);
				if (evaledArgs) {
					return apply(operator, evaledArgs);
				}
			} else if (operator->type == listType) {
				Value *evaledOperator = eval(operator, env);
				Value *evaledArgs = evalEach(args,env);
				if (evaledArgs && 
					(evaledOperator->type == closureType || operator->type == primitiveType)) {
					return apply(evaledOperator, evaledArgs);
				} else {
					printf("procedure application: expected procedure, given: ");
					printValue(operator);
					return NULL;
				}
			} else {
				printf("procedure application: expected procedure, given: ");
				printValue(operator);
				return NULL;
			}
			*/
		default:
			printf("eval returning null\n");
			return NULL;
	}
}

Value *apply(Value *f, Value *actualArgs) {
	if (f->type == primitiveType) {
		printf("applying primitive\n");
		printValue(actualArgs);
		printValue(f);
		return f->val.primitiveValue(actualArgs);
	} else {
		if (f->type == closureType) {
			printf("applying closure: creating enviornment\n");
			Environment *frame = createFrame(f->val.closureValue->environment);
			printf("applying closure: getting formal argument\n");
			Node *currentFA = f->val.closureValue->formalArgs->val.listValue->head;
			printf("applying closure: getting actual arg\n");
			Node *currentAA = actualArgs->val.listValue->head;
			printf("applying closure: beginning the loop\n");
			while (currentFA && currentAA) {
				printf("looping\n");
				printValue(currentFA->value);
				printValue(currentAA->value);
				bind(currentFA->value->val.symbolValue, currentAA->value, frame);
				currentFA = currentFA->next;
				currentAA = currentAA->next;
			}
			
			printf("printing body");
			printValue(f->val.closureValue->body);
			printf("returning from closure application\n");
			printf("the body is of type: %d\n", f->val.closureValue->body->type);
			return eval(f->val.closureValue->body, frame);
		} else {
			printf("procedure application: expected procedure\n");
		}
	}
}

Value *evalDefine(Value *args, Environment *environment) {
	while (environment->parentFrame) {
		environment = environment->parentFrame;
	}
	Value *v = eval(args->val.listValue->head->next->value, environment);
	bind(args->val.listValue->head->value->val.symbolValue, v, environment);
	Value *howdyDoodyValue = malloc(sizeof(*howdyDoodyValue));
	howdyDoodyValue->type = stringType;
	howdyDoodyValue->val.stringValue = "Howdy Doody";
	return howdyDoodyValue;
}

Value *evalLambda(Value *args, Environment *environment) {
	Value *value = malloc(sizeof(value));
	value->val.closureValue = malloc(sizeof(Closure));
	value->type = closureType;
	value->val.closureValue->formalArgs = car(args);
	Value *tempBody = cdr(args);
	tempBody->val = tempBody->val.listValue->head->value->val;
	tempBody->type = tempBody->val.listValue->head->value->type;
	value->val.closureValue->body = tempBody;
	printf("the body is of type: %d\n", value->val.closureValue->body->type);
	printf("list type: %d\n", listType);
	value->val.closureValue->environment = environment;
	//printValue(args);
	return value;
}

void printValue(Value* value) {
	if (!value) {
		return;
	}
	switch (value->type) {
		case booleanType:
			if (value->val.boolValue) {
				printf("#t\n");
			}
			else {
				printf("#f\n");
			}
			break;			
		case integerType:
			printf("%d\n", value->val.integerValue);
			break;
			
		case floatType:
			printf("%f\n", value->val.floatValue);
			break;
		
		case openType:
			printf("%s\n", value->val.openValue);
			break;
			
		case closeType:
			printf("%s\n", value->val.closeValue);
			break;
			
		case symbolType:
			printf("%s\n", value->val.symbolValue);
			break;
			
		case quoteType:
			printf("%s\n",value->val.quoteValue);
			break;
			
		case stringType:
			printf("%s\n", value->val.stringValue);
			break;
			
		case listType:
			//printf("it is a list\n");
			printParseTree(value->val.listValue->head);
			printf("\n");
			break;
		
		case closureType:
			printf("#<closure>\n");
			break;
		case primitiveType:
			printf("#<procedure>\n"); // maybe have some sort of label
			break;
		
		default:
			printf("invalid type for value structure\n");
			break;		
	}
}