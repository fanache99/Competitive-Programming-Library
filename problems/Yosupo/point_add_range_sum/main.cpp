// https://judge.yosupo.jp/problem/point_add_range_sum
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

#include "structures/segmentTree.h"

static constexpr size_t MAXN = 500000;

struct Policy : public DefaultPolicy {
    static constexpr bool lazy = false;
};

int main() {
    int n, q;
    cin >> n >> q;
    SegmentTree<SumData<long long>, AddOperation<long long>, Policy, StaticMemory<2 * MAXN>::Allocator> tree(n);
    cin >> tree;
    for (int i = 0, type, a, b; i < q; i++) {
        cin >> type >> a >> b;
        if (type == 0) {
            tree.update(a, AddOperation<long long>(b));
        } else {
            std::cout << tree.query(a, b - 1).sum << "\n";
        }
    }
    return 0;
}