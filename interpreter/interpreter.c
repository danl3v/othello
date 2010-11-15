#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

/*
 * STATE AND TOKEN TYPE DEFINITIONS
 *
 *
 *
 */

/*add in null*/

enum STATE_TYPE {
	inBetween, inBool, inInteger, inFloat, inString, inSymbol, inPreNumber, inPreFloat, inComment
};

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	pairType, closureType, primitiveType
};

enum SCOPE {
	localScope, globalScope
};

/*
 * MEMORY FUNCTIONS
 *
 *
 *
 */

Value *mallocValue() {
	Value *value = malloc(sizeof(*value));
	return value;
}

Value **mallocValueStarStar() {
	Value **value = malloc(sizeof(**value));
	return value;
}

Pair *mallocPair() {
	Pair *pair = malloc(sizeof(*pair));
	return pair;
}

void freeValue(Value *value) {
	switch (value->type) {
	
		case stringType:	free(value->val.stringValue);					break;
		case symbolType:	free(value->val.symbolValue);					break;
		case openType:		free(value->val.openValue);						break;
	    case closeType:		free(value->val.closeValue);					break;
	    case quoteType:		free(value->val.quoteValue);					break;
		/*case pairType:		freeValue(car(value)); freeValue(cdr(value));	break;
		case closureType:	freeValue(car(value)); freeValue(cdr(value));	break;
		case primitiveType:	freeValue(car(value)); freeValue(cdr(value));	break;*/
		default:															break;
    }
    free(value);
}

/*
 * LIST FUNCTIONS
 *
 *
 *
 */

Value *car(Value *value) {
	if (value && value->type == pairType) {
		return value->val.pairValue->car;
	} else {
		printf("error: cannot take the car of a non-list: ");
		printValue(value);
		printf("\n");
		return NULL;
	}
}

Value *fakeCar(Value *value) {
	return car(car(value));
}

Value *cdr(Value *value) {
	if (value && value->type == pairType) {
		return value->val.pairValue->cdr;
	} else {
		printf("error: cannot take the cdr of a non-list: ");
		printValue(value);
		printf("\n");
		return NULL;
	}
}

Value *fakeCdr(Value *value) {
	return cdr(car(value));
}

Value *cons(Value *value1, Value *value2) {
	Value *result = mallocValue();
	Pair *pair = mallocPair();
	pair->car = value1;
	pair->cdr = value2;
	result->type = pairType;
	result->val.pairValue = pair;
	return result;
}

Value *fakeCons(Value *value) {
	Value *result = mallocValue();
	Pair *pair = mallocPair();
	pair->car = car(value);
	pair->cdr = cdr(value);
	result->type = pairType;
	result->val.pairValue = pair;
	return result;
}

int isProper(Value **value) {
	if (*value) {
		Value *current = *value;
		if (current->type == pairType) {
			while (current) {
				if (cdr(current)) {
					if ((cdr(current))->type != pairType) {
						return 0;
					}
				}
				current = cdr(current);
			}
			return 1;
		}
	}
	return 0;
}

Value **append(Value **value1, Value **value2) {
	if (value1 && *value1 && value2 && *value2) {
		Value *current = *value1;
		while(cdr(current)) {
			current = cdr(current);
		}
		current->val.pairValue->cdr = *value2;
		return value1;
	}
	else if (value1 && *value1) { /* might not need this */
		return value1;
	}
	else if (value2 && *value2) {
		return value2;
	}
	else {
		return NULL;
	}
}

Value **reverse(Value **value) {
	if (*value) {
		Value *current = *value;
		Value *next;
		Value *previous = NULL;
		if (isProper(value)) {
			while (current) {
				next = cdr(current);
				current->val.pairValue->cdr = previous;
				previous = current;
				current = next;
			}
			(*value) = previous;
			return value;
		} else {
			printf("cannot reverse a non list\n");
			return NULL;
		}
	}
	return NULL;
}

/*
 * PRINT FUNCTIONS
 *
 *
 *
 */

