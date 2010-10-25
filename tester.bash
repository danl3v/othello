#!/bin/bash

# Change the following line so it has the right directory!
TOKENIZER=tokenizer/tokenizer

for inputFile in `find . -name "token-test.input*" | grep -v "\.svn"`
do
	outputFile=`echo $inputFile | sed 's/\.input\./.output./g;'`
	echo "---------------------"
	echo "TESTING ON $inputFile"
	cat $inputFile | $TOKENIZER | diff - $outputFile
done