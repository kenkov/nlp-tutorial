#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <utility> // pair
#include <map>
#include <boost/algorithm/string.hpp>

using namespace std;


void train(
        const char* filename,
        const string end = "</s>"
) {
    ifstream datafs(filename);
    string line;
    map< pair<string, string>, int> bigram;
    map<string, int> unigram;

    int total = 0;

    while (getline(datafs, line)) {
        vector<string> words;
        boost::algorithm::split(words, line, boost::is_space());
        words.push_back(end);

        for (unsigned int i = 0; i < words.size() - 1; i++) {
            bigram[make_pair(words[i], words[i+1])] += 1;
            unigram[words[i]] += 1;
            total += 1;
        }
        unigram[end] += 1;
        total += 1;
    }

    for (auto item : bigram) {
        printf("%s %s\t%f\n",
               item.first.first.c_str(),
               item.first.second.c_str(),
               (double) item.second / unigram[item.first.first]
        );
    }

    for (auto item : unigram) {
        printf("%s\t%f\n",
                item.first.c_str(),
               (double) item.second / total
        );
    }

}

int main(int argc, const char* argv[])
{
    const char *filename = argv[1];
    train(filename);
    return 0;
}
