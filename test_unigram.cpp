#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <cmath>

#include "util.h"

using namespace std;


void test(
        const char* modelfile,
        const char* filename,
        const double lambda1 = 0.95,
        const long n_unk = 1e6
) {
    // load model
    map<string, double> model = load_unimodel(modelfile);

    ifstream datafs(filename);
    string line;
    long total = 0;
    long found = 0;
    double entropy = 0;
    while (getline(datafs, line)) {
        vector<string> words;
        boost::algorithm::split(words, line, boost::is_space());
        words.push_back("</s>");
        double prob = 0;

        prob = uniprob(words[0], model, lambda1, n_unk);
        for (auto word: words) {
            if (model.count(word)) {
                found++;
            }
            total++;
            prob = uniprob(word, model, lambda1, n_unk);
            entropy -= log2(prob);
        }
    }
    entropy = entropy / total;
    double coverage = (double)found / total;

    printf("entropy = %f\ncoverage = %f\n", entropy, coverage);
}


int main(int argc, char const* argv[])
{
    const char *modelfile = argv[1];
    const char *filename = argv[2];
    test(modelfile, filename);
    return 0;
}

