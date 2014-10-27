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


typedef map<string, double> perc;

perc create_feature(vector<string> words) {
    perc mp;
    for (auto word : words) {
        mp[word] += 1;
    }

    return mp;
}

bool pred(const double y, perc feature, perc theta) {
    double ans = 0;
    for (auto item : feature) {
        string word = item.first;
        double val = item.second;
        if (theta.count(word)) {
            ans += theta[word] * val;
        }
    }
    ans = ans * y;
    if (ans > 0) return true; else return false;
}


void train(char *filename) {
    ifstream datafs(filename);
    string line;
    perc theta;

    while(getline(datafs, line)) {
        vector<string> sp;
        //sscanf(line.c_str(), "%lf\t%s", &y, str);
        boost::algorithm::split(sp, line, boost::is_any_of("\t"));
        const double y = stod(sp[0]);

        vector<string> words;
        boost::algorithm::split(words, sp[1], boost::is_space());
        perc feature = create_feature(words);

        if (pred(y, feature, theta) == false) {
            for (auto item : feature) {
                string word = item.first;
                double val = item.second;
                theta[word] = theta[word] + val * y;

            }
        }
    }
    for (auto item : theta) {
        printf("UNI:%s\t%f\n", item.first.c_str(), item.second);
    }
}


int main(int argc, char *argv[])
{
    char *filename = argv[1];
    train(filename);
    return 0;
}

