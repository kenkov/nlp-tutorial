#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

#include "typedef.h"

using namespace std;


string substring(vector<string> words, int start, int end);

double uniprob(
        string word,
        map<string, double> model,
        const double lambda1 = 0.95,
        const long n_unk = 1e6
);

map<string, double> load_unimodel(
        const char* modelfile
);

bimodeltype load_bimodel(
        const char* modelfile
);

double biprob(
        pair<string, string> wordpair,
        map<string, double> unimodel,
        map<pair<string, string>, double> bimodel,
        const double lambda1=0.90,
        const double lambda2=0.75,
        const long n_unk=1e6
);
