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


void train(
        const char* filename,
        const string start_symbol = "<s>",
        const string end_symbol = "</s>"
) {
    ifstream ifs(filename);
    string str;

    map< pair<string, string>, int> tag_bigram, tag_word_trans;
    map<string, int> tag_unigram;
    while (getline(ifs, str)) {
        vector<string> words_without_symbol;
        boost::algorithm::split(words_without_symbol, str, boost::is_space());
        vector<string> words, tags;

        //
        words.push_back(start_symbol);
        tags.push_back(start_symbol);
        for (auto word : words_without_symbol) {
            vector<string> pr;
            boost::algorithm::split(pr, word, boost::is_any_of("_"));
            words.push_back(pr[0]);
            tags.push_back(pr[1]);
        }
        words.push_back(end_symbol);
        tags.push_back(end_symbol);

        int size = words.size();

        for (int i = 0; i < size - 1; i++) {
            string t0 = tags[i];
            string t1 = tags[i+1];
            tag_unigram[t0]++;
            tag_bigram[make_pair(t0, t1)]++;
            string w1 = words[i+1];
            if (i != size - 2) {
                // </s> は除く
                tag_word_trans[make_pair(t1, w1)]++;
            }
        }
    }

    for (auto item : tag_bigram) {
        string t0 = item.first.first;
        string t1 = item.first.second;
        int count = item.second;
        double prob = (double)count / tag_unigram[t0];
        printf("T %s %s %f\n", t0.c_str(), t1.c_str(), prob);
    }

    for (auto item : tag_word_trans) {
        string tag = item.first.first;
        string word = item.first.second;
        int count = item.second;
        double prob = (double)count / tag_unigram[tag];
        printf("E %s %s %f\n", tag.c_str(), word.c_str(), prob);
    }
}


int main(int argc, const char *argv[])
{
    const char* filename = argv[1];
    train(filename);
    return 0;
}

