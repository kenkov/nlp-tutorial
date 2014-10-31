#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <boost/algorithm/string.hpp>

#include "util.h"

using namespace std;


double tag_bigram_prob(
        string w0,
        string w1
) {
    return 1.0;
}


double word_tag_prob(
        string word,
        string tag
) {
    return 1.0;
}


vector<string> hmm_search(
        vector<string> words_without_symbol,
        const char *modelfile,
        const set<string> tags,
        const double lambda1=0.90,
        const long n_unk=1e6,
        const string start_symbol = "<s>",
        const string end_symbol = "</s>"
) {

    // create words vector
    vector<string> words;
    words.push_back(start_symbol);
    for (auto word : words_without_symbol) {
        words.push_back(word);
    }
    words.push_back(end_symbol);

    unsigned int size = words.size();
    vector< map<string, double> > best_score(size);
    vector<string> before_tag(size);

    before_tag[0] = start_symbol;
    best_score[0][start_symbol] = 0;

    for (unsigned int curpos = 0; curpos < size - 1; curpos++) {
        for (auto curitem : best_score[curpos]) {
            string curtag = curitem.first;
            double curprob = curitem.second;

            int nextpos = curpos + 1;
            string nextword = words[nextpos];
            for (auto nexttag : tags) {
                double nextprob = curprob - log(tag_bigram_prob(curtag, nexttag));
                if (curpos != size - 2) {
                    nextprob -= log(word_tag_prob(nextword, nexttag));
                }
                if (best_score[nextpos].count(nexttag)) {
                    if (nextprob < best_score[nextpos][nexttag]) {
                        best_score[nextpos][nexttag] = nextprob;
                        before_tag[curpos] = curtag;
                    }
                } else {
                    best_score[nextpos][nexttag] = nextprob;
                    before_tag[curpos] = curtag;
                }
            }
        }
    }

    vector<string> answer(size);
    answer[size - 1] = end_symbol;
    for (unsigned int i = 1; i < size; i++) {
        int pos = size - i - 1;
        answer[pos] = before_tag[pos];
    }
    return answer;
}


int main(int argc, const char *argv[])
{
    const char *modelfile = argv[1];

    set<string> tags;
    for (auto tag : {"a", "b", "c", "d"}) {
        tags.insert(tag);
    }

    string str;
    while (getline(cin, str)) {
        vector<string> words;

        boost::algorithm::split(words, str, boost::is_space());
        //for (unsigned int i=0; i < str.size(); i++) {
        //    words[i] = str[i];
        //}

        vector<string> answer = hmm_search(words, modelfile, tags);

        for (unsigned int i = 0; i < answer.size(); i++) {
            cout << answer[i];
            if (i == answer.size() - 1) {
                 cout << endl;
            } else {
                cout << "|";
            }
        }
    }
}

