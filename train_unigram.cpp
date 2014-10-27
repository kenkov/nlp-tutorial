#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <stack>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>


using namespace std;

void train(const char *filename) {
    map<string, int> unicount;

    ifstream ifs(filename);
    string str;
    int total = 0;
    while (getline(ifs, str)) {
        vector<string> v;
        boost::algorithm::split(v, str, boost::is_any_of(" "));
        v.push_back("</s>");
        for (auto word : v) {
            unicount[word]++;
            total++;
        }
    }
    for (auto kv : unicount) {
        //cout << kv.first << "\t" << (double)kv.second / total<< endl;
        printf("%s\t%f\n", kv.first.c_str(), (double)kv.second / total);
    }
}


int main(int argc, const char* argv[])
{
    const char *filename = argv[1];
    train(filename);
    return 0;
}
