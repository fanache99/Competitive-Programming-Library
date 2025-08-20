// https://judge.yosupo.jp/problem/point_set_range_composite
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

    Mod apply(const Mod &x) {
        return a * x + b;
    }

    Mod a, b;
};

std::istream& operator>>(std::istream &in, Function &f) {
    in >> f.a >> f.b;
    return in;
}

struct Data {
    Data() = default;
    Data(const Function &f) : f(f) {}
    Data(const Data &left, const Data &right) : f(left.f, right.f) {}

    Function f;
};

struct Operation {
    Operation() = default;
    explicit Operation(const Mod &a, const Mod &b) : f(std::make_optional<Function>(a, b)) {}

    Operation(const Operation &op1, const Operation &op2) : f(op2.f) {}

    bool apply(Data &data, size_t left, size_t right) const {
        if (f) {
            assert(left == right);
            data.f = *f;
        }
        return true;
    }

    std::optional<Function> f;
};

int main() {
    int n, q;
    cin >> n >> q;
    std::vector<Function> v(n);
    cin >> v;
    SegmentTree<Data, Operation, StaticSize<2 * MAXN>::StaticAllocator> tree(v);
    for (int i = 0, type, a, b, p; i < q; i++) {
        cin >> type;
        if (type == 0) {
            cin >> p >> a >> b;
            tree.update(p, p, a, b);
        } else {
            cin >> a >> b >> p;
            std::cout << tree.query(a, b - 1).f.apply(p) << "\n";
        }
    }
    return 0;
}