#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

using namespace std;


void test(
        const char* modelfile,
        const char* filename,
        const double lambda1 = 0.95,
        const int n_unk = 1e6
) {
    // load model
    map<string, double> model;
    ifstream modelfs(modelfile);
    string str;
    while(getline(modelfs, str)) {
        char word[100];
        double val;
        sscanf(str.c_str(), "%s\t%lf", word, &val);
        model[(string)word] = val;
    }

    ifstream datafs(filename);
    string line;
    int total = 0;
    int found = 0;
    double entropy = 0;
    while (getline(datafs, line)) {
        vector<string> words;
        boost::algorithm::split(words, line, boost::is_space());
        words.push_back("</s>");
        double prob = 0;
        if (model.count(words[0])) {
            prob = lambda1 * model[words[0]];
        } else {
            prob = (1 - lambda1) * (1 / n_unk);
        }
        for (auto word: words) {
            if (model.count(word)) {
                prob = lambda1 *  model[word];
                found++;
            } else {
                prob = (1 - lambda1) * (1.0 / n_unk);
            }
            total++;
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

