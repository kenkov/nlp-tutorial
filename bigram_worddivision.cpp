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
#include "util.h"

using namespace std;


vector<string> wordsplit(
        vector<string> words,
        const char *modelfile,
        unsigned int max_word_len=8,
        double mx=pow(10, 10),
        const double lambda2 = 0.90,
        const double lambda1 = 0.95,
        const long n_unk = 1e6
) {
    bimodeltype model = load_bimodel(modelfile);
    map<string, double> unimodel = model.first;
    map<pair<string, string>, double> bimodel = model.second;

    unsigned int size = words.size();
    vector< map<pair<int, int>, double> > best_score(size + 1);
    vector< map<pair<int, int>, pair<int, int>> > before_pos(size + 1);

    // init
    best_score[0][make_pair(0, 0)] = 0;
    before_pos[0][make_pair(0, 0)] = make_pair(0, 0);

    for (unsigned int curpos = 0; curpos < size; curpos++) {
        for (auto item : best_score[curpos]) {
            pair<int, int> curposition = item.first;
            int curstart = curposition.first;
            int curend = curposition.second;
            double curprob = item.second;
            string curword = curpos == 0 ? "<s>" : substring(words, curstart, curend);

            for (unsigned int j = 0; j < min(max_word_len, size - curpos); j++) {
                int start = curpos + 1;
                int end = curpos + j + 1;
                pair<int, int> pos = make_pair(start, end);
                // 次の word
                string word = substring(words, curpos, curpos + j);
                if (curpos == 0) {
                    best_score[end][pos] = -log(uniprob(word, unimodel, lambda1, n_unk));
                    before_pos[end][pos] = make_pair(0, 0);
                } else {
                    double cand_prob = curprob - log(biprob(
                                make_pair(curword, word), unimodel, bimodel, lambda1, lambda2, n_unk));
                    if (best_score[end].count(pos)) {
                        double prob = best_score[end][pos];
                        if (cand_prob < prob) {
                            best_score[end][pos] = cand_prob;
                            before_pos[end][pos] = curposition;
                        }
                    } else {
                        best_score[end][pos] = cand_prob;
                        before_pos[end][pos] = curposition;
                    }
                }
            }
        }
    }
    for (unsigned int i = 0; i < best_score.size(); i++) {
        printf("%d\n", i);
        for (auto mapitem : best_score[i]) {
            int start = mapitem.first.first;
            int end = mapitem.first.second;
            double prob = mapitem.second;
            printf("\t(%d, %d)\t%f\n", start, end, prob);
        }
    }

    vector<string> answer;
    double prob = mx;
    int bstart = 0, bend = 0;
    int ind = words.size();
    // backword step
    for (auto mapitem : best_score[words.size()]) {
        if (mapitem.second < prob) {
            bstart = mapitem.first.first;
            bend = mapitem.first.second;
        }
    }
    while (ind) {
        answer.push_back(substring(words, bstart - 1, bend - 1));
        pair<int, int> newpos = before_pos[ind][make_pair(bstart, bend)];
        bstart = newpos.first;
        bend = newpos.second;
        ind = bend;
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
        words.push_back("</s>");

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

