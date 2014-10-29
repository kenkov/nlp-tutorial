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
