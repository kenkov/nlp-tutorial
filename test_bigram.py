#! /usr/bin/env python
# coding:utf-8


import math


def prob(
        words: (str, str),
        unigram: dict,
        bigram: dict,
        lambda1: float,
        lambda2: float,
        u_unk: float
    ):
    if words in bigram:
        pb = lambda2 * bigram[words]
    else:
        w1, w2 = words
        _pb = lambda1 * unigram[w2] if w2 in unigram \
                else (1 - lambda1) * (1 / u_unk)
        pb = (1 - lambda2) * _pb

    return pb


def test(
        modelfile: str,
        filename: str,
        lambda1: float=0.90,
        lambda2: float=0.75,
        n_unk: int=1e6
    ):

    # load model
    unimodel = {}
    bimodel = {}
    with open(modelfile) as f:
        for key, val in (line.rstrip().split("\t") for line in f):
            val = float(val)
            if " " in key:
                bimodel[tuple(key.split())] = val
            else:
                unimodel[key] = val

    entropy = 0
    total = 0
    found = 0

    with open(filename) as f:
        for line in f:
            lst = line.rstrip().split() + ["</s>"]

            w0 = lst[0]
            ent = lambda1 * unimodel[w0] if w0 in unimodel else \
                    (1 - lambda1) * (1 / n_unk)
            entropy += -math.log2(ent)
            for w1, w2 in zip(lst, lst[1:]):
                total += 1
                if w1 in unimodel:
                    found += 1
                else:
                    print(w1, file=open("log1", "a"))
                ent = prob((w1, w2), unimodel, bimodel, lambda1, lambda2, n_unk)
                entropy += -math.log2(ent)
            # count </s>
            total += 1
            found += 1

    entropy = entropy / total
    print(total, found)

    print("entropy = {:.6f}".format(entropy))
    print("coverage = {:.6f}".format(found / total))


if __name__ == '__main__':
    import sys
    modelfile, filename = sys.argv[1:3]
    test(modelfile, filename)
