#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "util.h"

using namespace std;


vector<string> wordsplit(
        vector<string> uniwords,
        const char *modelfile,
        unsigned int max_word_len=10,
        double mx=pow(10, 10),
        const double lambda1 = 0.95,
        const long n_unk = 1e6
) {
    // load model
    map<string, double> model = load_unimodel(modelfile);

    // main
    unsigned int size = uniwords.size();
    // best score for forward step
    vector<double> best_score(size+1);
    // positions for backword step
    vector<int> before_pos(size+1);
    for (unsigned int i = 0; i <=  size; i++) {
        best_score[i] = mx;
        before_pos[i] = 0;
    }
    best_score[0] = 0;
    for (unsigned int curpos = 0; curpos < uniwords.size(); curpos++) {
        for (unsigned int j = 0; j < min(max_word_len, size - curpos); j++) {
            string word = substring(uniwords, curpos, curpos + j);
            double prob = log(uniprob(word, model, lambda1, n_unk));
            double cand_prob = best_score[curpos] - prob;
            int endpos = curpos + j + 1;
            if (cand_prob < best_score[endpos]) {
                best_score[endpos] = cand_prob;
                before_pos[endpos] = curpos;
            }
        }
    }
    // verbose
    //for (unsigned int i=0; i <= size; i++) {
    //    printf("%d\t%f %d\n", i, best_score[i], before_pos[i]);
    //}
    int start = size;
    vector<string> answer;
    while (start != 0) {
        int curstart = before_pos[start];
        answer.push_back(substring(uniwords, curstart, start-1));
        start = curstart;
    }
    reverse(answer.begin(), answer.end());
    return answer;
}


int main(int argc, const char *argv[])
{
    const char* modelfile = argv[1];

    string str;
    while (getline(cin, str)) {
        vector<string> words(str.size());

        for (unsigned int i=0; i < str.size(); i++) {
            words[i] = str[i];
        }
        // このモデルでは </s> は考慮していない
        // words.push_back("</s>");

        vector<string> answer = wordsplit(words, modelfile);

        for (unsigned int i = 0; i < answer.size(); i++) {
            cout << answer[i];
            if (i == answer.size() - 1) {
                 cout << endl;
            } else {
                cout << " ";
            }
        }
    }
    return 0;
}
