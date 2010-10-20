#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

char* substr(char* string, int start, int end) {
	char *result = malloc(sizeof(char)*(end - start + 1));
	strncpy(result, &string[start], end-start);
	return result;
}

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType, quoteType
};

enum STATE_TYPE {
	/*
	inBetween - blah blah
	inBool
	inInteger
	inFloat
	inString
	inPreNumber
	inEscaped
	inComment
	*/
	inBetween, inBool, inInteger, inFloat, inString, inSymbol, inPreNumber, inEscaped, inComment
};

void tokenize (char *expression) {
	int currentState = inBetween;
	int tokenStartIndex = 0;
	int tokenCurrentIndex = 0;

	while (expression[tokenCurrentIndex]) {
		switch (currentState) {
		
			case inBetween:
				tokenStartIndex = tokenCurrentIndex;
				switch (expression[tokenCurrentIndex]) {
					case '#':
						currentState = inBool;
						break;
						
					case ' ':
						break;
					
					case '\n':
						break;
					
					case '(':
						printf("(:open\n");
						break;
						
					case ')':
						printf("):close\n");
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
						printf("':quote\n");
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
						printf("#t:bool\n");
						currentState = inBetween;
						break;
						
					case 'f':
					case 'F':
						printf("#f:bool\n");
						currentState = inBetween;
						break;
						
					default:
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
					case '\n':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
						
					case ')':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("(:open\n");
						currentState = inBetween;
						break;
						
					case '(':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("):close\n");
						currentState = inBetween;
						break;
						
					case '"':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
					
					case '\'':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("':quote\n");
						currentState = inBetween;
						break;
						
					case ';':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
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
					case '\n':
						printf("%s:integer\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
					
					case '(':
						printf("%s:integer\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("(:open\n");
						currentState = inBetween;
						break;
						
					case ')':
						printf("%s:integer\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("):close\n");
						currentState = inBetween;
						break;
						
					case '"':
						printf("%s:integer\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						printf("%s:integer\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("':quote\n");
						currentState = inBetween;
						break;
						
					case ';':
						printf("%s:integer\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
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
					case '\n':
						printf("%s:float\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
					
					case '(':
						printf("%s:float\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("(:open\n");
						currentState = inBetween;
						break;
						
					case ')':
						printf("%s:float\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("):close\n");
						currentState = inBetween;
						break;
					
					case '"':
						printf("%s:float\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						printf("%s:float\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("':quote\n");
						currentState = inBetween;
						break;
						
					case ';':
						printf("%s:float\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
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
					case '\n':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inBetween;
						break;
						
					case '(':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("(:open\n");
						currentState = inBetween;
						break;
							
					case ')':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("):close\n");
						currentState = inBetween;
						break;
						
					case '"':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						tokenStartIndex = tokenCurrentIndex;
						currentState = inString;
						break;
						
					case '\'':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						printf("':quote\n");
						currentState = inBetween;
						break;
						
					case ';':
						printf("%s:symbol\n", substr(expression, tokenStartIndex, tokenCurrentIndex));
						currentState = inComment;
						break;
						
					default:
						break;
				}
			break;
			
			case inString:
				switch (expression[tokenCurrentIndex]) {
					case '"':
						printf("%s:string\n", substr(expression, tokenStartIndex, tokenCurrentIndex+1));
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
				printf("what do i do???\n");
				break;
		}
		
		tokenCurrentIndex++;
	}

}