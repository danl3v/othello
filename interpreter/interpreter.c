#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

//add in null

enum STATE_TYPE {
	inBetween, inBool, inInteger, inFloat, inString, inSymbol, inPreNumber, inPreFloat, inComment
};

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	pairType, closureType, primitiveType
};

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

Value *car(Value *value) {
	if (value->type == pairType) {
		return value->val.pairValue->car;
	} else {
		return NULL;
	}
}

Value *cdr(Value *value) {
	if (value->type == pairType) {
		return value->val.pairValue->cdr;
	} else {
		return NULL;
	}
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
}
void printTokens(Value *value) {
	if (value) {
		Value *current = value;
		switch(current->type) {
			case booleanType:
				if (current->val.booleanValue) {
					printf("#t:boolean\n");
				}
				else {
					printf("#f:boolean\n");
				}
				break;
				
			case integerType:
				printf("%d:integer\n", current->val.integerValue);
				break;
				
			case floatType:
				printf("%f:float\n", current->val.floatValue);
				break;
			case stringType:
				printf("%s:string\n", current->val.stringValue);
				break;
				
			case symbolType:
				printf("%s:symbol\n", current->val.symbolValue);
				break;
				
			case openType:
				printf("%s:open\n", current->val.openValue);
				break;
				
			case closeType:
				printf("%s:close\n", current->val.closeValue);
				break;
				
			case quoteType:
				printf("%s:quote\n", current->val.quoteValue);
				break;
			
			case pairType:
				printTokens(car(current));
				printTokens(cdr(current));
				break;
			
			default:
				printf("invalid type for value structure");
				break;
	   }
	}
}

void printValue(Value *value) {
	if (value) {
		if (value->type == pairType) {
			printf("(");
			printValueHelper(value);
			printf(")\n");
		} else {
			printValueHelper(value);
			printf("\n");
		}
	}
}

void printValueHelper(Value *value) {
	if (value) {
		switch (value->type) {
			case booleanType:
				if (value->val.booleanValue) {
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
				printf("%s", value->val.quoteValue);
				break;
				
			case stringType:
				printf("%s", value->val.stringValue);
				break;
				
			case pairType:
				printf("in pair type\n");
				if ((car(value))->type == pairType) {
				printf("in pair type2\n");
					printValue(car(value));
				} else {
				printf("in pair type3\n");
					printValueHelper(car(value));
				}
				printf("in pair type55\n");
				printf("type: %d\n", value->type);
				if (cdr(value)) {
					printf(" ");
					if ((cdr(value))->type == pairType) {
						printValueHelper(cdr(value));
					} else {
						printf(". ");
						printValueHelper(cdr(value));
					}
				}
				break;
	
			case closureType:
				printf("#<closure>");
				break;
				
			case primitiveType:
				printf("#<procedure>"); // maybe have some sort of label
				break;
			
			default:
				printf("i don't know what type of value i am");
				break;		
		}
		printf("type: %d\n", value->type);
	}
}

char *substr(char *string, int start, int end) { // copies from string from index start to end-1   
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
			case booleanType:
			    token->val.booleanValue = (!strcmp(string,"T") || !strcmp(string,"t"));
				free(string);
				break;
			case integerType:
				token->val.integerValue = strtol(string, &str, 0);
				free(string);
				break;
			case floatType:
				token->val.floatValue = strtod(string, &str);
				free(string);
				break;
			case stringType:
				token->val.stringValue = string;
				break;
			case symbolType:
				token->val.symbolValue = string;
				break;
			case openType:
				token->val.openValue = string;
				break;
			case closeType:
				token->val.closeValue = string;
				break;
			case quoteType:
				token->val.quoteValue = string;
				break;
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
	Value *tokenListTemp = NULL;
	Value **tokenList = mallocValueStarStar();
	tokenList = &(tokenListTemp);
	
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
						
					default: // maybe transition to inbetween state?
						printf("error - no t or f after #\n");
						//destroy(tokenList);
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
						consToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
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
				//destroy(tokenList);
				return NULL;
		}
		
		tokenCurrentIndex++;
	}
	
	if (currentState == inString) {
		printf("error - unterminated string\n");
		//destroy(tokenList);
		return NULL;
	}
	
	return reverse(tokenList);
	
}