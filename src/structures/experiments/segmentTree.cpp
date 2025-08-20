#pragma GCC optimize("Ofast")

#include "structures/segmentTree.h"
#include "util/timer.h"
#include "util/io.h"

static constexpr size_t MAXN = 1000000;
static constexpr size_t MAXVAL = 1000000000;

long long tree[1 + 4 * MAXN];
int v[1 + MAXN];

void build(int node, int left, int right) {
    if (left == right) {
        tree[node] = v[left];
        return;
    }
    int middle = (left + right) / 2;
    build(2 * node, left, middle);
    build(2 * node + 1, middle + 1, right);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

void update(int node, int left, int right, int where, int add) {
    if (left == right) {
        tree[node] += add;
        return;
    }
    int middle = (left + right) / 2;
    if (where <= middle) {
        update(2 * node, left, middle, where, add);
    } else {
        update(2 * node + 1, middle + 1, right, where, add);
    }
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

long long query(int node, int left, int right, int from, int to) {
    if (from <= left && right <= to) {
        return tree[node];
    }
    int middle = (left + right) / 2;
    long long answer = 0;
    if (from <= middle) {
        answer += query(2 * node, left, middle, from, to);
    }
    if (middle + 1 <= to) {
        answer += query(2 * node + 1, middle + 1, right, from, to);
    }
    return answer;
}

void runRaw() {
    Timer t;
    int n = MAXN, q = MAXN;
    for (int i = 1; i <= n; i++) {
        v[i] = rand() % MAXVAL + 1;
    }
    build(1, 1, n);
    for (int i = 0, type, a, b; i < q; i++) {
        type = rand() % 2;
        if (type == 0) {
            a = rand() % n + 1;
            b = rand() % MAXVAL + 1;
            update(1, 1, n, a, b);
        } else {
            a = rand() % n + 1;
            b = rand() % n + 1;
            if (a > b) {
                std::swap(a, b);
            }
            query(1, 1, n, a, b);
        }
    }
    std::cout << "Raw: " << t.elapsed() << "\n";
}

struct Policy : public DefaultPolicy {
    static constexpr bool lazy = false;
};

template<typename T>
struct Data {
    Data() : sum(0) {}

    template<typename U>
    explicit Data(const U &u) : sum(u), size(1) {}

    template<typename U>
    Data<T> &operator=(const U &u) {
        sum = u;
        size = 1;
        return *this;
    }

    Data(const Data &left, const Data &right) : sum(left.sum + right.sum), size(left.size + right.size) {}

    T sum;
    int size;
};

template<typename T>
struct Add {
    Add() : add(0) {}

    explicit Add(const T &t) : add(t) {}

    Add(const Add &op1, const Add &op2) : add(op1.add + op2.add) {}

    bool apply(Data<T> &data) const {
        data.sum += add * data.size;
        return true;
    }

    T add;
};

void runComplex() {
    Timer t;
    int n = MAXN, q = MAXN;
    std::vector<int> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = rand() % MAXVAL + 1;
    }
    SegmentTree<Data<long long>, Add<long long>, Policy, StaticMemory<2 * MAXN>::Allocator> tree(v);
    for (int i = 0, type, a, b; i < q; i++) {
        type = rand() % 2;
        if (type == 0) {
            a = rand() % n;
            b = rand() % MAXVAL + 1;
            tree.update(a, Add<long long>(b));
        } else {
            a = rand() % n;
            b = rand() % n;
            if (a > b) {
                std::swap(a, b);
            }
            tree.query(a, b);
        }
    }
    std::cout << "Complex: " << t.elapsed() << "\n";
}

int main() {
    srand(0);
    runRaw();
    runComplex();
    return 0;
}