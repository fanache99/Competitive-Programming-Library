// https://judge.yosupo.jp/problem/range_affine_range_sum
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
#include "math/modular.h"

static constexpr size_t MAXN = 500000;
using Mod = Modular<StaticModuloPolicy<998244353, unsigned int, MontgomeryModulo>>;

struct Function {
    Function() : a(1), b(0) {}
    Function(const Mod &a, const Mod &b) : a(a), b(b) {}

    Function(const Function &left, const Function &right) :
        a(left.a * right.a), b(right.a * left.b + right.b) {}

    Mod apply(const Mod &x) const {
        return a * x + b;
    }

    Mod a, b;
};

std::istream& operator>>(std::istream &in, Function &f) {
    in >> f.a >> f.b;
    return in;
}


struct Operation {
    explicit Operation(const Mod &a, const Mod &b) : f(a, b) {}
    Operation(const Function &f) : f(f) {}
    Operation() : f() {}
    Operation(const Operation &op1, const Operation &op2) : f(op1.f, op2.f) {}

    bool apply(SumData<Mod> &data, size_t left, size_t right) const {
        data.sum = f.a * data.sum + (right - left + 1) * f.b;
        return true;
    }

    Function f;
};

int main() {
    int n, q;
    cin >> n >> q;
    std::vector<Mod> v(n);
    cin >> v;
    SegmentTree<SumData<Mod>, Operation, StaticSize<2 * MAXN>::StaticAllocator> tree(v);
    for (int i = 0, type; i < q; i++) {
        cin >> type;
        if (type == 0) {
            size_t a, b;
            Function f;
            cin >> a >> b >> f;
            tree.update(a, b - 1, f);
        } else {
            size_t a, b;
            cin >> a >> b;
            std::cout << tree.query(a, b - 1).sum << "\n";
        }
    }
    return 0;
}