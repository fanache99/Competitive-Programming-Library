// https://codeforces.com/contest/61/problem/E

#include "bits/stdc++.h"
#include "util/io.h"

#ifdef LOCAL_PROJECT

std::string input_path() {
    std::string file = __FILE__;
    std::string directory = file.substr(0, file.rfind('/'));
    return directory + "/input.txt";
}

FileReader cin(input_path());
#else
StdinReader cin;
#endif

int main() {
    int n;
    std::vector<int> v;
    cin >> n >> v;
    return 0;
}