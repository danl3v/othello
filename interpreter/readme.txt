;; Team Othello
;; readme.txt
;; November 17, 2010

Everything is stored in the interpreter folder. Our tester.c file contains the main function that tests our interpreter, but their are also parse specific and tokenize specific tester functions in there.

Our interpreter correctly interprets most Scheme code. It can handle all functions defined in math.ss, lists.ss, sort.ss, and standard.ss. The load function loads from the directory where the executable is being run. It can handle special forms like lambda, define, let, let*, letrec, and, or, set!, quote, and primitive values like arithmetic operators, car, cdr, cons, etc. It can handle both types of variable arity that Scheme supports, both by using the dot notation and by passing lambda a full list of arguments. We have an overarching framework that allows the user to type "exit" to quit the program, freeing all bound variables. To execute the default configuration, execute 'make run' in the interpreter directory. This will compile the code, and execute the executable with the four aforementioned libraries.

We wrote a couple of garbage collection functions, but they are very buggy and seg fault a lot so we do not really use them. Overall, we maintain a list of all malloced values. With the freeAll function, we iterate through the list of malloced values and free all of the malloced values. We also implemented a mark and sweep garbage collector. It starts with the bindings in the top frame, and depth first searches while marking all of the values it can find. Then the sweeper goes through the malloced value list and frees and removes those items from the list while resetting all values as unmarked. We had a branch of our code where freeAll and markValue worked. However, with sweepValues, we always segmentation faulted on the next input indicating that maybe we were not marking all of the values that are actually necessary. It is strange though, because we traced the segmentation fault to the tokenizer - in reverse. If we had more time, we would like to do a better job with garbage collection.

Currently not working:
garbage collection
syntactic sugar quote
reserved function names
equality has some issues