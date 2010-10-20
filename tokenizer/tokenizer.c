#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType
};

enum STATE_TYPE {
	inBetween, inBool, inInteger, inFloat, inString, inSymbol, inPreNumber, inEscaped, inComment
};

char *substr(char *string, int start, int end) {
	char *result = malloc(sizeof(char)*(end - start + 1));
	strncpy(result, &string[start], end-start);
	return result;
}

// write a freeValue function


// have a lst of malloced values and then free all that at the end
// freeing values after we hit enter
// in destroy we want to free the values

void freeValue(Value *value) {
	switch (value->type) {
	
		case stringType:
		   free(value->val.stringValue);
		   break;
		   
		case symbolType:
		   free(value->val.symbolValue);
		   break;
		   
		case openType:
		   free(value->val.openValue);
		   break;
		   
	    case closeType:
		   free(value->val.closeValue);
		   break;
		   
	    case quoteType:
		   free(value->val.quoteValue);
		   break;
    }
    free(value);
}

int pushToken(LinkedList *tokenList, int type, char *string) {
	Value *token = malloc(sizeof(Value));
	// add value to a list of malloc'ed values to free in destroy
	char * str;
	char * pEnd; // can we just use * str????
	
	if (token) {
		token->type = type;
		switch (type) {
			case booleanType:
			    token->val.boolValue = (!strcmp(string,"T") || !strcmp(string,"t"));
				free(string);
				break;
			case integerType:
				token->val.integerValue = strtol(string, &str, 0);
				free(string);
				break;
			case floatType:
				token->val.floatValue = strtod(string, &pEnd);
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
		push(tokenList, token);
	}
	return 0;
}

LinkedList *tokenize (char *expression) {
	int currentState = inBetween;
	int tokenStartIndex = 0;
	int tokenCurrentIndex = 0;
	Value *token;
	char* token_value;
	LinkedList *tokenList = malloc(sizeof(*tokenList));
	create(tokenList);

	while (expression[tokenCurrentIndex]) {
		switch (currentState) {
		
			case inBetween:
				tokenStartIndex = tokenCurrentIndex;
				switch (expression[tokenCurrentIndex]) {
					case '#':
						currentState = inBool;
						break;
						
					case ' ':
					case '\t':
					case '\n':
						break;
					
					case '(':
						pushToken(tokenList, openType, substr(expression, tokenStartIndex, tokenCurrentIndex+1));
						break;
						
					case ')':
						pushToken(tokenList, closeType, substr(expression, tokenStartIndex, tokenCurrentIndex+1));
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
						pushToken(tokenList, quoteType, substr(expression, tokenStartIndex, tokenCurrentIndex+1));
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
						pushToken(tokenList, booleanType, substr(expression, tokenStartIndex+1, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					default: // maybe transition to inbetween state?
						printf("error - no t or f after #\n");
						break;
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
						currentState = inFloat;
						break;
						
					case ' ':
					case '\t':
					case '\n':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
						
					case ')':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case '(':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case '"':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
					
					case '\'':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inComment;
						break;	
						
					default:
						currentState=inSymbol;
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
						
					case ' ':
					case '\t':
					case '\n':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
					
					case '(':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case ')':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case '"':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inComment;
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
						
					case ' ':
					case '\t':
					case '\n':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
					
					case '(':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case ')':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
					
					case '"':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inComment;
						break;	
						
					default:
						currentState = inSymbol;
						break;
				}
			break;
			
			case inSymbol:
				switch (expression[tokenCurrentIndex]) {
					case ' ':
					case '\t':
					case '\n':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
						
					case '(':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
							
					case ')':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case '"':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inComment;
						break;
						
					default:
						break;
				}
			break;
			
			case inString:
				switch (expression[tokenCurrentIndex]) {
					case '"':
						pushToken(tokenList, stringType, substr(expression, tokenStartIndex, tokenCurrentIndex+1));
						currentState = inBetween;
						break;
					
					case '\\':
						currentState = inEscaped;
						break;
					
					default:
						break;			
				}
			break;
			
			case inEscaped:
				currentState = inString;
				break;
			
			case inComment:
				switch (expression[tokenCurrentIndex]) {
					case '\n':
						currentState = inBetween;
						break;
						
					default:
						break;
				}
			break;
			
			default:
				printf("error: current state error\n");
				break;
		}
		
		tokenCurrentIndex++;
	}
	
	reverse(tokenList);
	return tokenList;
	
}
