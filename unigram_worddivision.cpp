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
    for (unsigned int i=0; i <= size; i++) {
        printf("%d\t%f %d\n", i, best_score[i], before_pos[i]);
    }
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
    const char* filename = argv[1];
    string word = "I am the bone of my sword.";
    vector<string> words(word.size());

    for (unsigned int i=0; i < word.size(); i++) {
        words[i] = word[i];
    }
    //boost::algorithm::split(words, word, boost::is_space());
    vector<string> answer = wordsplit(words, filename);
    words.push_back("</s>");
    for (auto word : answer) {
        cout << word << " ";
    }
    cout << endl;
    return 0;
}
