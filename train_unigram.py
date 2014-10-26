#! /usr/bin/env python
# coding:utf-8


from collections import defaultdict
import sys
import math


def train(
        filename: str,
        end_symbol: bool=True
    ):

    count = defaultdict(int)
    total = 0
    with open(filename) as f:
        for line in f:
            for word in line.split() + ["</s>"] if end_symbol \
                                                else line.split():
                count[word] += 1
                total += 1

    return {w: i / total for w, i in count.items()}


if __name__ == "__main__":

    filename = sys.argv[1]
    dic = train(filename)
    for key, val in dic.items():
        print("{}\t{:.6f}".format(key, val))
