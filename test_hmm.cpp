#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <boost/algorithm/string.hpp>

#include "util.h"

using namespace std;


double tag_bigram_prob(
        string w0,
        string w1,
        map< pair<string, string>, double> model
) {
    return model[make_pair(w0, w1)];
}


double tag_word_prob(
        string tag,
        string word,
        map< pair<string, string>, double> model,
        const double lambda1=0.95,
        const long n_unk=1e6
) {
    pair<string, string> pr = make_pair(tag, word);
    if (model.count(pr) == 0) {
        return (1 - lambda1) * (double)1 / n_unk;
    } else {
        return lambda1 * model[pr];
    }
}


vector<string> hmm_search(
        vector<string> words_without_symbol,
        const char *modelfile,
        const set<string> tags,
        const double lambda1=0.95,
        const long n_unk=1e6,
        const string start_symbol = "<s>",
        const string end_symbol = "</s>"
) {
    // load model
    map< pair<string, string>, double> tag_bigram, tag_word_trans;
    ifstream modelfs(modelfile);
    string str;
    while (getline(modelfs, str)) {
        char _type[8], _w0[100], _w1[100];
        double prob;
        sscanf(str.c_str(), "%s %s %s %lf", _type, _w0, _w1, &prob);
        string type = _type;
        string w0 = _w0;
        string w1 = _w1;

        if (type == "T") {
            tag_bigram[make_pair(w0, w1)] = prob;
        } else {
            tag_word_trans[make_pair(w0, w1)] = prob;
        }
    }

    // create words vector
    vector<string> words;
    words.push_back(start_symbol);
    for (auto word : words_without_symbol) {
        words.push_back(word);
    }
    words.push_back(end_symbol);

    unsigned int size = words.size();
    vector< map<string, double> > best_score(size);
    vector< map<string, string> > before_tag(size);

    //before_tag[0] = start_symbol;
    best_score[0][start_symbol] = 0;

    for (unsigned int curpos = 0; curpos < size - 1; curpos++) {
        for (auto curitem : best_score[curpos]) {
            string curtag = curitem.first;
            double curprob = curitem.second;

            int nextpos = curpos + 1;
            string nextword = words[nextpos];
            if (curpos == size - 2) {
                // deal with </s>
                string nexttag = end_symbol;
                double nextprob = curprob - log(tag_bigram_prob(curtag, nexttag, tag_bigram));
                if (best_score[nextpos].count(nexttag)) {
                    if (nextprob < best_score[nextpos][nexttag]) {
                        best_score[nextpos][nexttag] = nextprob;
                        before_tag[nextpos][nexttag] = curtag;
                    }
                } else {
                    best_score[nextpos][nexttag] = nextprob;
                    before_tag[nextpos][nexttag] = curtag;
                }
            } else {
                for (auto nexttag : tags) {
                    double nextprob = curprob - log(tag_bigram_prob(curtag, nexttag, tag_bigram)) - log(tag_word_prob(nexttag, nextword, tag_word_trans));
                    if (best_score[nextpos].count(nexttag)) {
                        if (nextprob < best_score[nextpos][nexttag]) {
                            best_score[nextpos][nexttag] = nextprob;
                            before_tag[nextpos][nexttag] = curtag;

                        }
                    } else {
                        best_score[nextpos][nexttag] = nextprob;
                        before_tag[nextpos][nexttag] = curtag;
                    }
                }
            }
        }
    }

    vector<string> answer(size);
    answer[size - 1] = end_symbol;
    string ctag = end_symbol;
    for (unsigned int i = 1; i < size; i++) {
        int pos = size - i;
        string ntag = before_tag[pos][ctag];
        answer[pos - 1] = ntag;
        ctag = ntag;
    }
    return answer;
}


int main(int argc, const char *argv[])
{
    const char *modelfile = argv[1];

    set<string> tags;
    for (auto tag : {"X", "Y", "Z"}) {
        tags.insert(tag);
    }

    string str;
    while (getline(cin, str)) {
        vector<string> words;
        boost::algorithm::split(words, str, boost::is_space());

        vector<string> answer = hmm_search(words, modelfile, tags);

        for (unsigned int i = 1; i < answer.size()-1; i++) {
            cout << answer[i];
            if (i == answer.size() - 2) {
                 cout << endl;
            } else {
                cout << " ";
            }
        }
    }
}

