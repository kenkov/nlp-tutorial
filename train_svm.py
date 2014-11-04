#! /usr/bin/env python
# coding:utf-8


from collections import defaultdict


def _margin(
        x: dict,
        y: int,
        w: dict) -> float:
    mg = 0
    for key, val in x.items():
        mg += val * w[key]
    return mg * y


def condition(
        x: dict,
        y: int,
        w: dict,
        margin: float):
    return _margin(x, y, w) <= margin


def sign(x: float) -> int:
    return 1 if x >= 0 else -1


def train(
        trainset: [(dict, float)],
        margin: float,
        c=1e-4) -> dict:
    w = defaultdict(int)
    for x, y in trainset:
        # update parameter
        if condition(x, y, w, margin):
            # regulalization update
            for key, val in w.items():
                if val <= c:
                    w[key] = 0
                else:
                    w[key] -= c * sign(val)
            # dataset update
            for key, val in x.items():
                w[key] += val * y
                print(key, w[key])
    return w


def unigram(x: list):
    dic = defaultdict(int)
    for i in x:
        dic[i] += 1
    return dic


if __name__ == '__main__':
    import sys
    w = train(
        ((unigram(items[1:]), float(items[0]))
            for items in (line.split() for line in sys.stdin)),
        1.0)

    for key, val in w.items():
        print("{}\t{}".format(key, val))
