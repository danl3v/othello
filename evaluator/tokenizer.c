/*to think about: free Value objects (but we need to consider if we have two lists point to same value, so maybe we want a list of malloc'ed values, or we store the number of the object is referenced, number of pointers to the object in the value struct)
 		 on errors, we should print list and then return to stop tokenizing, consider how we want to handle errors
		 store line numbers in value struct */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType, 
	listType, closureType, primitiveType
};

enum STATE_TYPE {
	inBetween, inBool, inInteger, inFloat, inString, inSymbol, inPreNumber, inPreFloat, inComment
};

char *substr(char *string, int start, int end) { // copies from string from index start to end-1   
	int i;
	char *result = malloc(sizeof(char)*(end - start + 1));
	for (i=0;i<end-start;i++) {
		result[i] = string[start+i];
	}
	result[end-start] = 0;
	return result;
}

int pushToken(LinkedList *tokenList, int type, char *string, int lineNumber) {
	Value *token = malloc(sizeof(Value));
	//push(allValueObjects, token);
	char * str;
	
	if (token) {
		token->type = type;
		token->lineNumber = lineNumber;
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
		push(tokenList, token);
	}
	return 0;
}

LinkedList *tokenize (char *expression) {
	int lineNumber = 1;
	int currentState = inBetween;
	int tokenStartIndex = 0;
	int tokenCurrentIndex = 0;
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
						
					case '\n':
					case '\r':
						lineNumber++;
					case ' ':
					case '\t':
						break;
					
					case '(':
					case '[':
						pushToken(tokenList, openType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
						break;
						
					case ')':
					case ']':
						pushToken(tokenList, closeType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
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
						pushToken(tokenList, quoteType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
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
						pushToken(tokenList, booleanType, substr(expression, tokenStartIndex+1, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					default: // maybe transition to inbetween state?
						printf("error - no t or f after #\n");
						destroy(tokenList);
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
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '(':
					case '[':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '"':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
					
					case '\'':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
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
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
					
					case '(':
					case '[':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
					
					case '"':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
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
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
					
					case '(':
					case '[':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '"':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, integerType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
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
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
					
					case '(':
					case '[':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ')':
					case ']':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
					
					case '"':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, floatType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
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
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						currentState = inBetween;
						break;
						
					case '(':
					case '[':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, openType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
							
					case ')':
					case ']':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, closeType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case '"':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
						pushToken(tokenList, quoteType, substr(expression, tokenCurrentIndex, tokenCurrentIndex+1), lineNumber);
						currentState = inBetween;
						break;
						
					case ';':
						pushToken(tokenList, symbolType, substr(expression, tokenStartIndex, tokenCurrentIndex), lineNumber);
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
						pushToken(tokenList, stringType, substr(expression, tokenStartIndex, tokenCurrentIndex+1), lineNumber);
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
				destroy(tokenList);
				return NULL;
		}
		
		tokenCurrentIndex++;
	}
	
	if (currentState == inString) {
		printf("error - unterminated string\n");
		destroy(tokenList);
		return NULL;
	}
	
	return reverse(tokenList);
	
}
