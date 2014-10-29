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


string substring(vector<string> words, int start, int end) {
    string word = "";
    for (int i = start; i < end + 1; i++) {
        word += words[i];
    }
    return word;
}


double uniprob(
        string word,
        map<string, double> unimodel,
        const double lambda1 = 0.95,
        const long n_unk = 1e6
) {
    double prob;
    if (unimodel.count(word)) {
        prob = lambda1 *  unimodel[word];
    } else {
        prob = (1 - lambda1) * (1.0 / n_unk);
    }
    return prob;
}


map<string, double> load_unimodel(
        const char* modelfile
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
    return model;
}
