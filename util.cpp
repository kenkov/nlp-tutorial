#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

#include "typedef.h"

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


bimodeltype load_bimodel(
        const char* modelfile
) {
    // load model
    ifstream modelfs(modelfile);
    string line;
    vector<string> words;

    map<string, double> unimodel;
    map< pair<string, string>, double> bimodel;

    while(getline(modelfs, line)) {
        if (line.find(" ") == string::npos) {
            char word[100];
            double val;
            sscanf(line.c_str(), "%s\t%lf", word, &val);
            unimodel[(string)word] = val;
        } else {
            char w1[100];
            char w2[100];
            double val;
            sscanf(line.c_str(), "%s %s\t%lf", w1, w2, &val);
            bimodel[make_pair(w1, w2)] = val;
        }
    }
    return make_pair(unimodel, bimodel);
}

double biprob(
        pair<string, string> wordpair,
        map<string, double> unimodel,
        map<pair<string, string>, double> bimodel,
        const double lambda1=0.90,
        const double lambda2=0.75,
        const long n_unk=1e6,
        const string end = "</s>"
) {
    string w1 = wordpair.first;
    string w2 = wordpair.second;
    double prob;
    if (bimodel.count(wordpair)) {
        prob = lambda2 * bimodel[wordpair];
    } else if (unimodel.count(w2)) {
        prob = (1 - lambda2) * lambda1 * unimodel[w2];
    } else {
        prob = (1 - lambda2) * (1 - lambda1) * ((double)1 / n_unk);
    }
    return prob;
}
