OBJ = util.o
GPP = g++ -std=c++11 -Wall -O2 util.o

main: util.o 01 01check 02 02wiki 03 04

util.o : util.cpp
	g++ -std=c++11 -Wall -O2 -c util.cpp

01:
	${GPP} train_unigram.cpp -o train_unigram && \
		./train_unigram ./nlp-programming/test/01-train-input.txt >01-train-answer.txt
	${GPP} test_unigram.cpp -o test_unigram && ./test_unigram 01-train-answer.txt nlp-programming/test/01-test-input.txt

01wiki:
	./train_unigram nlp-programming/data/wiki-en-train.word >01-wiki.model
	./test_unigram 01-wiki.model nlp-programming/data/wiki-en-test.word

01-python:
	python train_unigram.py ./nlp-programming/test/01-train-input.txt | sort >01-train-answer.txt
	python test_unigram.py ./01-train-answer.txt ./nlp-programming/test/01-test-input.txt >01-test-answer.txt

01wiki-python:
	python train_unigram.py nlp-programming/data/wiki-en-train.word >01-wiki.model
	python test_unigram.py 01-wiki.model nlp-programming/data/wiki-en-test.word

01check:
	diff -q ./01-train-answer.txt ./nlp-programming/test/01-train-answer.txt

02: util.o
	${GPP} train_bigram.cpp -o train_bigram && \
		./train_bigram nlp-programming/test/02-train-input.txt | sort
	${GPP} test_bigram.cpp -o test_bigram

02wiki:
	./train_bigram nlp-programming/data/wiki-en-train.word >/dev/null
	./test_bigram 02-wiki.model nlp-programming/data/wiki-en-test.word

02-python:
	python train_bigram.py nlp-programming/test/02-train-input.txt | sort >02-train-answer.txt

02wiki-python:
	python train_bigram.py nlp-programming/data/wiki-en-train.word | sort >02-wiki.model
	python test_bigram.py 02-wiki.model nlp-programming/data/wiki-en-test.word

03:
	${GPP} train_perceptron.cpp -o train_perceptron && ./train_perceptron ./nlp-programming/test/03-train-input.txt | sort >03-train-answer.txt
	diff -q 03-train-answer.txt nlp-programming/test/03-train-answer.txt

03wiki:
	./train_perceptron ./nlp-programming/data/titles-en-train.labeled >03-wiki.model

04: util.o
	${GPP} unigram_worddivision.cpp -o unigram_worddivision && ./unigram_worddivision 01wiki.model