void printTokens(Value *value) {
	if (value) {
		switch(value->type) {
			case booleanType:	printf("#%c:boolean\n", value->val.booleanValue?'t':'f');	break;
			case integerType:	printf("%d:integer\n", value->val.integerValue);			break;
			case floatType:		printf("%f:float\n", value->val.floatValue);				break;
			case stringType:	printf("%s:string\n", value->val.stringValue);				break;	
			case symbolType:	printf("%s:symbol\n", value->val.symbolValue);				break;	
			case openType:		printf("%s:open\n", value->val.openValue);					break;	
			case closeType:		printf("%s:close\n", value->val.closeValue);				break;	
			case quoteType:		printf("%s:quote\n", value->val.quoteValue);				break;
			case pairType:		printTokens(car(value)); printTokens(cdr(value));			break;
			default:			printf("in printTokens: i don't know what type of value i am");				break;
		}
	}
}

void printParseTree(Value *value) {
	printValueHelper(value);
}

void printEvaluation(Value *value) { /* do we want this to take in a * or **? */
	Value *current = value;
	while (current) {
		printValueHelper(car(current));
		printf("\n");
		current = cdr(current);
	}
}

void printValue(Value *value) {
	if (value) {
		if (value->type == pairType) {
			printf("(");
			printValueHelper(value);
			printf(")");
		} else {
			printValueHelper(value);
		}
	}
}

void printValueHelper(Value *value) {
	if (value) {
		switch (value->type) {
			case booleanType:	printf("#%c", value->val.booleanValue?'t':'f');				break;
			case integerType:	printf("%d", value->val.integerValue);						break;
			case floatType:		printf("%f", value->val.floatValue);						break;
			case openType:		printf("%s--", value->val.openValue);						break;
			case closeType:		printf("%s--", value->val.closeValue);						break;
			case symbolType:	printf("%s", value->val.symbolValue);						break;
			case quoteType:		printf("%s", value->val.quoteValue);						break;
			case stringType:	printf("%s", value->val.stringValue);						break;
			case pairType:
				if (car(value)) {
					if ((car(value))->type == pairType) { printValue(car(value)); }
					else { printValueHelper(car(value)); }
				}
				else {
					printf("()");
				}
				if (cdr(value)) {
					printf(" ");
					if ((cdr(value))->type == pairType) { printValueHelper(cdr(value)); }
					else { printf(". "); printValueHelper(cdr(value)); }
				}
				break;
			case closureType:	printf("#<closure>");																break;
			case primitiveType:	printf("#<procedure> (add a label)"); 												break;
			default:			printf("in printValueHelper: i don't know what type of value i am");				break;		
		}
	}
}

/*
 * TOKENIZER
 *
 *
 *
 */
 
char *substr(char *string, int start, int end) { /* copies from string from index start to end-1 */ 
	int i;
	char *result = malloc(sizeof(char)*(end - start + 1));
	for (i=0;i<end-start;i++) {
		result[i] = string[start+i];
	}
	result[end-start] = 0;
	return result;
}

int consToken(Value **tokenList, int type, char *string, int lineNumber) {
	Value *token = mallocValue();
	char * str;
	
	if (token) {
		token->type = type;
		token->lineNumber = lineNumber;
		switch (type) {
			case booleanType:	token->val.booleanValue = (!strcmp(string,"T") || !strcmp(string,"t")); free(string);	break;
			case integerType:	token->val.integerValue = strtol(string, &str, 0); free(string);						break;
			case floatType:		token->val.floatValue = strtod(string, &str); free(string);								break;
			case stringType:	token->val.stringValue = string;														break;
			case symbolType:	token->val.symbolValue = string;														break;
			case openType:		token->val.openValue = string;															break;
			case closeType:		token->val.closeValue = string;															break;
			case quoteType:		token->val.quoteValue = string;															break;
		}
		*tokenList = cons(token, (*tokenList));
	}
	return 0;
}


