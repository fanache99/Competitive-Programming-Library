// https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum
#ifdef NDEBUG
#pragma GCC optimize("Ofast")
#else
#endif

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

static constexpr size_t MAXN = 200000;
static constexpr long long MAX = std::numeric_limits<long long>::max();
static constexpr long long MIN = std::numeric_limits<long long>::min();

struct Data {
    Data() : sum(0), max1(MIN), max2(MIN), maxCnt(0), min1(MAX), min2(MAX), minCnt(0) {}
    Data(const long long &val) : sum(val), max1(val), max2(MIN), maxCnt(1), min1(val), min2(MAX), minCnt(1) {}

    Data(const Data &left, const Data &right) : sum(left.sum + right.sum) {
        if (left.max1 == right.max1) {
            max1 = left.max1;
            max2 = std::max(left.max2, right.max2);
            maxCnt = left.maxCnt + right.maxCnt;
        } else if (left.max1 > right.max1) {
            max1 = left.max1;
            max2 = std::max(left.max2, right.max1);
            maxCnt = left.maxCnt;
        } else {
            max1 = right.max1;
            max2 = std::max(left.max1, right.max2);
            maxCnt = right.maxCnt;
        }
        if (left.min1 == right.min1) {
            min1 = left.min1;
            min2 = std::min(left.min2, right.min2);
            minCnt = left.minCnt + right.minCnt;
        } else if (left.min1 < right.min1) {
            min1 = left.min1;
            min2 = std::min(left.min2, right.min1);
            minCnt = left.minCnt;
        } else {
            min1 = right.min1;
            min2 = std::min(left.min1, right.min2);
            minCnt = right.minCnt;
        }
    }

    long long sum;
    long long max1, max2, maxCnt;
    long long min1, min2, minCnt;
};

struct Operation {
    Operation() : min(MIN), max(MAX), add(0) {}
    Operation(long long min, long long max, long long add) : min(std::min(min, max)), max(max), add(add) {}
    Operation(const Operation &op1, const Operation &op2) : Operation(
        std::min(std::max(op1.min, op2.min == MIN ? MIN : op2.min - op1.add), op2.max == MAX ? MAX : op2.max - op1.add),
        std::min(std::max(op1.max, op2.min == MIN ? MIN : op2.min - op1.add), op2.max == MAX ? MAX : op2.max - op1.add),
        op1.add + op2.add
    ) {}

    bool apply(Data &data, size_t left, size_t right) const {
        if (max > data.max2 && min < data.min2) {
            if (min > data.min1) {
                data.sum -= data.min1 * data.minCnt;
                data.min1 = min;
                data.sum += data.min1 * data.minCnt;
                if (min > data.max1) {
                    data.max1 = min;
                } else if (min > data.max2) {
                    data.max2 = min;
                }
            }

            if (max < data.max1) {
                data.sum -= data.max1 * data.maxCnt;
                data.max1 = max;
                data.sum += data.max1 * data.maxCnt;
                if (max < data.min1) {
                    data.min1 = max;
                } else if (max < data.min2) {
                    data.min2 = max;
                }
            }

            if (add != 0) {
                data.sum += add * (right - left + 1);
                data.max1 += add;
                if (data.max2 != MIN) { data.max2 += add; }
                data.min1 += add;
                if (data.min2 != MAX) { data.min2 += add; }
            }

            return true;
        }
        return false;
    }

    static Operation addOperation(long long val) {
        return {MIN, MAX, val};
    }

    static Operation maxOperation(long long val) {
        return {val, MAX, 0};
    }

    static Operation minOperation(long long val) {
        return {MIN, val, 0};
    }

    long long min;
    long long max;
    long long add;
};

const int MAXVAL = 10;

int main() {
    int n, q;
    cin >> n >> q;
//    int n = 1000, q = 200000;
    srand(time(0));
    std::vector<long long> v(n);
    cin >> v;
//    for (auto &val : v) {
//        val = rand() % MAXVAL;
//        std::cout << val << " ";
//    }
//    std::cout << "\n";
//    auto pula = v;
    SegmentTree<Data, Operation, StaticSize<2 * MAXN>::StaticAllocator> tree(v);

//    auto print = [&]() {
//        for (int i = 0; i < n; i++) {
//            std::cout << i << ": " << v[i] << " " << tree.query(i, i).sum << "\n";
//            assert(v[i] == tree.query(i, i).sum);
//        }
//    };

    for (int i = 0, type, a, b; i < q; i++) {
        cin >> type >> a >> b;
//        type = rand() % 4;
//        a = rand() % n;
//        b = a + 1 + rand() % (n - a);
        if (type == 0) {
            long long x;
            cin >> x;
//            long long x = rand() % MAXVAL;
            tree.update(a, b - 1, Operation::minOperation(x));
//            for (int j = a; j < b; j++) {
//                v[j] = std::min(v[j], x);
//            }
//            std::cout << "Min " << a << " " << b - 1 << " " << x << "\n";
//            print();
        } else if (type == 1) {
            long long x;
            cin >> x;
//            long long x = rand() % MAXVAL;
            tree.update(a, b - 1, Operation::maxOperation(x));
//            for (int j = a; j < b; j++) {
//                v[j] = std::max(v[j], x);
//            }
//            std::cout << "Max " << a << " " << b - 1 << " " << x << "\n";
//            print();
        } else if (type == 2) {
            long long x;
            cin >> x;
//            long long x = rand() % MAXVAL;
            tree.update(a, b - 1, Operation::addOperation(x));
//            for (int j = a; j < b; j++) {
//                v[j] += x;
//            }
//            std::cout << "Add " << a << " " << b - 1 << " " << x << "\n";
//            print();
        } else {
            std::cout << tree.query(a, b - 1).sum << "\n";
//            long long fast = tree.query(a, b - 1).sum;
//            long long brute = 0;
//            for (int j = a; j < b; j++) {
//                brute += v[j];
//            }
//            std::cout << "Query " << a << " " << b - 1 << " " << fast << " " << brute << ":\n";
//            std::cout.flush();
//            for (auto x : v) {
//                std::cout << " " << x;
//            }
//            std::cout << "\n";
//            assert(fast == brute);
        }
    }
    return 0;
}