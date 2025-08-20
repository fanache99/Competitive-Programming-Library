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

#include "bits/stdc++.h"
#include "structures/segmentTree.h"
#include "util/io.h"

struct Data {
    explicit Data() : value(0), step(0) {}

    template<typename T>
    Data(T value) : value(value) {}

    template<typename T>
    Data &operator=(const T &t) {
        value = t;
        step = 0;
        return *this;
    }

    Data(const Data &left, const Data &right) :
        value(left.step ? (left.value ^ right.value) : (left.value | right.value)),
        step(left.step ^ 1) {}

    int value;
    bool step = 0;  // 0 = OR, 1 = XOR
};

struct Update {
    explicit Update(int newValue) : newValue(newValue) {}

    bool apply(Data &data) const {
        data.value = newValue;
        return true;
    }

    int newValue;
};

struct Policy {
    static constexpr bool lazy = false;
    static constexpr bool sparse = false;
    static constexpr bool persistent = false;
};

int main() {
    int n, m;
    cin >> n >> m;
    n = 1 << n;
    std::vector<int> v(n);
    cin >> v;
    SegmentTree<Data, Update, Policy> tree(v);
    for (int i = 0; i < m; i++) {
        int p, b;
        cin >> p >> b;
        tree.update(p - 1, Update(b));
        std::cout << tree.query(0, n - 1).value << "\n";
    }
    return 0;
}