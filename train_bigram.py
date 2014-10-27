#! /usr/bin/env python
# coding:utf-8


from collections import defaultdict
from ngram import ngram


def train(
        filename: str,
        start_end_symbol=True
    ):

    bigram = defaultdict(int)
    unigram = defaultdict(int)
    total = 0

    with open(filename) as f:
        for line in f:
            lst = line.split() + ["</s>"]
            for w1, w2 in zip(lst, lst[1:]):
                bigram[(w1, w2)] += 1
                unigram[w1] += 1
                total += 1
            unigram[w2] += 1
            total += 1

    for (w1, w2), val in bigram.items():
        print("{} {}\t{:.6f}".format(w1, w2, val/unigram[w1]))

    for key, val in unigram.items():
        print("{}\t{:.6f}".format(key, val/total))


if __name__ == '__main__':
    import sys

    filename = sys.argv[1]
    train(filename=filename)
