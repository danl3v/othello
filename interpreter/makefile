all: tester.c interpreter.c
	gcc -o interpreter tester.c interpreter.c -Wall -pedantic
	
run: tester.c interpreter.c
	gcc -o interpreter tester.c interpreter.c -Wall -pedantic
	./interpreter standard.ss lists.ss math.ss sort.ss