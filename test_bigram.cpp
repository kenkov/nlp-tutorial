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
        const char *modelfile,
        const char *filename,
        const double lambda1=0.90,
        const double lambda2=0.75,
        const long n_unk=1e6,
        const string end = "</s>"
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

    ifstream datafs(filename);
    double entropy = 0;
    long total = 0;
    long found = 0;
    while(getline(datafs, line)) {
        boost::algorithm::split(words, line, boost::is_space());
        words.push_back(end);
        double prob = 0;
        if (unimodel.count(words[0])) {
            prob = lambda1 * unimodel[words[0]];
        } else {
            prob = (1 - lambda1) * ((double)1 / n_unk);
        }
        entropy -= log2(prob);
        for (unsigned long i = 0; i < words.size() - 1; i++) {
            pair<string, string> pr = make_pair(words[i], words[i+1]);
            string w1 = words[i];
            string w2 = words[i+1];
            if (bimodel.count(pr)) {
                prob = lambda2 * bimodel[pr];
            } else if (unimodel.count(w2)) {
                prob = (1 - lambda2) * lambda1 * unimodel[w2];
            } else {
                prob = (1 - lambda2) * (1 - lambda1) * ((double)1 / n_unk);
            }
            entropy -= log2(prob);

            // count found word for coverage
            if (unimodel.count(w1)) {
                found += 1;
            }
            total += 1;
        }
        // count </s>
        total += 1;
        found += 1;
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

