OBJ = util.o
GPP = g++ -std=c++11 -Wall -O2 util.o
TESTDIR = ./nlp-programming/test
DATADIR = ./nlp-programming/data

main: util.o 01 01check 02 02wiki 03 04

all: util.o 01 01wiki 01-python 01wiki-python 01check 02 02wiki 02-python 02wiki-python 03 03wiki 04

util.o : util.cpp
	g++ -std=c++11 -Wall -O2 -c util.cpp

01:
	${GPP} train_unigram.cpp -o train_unigram && \
		./train_unigram ${TESTDIR}/01-train-input.txt >01-train-answer.txt
	${GPP} test_unigram.cpp -o test_unigram && ./test_unigram 01-train-answer.txt ${TESTDIR}/01-test-input.txt

01wiki:
	./train_unigram ${DATADIR}/wiki-en-train.word >01-wiki.model
	./test_unigram 01-wiki.model ${DATADIR}/wiki-en-test.word

01-python:
	python train_unigram.py ${TESTDIR}/01-train-input.txt | sort >01-train-answer.txt
	python test_unigram.py ./01-train-answer.txt ${TESTDIR}/01-test-input.txt >01-test-answer.txt

01wiki-python:
	python train_unigram.py ${DATADIR}/wiki-en-train.word >01-wiki.model
	python test_unigram.py 01-wiki.model ${DATADIR}/wiki-en-test.word

01check:
	diff -q ./01-train-answer.txt ${TESTDIR}/01-train-answer.txt

02: util.o
	${GPP} train_bigram.cpp -o train_bigram && \
		./train_bigram ${TESTDIR}/02-train-input.txt | sort
	${GPP} test_bigram.cpp -o test_bigram

02wiki:
	./train_bigram ${DATADIR}/wiki-en-train.word >/dev/null
	./test_bigram 02-wiki.model ${DATADIR}/wiki-en-test.word

02-python:
	python train_bigram.py ${TESTDIR}/02-train-input.txt | sort >02-train-answer.txt

02wiki-python:
	python train_bigram.py ${DATADIR}/wiki-en-train.word | sort >02-wiki.model
	python test_bigram.py 02-wiki.model ${DATADIR}/wiki-en-test.word

03:
	${GPP} train_perceptron.cpp -o train_perceptron && ./train_perceptron ${TESTDIR}/03-train-input.txt | sort >03-train-answer.txt
	diff -q 03-train-answer.txt ${TESTDIR}/03-train-answer.txt

03wiki:
	./train_perceptron ${DATADIR}/titles-en-train.labeled >03-wiki.model

04: util.o
	${GPP} unigram_worddivision.cpp -o unigram_worddivision && \
		cat ${TESTDIR}/04-input.txt | ./unigram_worddivision ${TESTDIR}/04-model.txt >04-answer.txt
	diff -q 04-answer.txt ${TESTDIR}/04-answer.txt

