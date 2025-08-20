//https://www.infoarena.ro/problema/datorii
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

static constexpr size_t MAXN = 100000;

int main() {
    int n, q;
    cin >> n >> q;
    std::vector<int> v(n);
    cin >> v;
    SegmentTree<SumData<long long>, AddOperation<long long>, StaticSize<2 * MAXN>::StaticAllocator> tree(v);
    for (int i = 0, type, a, b; i < q; i++) {
        cin >> type >> a >> b;
        if (type == 0) {
            tree.update(a - 1, a - 1, -b);
        } else {
            std::cout << tree.query(a - 1, b - 1).sum << "\n";
        }
    }
    return 0;
}