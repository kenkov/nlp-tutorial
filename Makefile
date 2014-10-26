main: 01

01:
	python train_unigram.py ./nlp-programming/test/01-train-input.txt | sort >01-train-answer.txt
	diff -q ./01-train-answer.txt ./nlp-programming/test/01-train-answer.txt
	python test_unigram.py ./01-train-answer.txt ./nlp-programming/test/01-test-input.txt >01-test-answer.txt

01wiki:
	python train_unigram.py nlp-programming/data/wiki-en-train.word >01-wiki.model
	python test_unigram.py 01-wiki.model nlp-programming/data/wiki-en-test.word

