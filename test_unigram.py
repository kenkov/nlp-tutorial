#! /usr/bin/env python
# coding:utf-8

from train_unigram import train
from collections import defaultdict
import math


def test(
    modelfile: str,
    filename: str,
    lambda1: float=0.95,
    n_unk: int=1e6
    ):
    # load model
    model = {}
    with open(modelfile) as f:
        for key, val in (line.split("\t") for line in f):
            model[key] = float(val)

    entropy = 0
    total = 0
    found = 0

    with open(filename) as f:
        for line in f:
            for word in line.split() + ["</s>"]:
                total += 1
                if word in model:
                    ent = lambda1 * model[word]
                    found += 1
                else:
                    ent = (1 - lambda1) * (1 / n_unk)
                entropy += -math.log2(ent)
    entropy = entropy / total

    print("entropy = {:.6f}".format(entropy))
    print("coverage = {:.6f}".format(found / total))



if __name__ == '__main__':
    import sys
    modelfile, filename = sys.argv[1:3]
    test(modelfile, filename)
