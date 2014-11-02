#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <cmath>
#include <boost/algorithm/string.hpp>

#include "typedef.h"
#include "util.h"

using namespace std;


void test(
        const char *modelfile,
        const char *filename,
        const double lambda1=0.90,
        const double lambda2=0.75,
        const long n_unk=1e6,
        const string start_symbol = "<s>",
        const string end_symbol = "</s>"
) {
    // load model
    ifstream modelfs(modelfile);
    string line;

    pair< map<string, double>, map< pair<string, string>, double> > model = load_bimodel(modelfile);
    map<string, double> unimodel = model.first;
    map< pair<string, string>, double> bimodel = model.second;


    ifstream datafs(filename);
    double entropy = 0;
    long total = 0;
    long found = 0;
    while(getline(datafs, line)) {

        vector<string> words, words_without_symbol;
        boost::algorithm::split(words_without_symbol, line, boost::is_space());

        // create words vector
        words.push_back(start_symbol);
        for (auto word : words_without_symbol) {
            words.push_back(word);
        }
        words.push_back(end_symbol);

        for (unsigned long i = 0; i < words.size() - 1; i++) {
            string w0 = words[i];
            string w1 = words[i+1];
            pair<string, string> pr = make_pair(w0, w1);
            double prob = biprob(pr, unimodel, bimodel, lambda1, lambda2, n_unk);

            entropy -= log2(prob);

            // count found word for coverage
            // start_symbol ¤Ï½ü¤¯
            if (i != 0) {
                total++;
                if (unimodel.count(w0)) {
                found += 1;
                }
            }
        }
        // count </s>
        total++;
        found++;
    }
    entropy = entropy / total;
    printf("%ld %ld\n", total, found);
    printf("entropy = %f\n", entropy);
    printf("coverage = %f\n", (double)found / total);

}


int main(int argc, char const* argv[])
{
    const char *modelfile = argv[1];
    const char *filename = argv[2];
    test(modelfile, filename);
    return 0;
}

