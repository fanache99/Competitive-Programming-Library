#pragma GCC optimize("Ofast")

#include <string>
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

#include "math/polynomial.h"
#include "math/modular.h"


int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        cin >> n;
        int nr = -1;
        bool bad = false;
        for (int i = 0; i < n; i++) {
            int x;
            cin >> x;
            if (x != -1) {
                if (nr != -1 && nr != x) {
                    bad = true;
                }
                nr = x;
            }
        }
        if (!bad && nr != 0) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    return 0;
}