Value **tokenize (char *expression) {
	int lineNumber = 1;
	int currentState = inBetween;
	int tokenStartIndex = 0;
	int tokenCurrentIndex = 0;
	Value **tokenList = mallocValueStarStar();
	if (!(*expression) || (!strcmp(expression, "\n"))) {
		return tokenList;
	}
	while (expression[tokenCurrentIndex]) {
		switch (currentState) {
			case inBetween:
				tokenStartIndex = tokenCurrentIndex;
				switch (expression[tokenCurrentIndex]) {
					case '#':
						currentState = inBool;
						break;
						
					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						break;
					
					case '(':
					case '[':
						consToken(tokenList, openType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
						break;
						
					case ')':
					case ']':
						consToken(tokenList, closeType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
						break;
						
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						currentState = inInteger;
						break;
						
					case '+':
					case '-':
						currentState = inPreNumber;
						break;
						
					case '.':
						currentState = inFloat;
						break;
						
					case '"':
						currentState = inString;
						break;
						
					case ';':
						currentState = inComment;
						break;
						
					case '\'':
						consToken(tokenList, quoteType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
						break;
					
					case '\\':
						tokenCurrentIndex++;
						currentState = inSymbol;
						break;
						
					default:
						currentState = inSymbol;
						break;
				}
			break;
				
			case inBool:
				switch (expression[tokenCurrentIndex]) {
					case 't':					
					case 'T':
					case 'f':
					case 'F':
						consToken(tokenList, booleanType, substr(expression, tokenStartIndex+1, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					default: /* maybe transition to inbetween state? */
						printf("error - no t or f after #\n");
						/* destroy(tokenList); */
						return NULL;
				}
			break;
			
			case inPreNumber:
				switch (expression[tokenCurrentIndex]) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						currentState = inInteger;
						break;
						
					case '.':
						currentState = inPreFloat;
						break;
					
					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '(':
					case '[':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '"':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
					
					case '\'':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inComment;
						break;
					
					case '\\':
						tokenCurrentIndex++;
						currentState = inSymbol;
						break;
						
					default:
						currentState=inSymbol;
						break;
				}
			break;
			
			case inPreFloat:
				switch (expression[tokenCurrentIndex]) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						currentState = inFloat;
						break;
						
					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
					
					case '(':
					case '[':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
					
					case '"':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inComment;
						break;

					case '\\':
						tokenCurrentIndex++;
						currentState = inSymbol;
						break;
						
					default:
						currentState = inSymbol;
						break;
				}
			break;	
			
			case inInteger:
				switch (expression[tokenCurrentIndex]) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
						
					case '.':
						currentState = inFloat;
						break;

					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						consToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
					
					case '(':
					case '[':
						consToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						consToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '"':
						consToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						consToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						consToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inComment;
						break;
						
					case '\\':
						tokenCurrentIndex++;
						currentState = inSymbol;
						break;
						
					default:
						currentState = inSymbol;
						break;
				}
			break;
			
			case inFloat:
				switch (expression[tokenCurrentIndex]) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
						
					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						consToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
					
					case '(':
					case '[':
						consToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						consToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
					
					case '"':
						consToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						consToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						consToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inComment;
						break;
						
					case '\\':
						tokenCurrentIndex++;
						currentState = inSymbol;
						break;
						
					default:
						currentState = inSymbol;
						break;
				}
			break;
			
			case inSymbol:
				switch (expression[tokenCurrentIndex]) {
					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
						
					case '(':
					case '[':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
							
					case ')':
					case ']':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '"':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						consToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						consToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inComment;
						break;
				
					case '\\':
						tokenCurrentIndex++;
						break;
					
					default:
						break;
				}
			break;
			
			case inString:
				switch (expression[tokenCurrentIndex]) {
					case '\n':
					case '\r':
						lineNumber++;
						break;
						
					case '"':
						consToken(tokenList, stringType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
					
					case '\\':
						tokenCurrentIndex++;
						break;
					
					default:
						break;			
				}
			break;
			
			case inComment:
				switch (expression[tokenCurrentIndex]) {
					case '\n':
					case '\r':
						currentState = inBetween;
						lineNumber++;
						break;
						
					default:
						break;
				}
			break;
			
			default:
				printf("error - current state error\n");
				/* destroy(tokenList); */
				return NULL;
		}
		
		tokenCurrentIndex++;
	}
	
	if (currentState == inString) {
		printf("error - unterminated string\n");
		/* destroy(tokenList); */
		return NULL;
	}
	
	return reverse(tokenList);
	
}


/*
 * PARSER
 *
 *
 *
 */

Value **parse(Value **tokenList, int* depth) {
	Value **parseTree = mallocValueStarStar();
	Value *current;
	if (tokenList) { /* check to see if the tokenList exists */
		current = *tokenList;
		while (current) { /* start to iterate through the tokenList */
			if ((car(current))->type == openType) { /* if we see an open paren, push onto stack and increment depth */
				(*parseTree) = cons(car(current), *parseTree);
				(*depth)++;
			}
			else if ((car(current))->type == closeType) { /* if we see a close type, do something special */
				Value *subTree = NULL;
				if (!(*parseTree)) { printf("error: too many close parens"); return NULL; }
				while (!(car(*parseTree) && car(*parseTree)->type == openType)) {
					subTree = cons(car(*parseTree), subTree);
					if (!(*parseTree) || !(cdr(*parseTree))) { printf("error: too many close parens"); return NULL; }
					*parseTree = cdr(*parseTree);
				}
				*parseTree = cdr(*parseTree); /* pop off the open type */
				(*parseTree) = cons(subTree, *parseTree); /* push the subtree onto the parseTree */
				(*depth)--; /* decrease the depth */
			}
			/*else if ((car(current))->type == quoteType) {
				Value **parseOne = parseQuote(current);
				(*parseTree) = cons(*parseOne, *parseTree);
			}*/
			else { /* if regular old token, just push onto the tree */
				(*parseTree) = cons(car(current), *parseTree);
			}
			current = cdr(current); /* move to the next token in the tokenList */
		}
		return reverse(parseTree);
	}
	else { /* return NULL if the tokenList does not exist */
		return NULL;
	}
}

/*
 * EVALUATOR
 *
 *
 *
 */

Value *add(Value *args) {
	Value *result = mallocValue();
	Value *current = args;
	result->type = integerType;
	result->val.integerValue = 0;
	if (!args) {
		return result;
	}
	if (args->type == pairType) {
		while (current) { /* check our error checking. maybe not so good? (+) should be 0 */
			switch (car(current)->type) {
				case integerType:
					if (result->type == integerType) {
						result->val.integerValue = result->val.integerValue + car(current)->val.integerValue;
					}
					else {
						result->val.floatValue = result->val.floatValue + car(current)->val.integerValue;
					}
					break;
					
				case floatType:
					if (result->type == integerType) {
						result->val.floatValue = result->val.integerValue;
						result->type = floatType;
					}
					result->val.floatValue = result->val.floatValue + car(current)->val.floatValue;
					break;
					
				default:
					printf("Error: Attempted to add non-number\n");
					free(result);
					return NULL;
			}
			current = cdr(current);
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
	Value *result = mallocValue();
	Value *current = args;
	if (!current) {
		printf("-: expects at least 1 argument, given 0\n");
		free(result);
		return NULL;
	}
	if (args->type == pairType) {
		if (cdr(current)) {
			switch (car(current)->type) {
				case integerType:
					result->type = integerType;
					result->val.integerValue = car(current)->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val.floatValue = car(current)->val.floatValue;
					break;
				default:
					printf("Error: Attempted to subtract non-number\n");
					free(result);
					return NULL;
			}
			current = cdr(current);
			while (current) { /* check our error checking. maybe not so good? */
				switch (car(current)->type) {
					case integerType:
						if (result->type == integerType) {
							result->val.integerValue = result->val.integerValue - car(current)->val.integerValue;
						}
						else {
							result->val.floatValue = result->val.floatValue - car(current)->val.integerValue;
						}
						break;
						
					case floatType:
						if (result->type == integerType) {
							result->val.floatValue = result->val.integerValue;
							result->type = floatType;
						}
						result->val.floatValue = result->val.floatValue - car(current)->val.floatValue;
						break;
						
					default:
						printf("Error: Attempted to subtract non-number\n");
						free(result);
						return NULL;
				}
				current = cdr(current);
			}
		} else {
			switch (car(current)->type) {
				case integerType:
					result->type = integerType;
					result->val.integerValue = -1 * car(current)->val.integerValue;
					break;
				case floatType:
					result->type = floatType;
					result->val.floatValue = -1 * car(current)->val.floatValue;
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
	Value *result = mallocValue();
	Value *current = args;
	result->type = integerType;
	result->val.integerValue = 1;
	if (!args) {
		return result;
	}
	if (args->type == pairType) {
		while (current) { 
			switch (car(current)->type) {
				case integerType:
					if (result->type == integerType) {
						result->val.integerValue = result->val.integerValue * car(current)->val.integerValue;
					}
					else {
						result->val.floatValue = result->val.floatValue * car(current)->val.integerValue;
					}
					break;
					
				case floatType:
					if (result->type == integerType) {
						result->val.floatValue = result->val.integerValue;
						result->type = floatType;
					}
					result->val.floatValue = result->val.floatValue * car(current)->val.floatValue;
					break;
					
				default:
					printf("Error: Attempted to multiply non-number\n");
					free(result);
					return NULL;
			}
			current = cdr(current);
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
	Value *result = mallocValue();
	Value *current = args;
	if (!current) {
		printf("/: expects at least 1 argument, given 0\n");
		freeValue(result);
		return NULL;
	}
	/* Here we check for if args is one item long, and if first item is non-number */
	if (car(current)->type == integerType) {
		if (!cdr(current)) {
			if (car(current)->val.integerValue == 0) {
				printf("/: division by zero\n");
				freeValue(result);
				return NULL;
			}
			else {
				result->type = integerType;
				result->val.integerValue = 1 / car(current)->val.integerValue;
				return result;
			}
		}
	}
	else if (car(current)->type == floatType) {
		if (!cdr(current)) {
			if (car(current)->val.floatValue == 0) {
				printf("/: division by zero\n");
				freeValue(result);
				return NULL;
			}
			else {
				result->type = floatType;
				result->val.floatValue = 1 / car(current)->val.floatValue;
				return result;
			}
		}
	}
	else {
		printf("Error: Attempted to divide non-number\n");
		freeValue(result);
		return NULL;
	}
	/* Here we set the type of the result based upon the first arg */
	result->type = integerType;
	if (car(current)->type == floatType) { result->type = floatType; }
	/* starting with the second item, we check for division by zero */
	current = cdr(current);
	while (current) {
		if (car(current)->type == floatType) {
			result->type = floatType;
			if (car(current)->val.floatValue == 0) {
				printf("/: division by zero\n");
				freeValue(result);
				return NULL;
			}
		}
		else if (car(current)->type == integerType) {
			if (car(current)->val.integerValue == 0) {
				printf("/: division by zero\n");
				freeValue(result);
				return NULL;
			}
		}
		else {
			printf("Error: Attempted to divide non-number\n");
			freeValue(result);
			return NULL;
		}
		current = cdr(current);
	}
	/* Here we set the value of the result based upon the first item*/
	current = args;
	if (result->type == integerType) { 
		result->val.integerValue = car(current)->val.integerValue; 
	}
	else {
		if (car(current)->type == integerType) {
			result->val.floatValue = (float)(car(current)->val.integerValue);
		}
		else {
			result->val.floatValue = (car(current)->val.floatValue);
		}
	}
	/* starting again with the second item, we divide */
	current = cdr(current);
	while (current) {
		if (result->type == integerType) {
			result->val.integerValue = result->val.integerValue / car(current)->val.integerValue;
		}
		else {
			if (car(current)->type == integerType) { result->val.floatValue = result->val.floatValue / car(current)->val.integerValue; }
			else { result->val.floatValue = result->val.floatValue / car(current)->val.floatValue; }
		}
		current = cdr(current);
	}
	return result;
}

Value *evalQuote(Value *args) {
	if (cdr(args)) {
		printf("error: quote: bad syntax (wrong number of parts)" );
		return NULL;
	}
	else {
		return car(args);
	}
}

Value *makePrimitiveValue(Value* (*f)(Value *)){
	Value *resultValue = mallocValue();
	resultValue->type = primitiveType;
	resultValue->val.primitiveValue = f;
	return resultValue;
}

Environment* createTopFrame() {
	Environment *topFrame = createFrame(NULL);
	bind("+", makePrimitiveValue(add), topFrame);
	bind("-", makePrimitiveValue(subtract), topFrame);
	bind("*", makePrimitiveValue(multiply), topFrame);
	bind("/", makePrimitiveValue(divide), topFrame);
	bind("car", makePrimitiveValue(fakeCar), topFrame);
	bind("cdr", makePrimitiveValue(fakeCdr), topFrame);
	bind("cons", makePrimitiveValue(fakeCons), topFrame);
	return topFrame;
}

Environment* createFrame(Environment *parent) {
	Environment *frame = malloc(sizeof(*frame));
	frame->parentFrame = parent;
	frame->bindings = malloc(sizeof(frame->bindings));
	return frame;
}

Value *environmentLookup(char *symbol, Environment *environment, int global) {
	Value *current;
	if (!environment) {
		if (global) { printf("error: variable not found\n"); }
		return NULL;
	}
	
	current = *(environment->bindings);
	while (current) {
		if (!strcmp((car(car(current)))->val.symbolValue, symbol)) {
			return cdr(car(current));
		}
		current = cdr(current);
	}
	
	if (global) {
		return environmentLookup(symbol, environment->parentFrame, global);
	}
	else {
		return environmentLookup(symbol, NULL, global); /* maybe this is dumb, uses extra memory */
	}
}

void bind(char *symbol, Value *value, Environment *environment) {
	Value *v = environmentLookup(symbol, environment, localScope);
	if (v && value) {
		v->type = value->type;
		v->val = value->val;
	}
	else if (value) {
		Value *symbolValue;
		Value *binding = mallocValue();
		binding->type = pairType;
		symbolValue = mallocValue();
		symbolValue->type = symbolType;
		symbolValue->val.symbolValue = symbol;
		binding->val.pairValue = mallocPair();
		binding->val.pairValue->car = symbolValue;
		binding->val.pairValue->cdr = value;
		*(environment->bindings) = cons(binding, *(environment->bindings));
	}
	else {
		printf("dont wanna seg fault! - in bind\n");
	}
}

Value **evaluate(Value **parseTree, Environment *environment) {
	return evalTop(parseTree, environment);
}

Value **evalTop(Value **tree, Environment *environment) {
	Value **evaluated = mallocValueStarStar();
	Value *valueStar = NULL;
	Value *current = *tree;
	while (current && car(current)) {
		if (!(*evaluated)) {
			*evaluated = cons(eval(car(current), environment), valueStar);
		} else {
			*evaluated = cons(eval(car(current), environment), *evaluated);
		}
		current = cdr(current);
	}
	if (!(*evaluated)) {
		*evaluated = NULL;
		return evaluated;
	}
	return reverse(evaluated);
}

Value **evalEach(Value **tree, Environment *environment) {
	Value **evaluated = mallocValueStarStar();
	Value *current = *tree;
	while (current && car(current)) {
		*evaluated = cons(eval(car(current), environment), *evaluated);
		current = cdr(current);
	}
	if (!(*evaluated)) {
		*evaluated = NULL;
		return evaluated;
	}
	return reverse(evaluated);
}

Value *eval(Value *value, Environment *environment) {
	Value *operator;
	Value **args = mallocValueStarStar();
	Value *v;
	switch (value->type) {
		case booleanType:
		case integerType:
		case floatType:
		case stringType:
			return value;
		case symbolType:
			v = environmentLookup(value->val.symbolValue, environment, globalScope);
			if (v) {
				return v;
			} else {
				printf("reference to undefined identifier: '%s'\n", value->val.symbolValue);
				return NULL;
			}
		case pairType:
			operator = car(value);
			*args = cdr(value);
			if (operator->type == symbolType) {
				Value *evaledOperator;
				Value **evaledArgs;
				if (!strcmp(operator->val.symbolValue, "quote")) {return evalQuote(*args);}
				/*
				if (!strcmp(operator->val.symbolValue, "define")) { return evalDefine(args, environment); }
				if (!strcmp(operator->val.symbolValue, "lambda")) { return evalLambda(args, environment); }
				if (!strcmp(operator->val.symbolValue, "let")) {return evalLet(args, environment);}
				if (!strcmp(operator->val.symbolValue, "letrec")) {return evalLetRec(args, environment);}
				if (!strcmp(operator->val.symbolValue, "if")) {return evalIf(args, environment);}
				if (!strcmp(operator->val.symbolValue, "load")) {return evalLoad(args, environment);}
				if (!strcmp(operator->val.symbolValue, "'")) {return evalQuote(args, environment);}
				*/
				evaledOperator = eval(operator, environment);
				evaledArgs = evalEach(args, environment);
				return apply(evaledOperator, evaledArgs);
			}
			else if (operator->type == closureType || operator->type == primitiveType || operator->type == pairType) {
				Value *evaledOperator = eval(operator, environment);
				Value **evaledArgs = evalEach(args, environment);
				printf("WHAT IN THE WORLD ARE WE DOING HERE\n");
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
		default:
			printf("wrong type\n");
			return NULL;
	}
}

Value *apply(Value *f, Value **actualArgs) {
	if (f->type == primitiveType) {
		return f->val.primitiveValue(*actualArgs);
	} else {
		if (f->type == closureType) {
			Environment *frame = createFrame(f->val.closureValue->environment);
			Value *currentFormalArg = car(*(f->val.closureValue->formalArgs));
			Value *currentActualArg = car(*actualArgs);
			while (currentFormalArg && currentActualArg) {
				bind(currentFormalArg->val.symbolValue, currentActualArg, frame);
				currentFormalArg = cdr(currentFormalArg);
				currentActualArg = cdr(currentActualArg);
			}
			return eval(*(f->val.closureValue->body), frame);
		} else {
			printf("procedure application: expected procedure\n");
			return NULL;
		}
	}
}