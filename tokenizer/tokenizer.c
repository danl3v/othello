#include <stdio.h>
#include "tokenizer.h"

enum TOKEN_TYPE {
	booleanType, integerType, floatType, stringType, symbolType, openType, closeType
};

enum STATE_TYPE {
	/*
	
	
	
	
	
	*/
	inBetween, inBool, inInteger, inFloat, inString, inSymbol, inPreNumber, inEscaped
};

typedef struct __Value {
	int type;
	union {
		int boolValue;
		int integerValue;
		float floatValue;
		char *stringValue;
		char *symbolValue;
		char *openType;
		char *closeType;
	} val;
} Value;

void tokenize (char *expression) {
	int currentState = inBetween;
	int tokenStartIndex = 0;
	int tokenCurrentIndex = 0;

	while (expression[tokenCurrentIndex]) {
		switch (currentState) {
		
			case inBetween:
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
						
					default:
						printf("default inBetween\n");
						break;
				}
			break;
				
			case inBool:
				switch (expression[tokenCurrentIndex]) {
					case 't':
						printf("#t:bool\n");
						currentState = inBetween;
						break;
					
					case 'T':
						printf("#t:bool\n");
						currentState = inBetween;
						break;
						
					case 'f':
						printf("#f:bool\n");
						currentState = inBetween;
						break;
						
					case 'F':
						printf("#f:bool\n");
						currentState = inBetween;
						break;
						
					default:
						printf("error - no t or f after #\n");
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