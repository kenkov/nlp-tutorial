OBJ = util.o
SHELL = /bin/bash
GPP = g++ -std=c++11 -Wall -O2 util.o
TESTDIR = nlp-programming/test
DATADIR = nlp-programming/data

main: util.o 01 02 03 04 05

test: 01test 02test 03test 04test 05test

all: util.o 01 01wiki 01-python 01wiki-python 01test 02 02wiki 02-python 02wiki-python 03 03test 03wiki 04

util.o : util.cpp
	g++ -std=c++11 -Wall -O2 -c util.cpp

01:
	${GPP} train_unigram.cpp -o train_unigram
	${GPP} test_unigram.cpp -o test_unigram

01test:
	./train_unigram ${TESTDIR}/01-train-input.txt >01-train-answer.txt
	./test_unigram 01-train-answer.txt ${TESTDIR}/01-test-input.txt
	diff -q ./01-train-answer.txt ${TESTDIR}/01-train-answer.txt

01wiki:
	./train_unigram ${DATADIR}/wiki-en-train.word >01-wiki.model
	./test_unigram 01-wiki.model ${DATADIR}/wiki-en-test.word

01-python:
	python train_unigram.py ${TESTDIR}/01-train-input.txt | sort >01-train-answer.txt
	python test_unigram.py ./01-train-answer.txt ${TESTDIR}/01-test-input.txt >01-test-answer.txt

01wiki-python:
	python train_unigram.py ${DATADIR}/wiki-en-train.word >01-wiki.model
	python test_unigram.py 01-wiki.model ${DATADIR}/wiki-en-test.word

02: util.o
	${GPP} train_bigram.cpp -o train_bigram
	${GPP} test_bigram.cpp -o test_bigram

02test:
	./train_bigram ${TESTDIR}/02-train-input.txt | sort >02-train-answer.txt
	#diff -q 02-train-answer.txt <(grep -v "^#" 02-mod-train-answer.txt)

02wiki:
	./train_bigram ${DATADIR}/wiki-en-train.word >02-wiki.model
	./test_bigram 02-wiki.model ${DATADIR}/wiki-en-test.word

02-python:
	python train_bigram.py ${TESTDIR}/02-train-input.txt | sort >02-train-answer.txt

02wiki-python:
	python train_bigram.py ${DATADIR}/wiki-en-train.word | sort >02-wiki.model
	python test_bigram.py 02-wiki.model ${DATADIR}/wiki-en-test.word

03:
	${GPP} train_perceptron.cpp -o train_perceptron 

03test:
	./train_perceptron ${TESTDIR}/03-train-input.txt | sort >03-train-answer.txt
	diff -q 03-train-answer.txt ${TESTDIR}/03-train-answer.txt

03wiki:
	./train_perceptron ${DATADIR}/titles-en-train.labeled >03-wiki.model

04: util.o
	${GPP} unigram_worddivision.cpp -o unigram_worddivision
	${GPP} bigram_worddivision.cpp -o bigram_worddivision

04test:
	./unigram_worddivision <${TESTDIR}/04-input.txt ${TESTDIR}/04-model.txt >04-answer.txt
	diff -q 04-answer.txt ${TESTDIR}/04-answer.txt

04bigram:
	echo "I am the bone of my sword." | ./bigram_worddivision ./02-wiki.model

05:
	${GPP} train_hmm.cpp -o train_hmm
	${GPP} test_hmm.cpp -o test_hmm

05test:
	./train_hmm ${TESTDIR}/05-train-input.txt | sort >05-train-answer.txt
	diff -q 05-train-answer.txt <(sort ${TESTDIR}/05-train-answer.txt)
	./test_hmm 05-train-answer.txt <${TESTDIR}/05-test-input.txt >05-test-answer.txt
	diff -q 05-test-answer.txt ${TESTDIR}/05-test-answer.txt
