#pragma once

#include <optional>
#include <cassert>
#include <memory>
#include "util/alloc.h"
#include <iostream>

namespace {
    template<typename Operation, bool lazy>
    struct Lazy {
    };

    template<typename Operation>
    struct Lazy<Operation, true> {
        Operation lazy;
    };

    auto getMiddle(auto left, auto right) {
        return left + (right - left) / 2;
    }
}

struct DefaultPolicy {
    static constexpr bool lazy = true;
    static constexpr bool sparse = false;
    static constexpr bool persistent = false;
};

template<typename Data, typename Operation, typename Policy,
        template<typename> typename Allocator, typename IndexT>
class SegmentTree;

template<typename Policy>
concept Basic = !Policy::sparse && !Policy::persistent;

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
std::istream& operator>>(std::istream &in, SegmentTree<Data, Operation, Policy, Allocator, IndexT> &tree);

template<typename Data, typename Operation, Basic Policy, template<typename> typename Allocator, typename IndexT>
std::istream& operator>>(std::istream &in, SegmentTree<Data, Operation, Policy, Allocator, IndexT> &tree);

template<typename Data, typename Operation, typename Policy = DefaultPolicy,
        template<typename> typename Allocator = std::allocator, typename IndexT = uint32_t>
class SegmentTree {
    static constexpr bool lazy = Policy::lazy;
    static constexpr bool sparse = Policy::sparse;
    static constexpr bool persistent = Policy::persistent;

public:
    explicit SegmentTree(IndexT n);

    template<typename T>
    explicit SegmentTree(const std::vector<T> &v);

    void update(IndexT pos, const Operation &op);
    void update(IndexT left, IndexT right, const Operation &op) requires lazy;

    Data query(IndexT pos);
    Data query(IndexT left, IndexT right);
    friend std::istream &operator>><>(std::istream &in, SegmentTree<Data, Operation, Policy, Allocator, IndexT> &tree);

private:
    struct Node : public Lazy<Operation, Policy::lazy> {
        Node() = default;

        explicit Node(const Data &data, Node *leftSon = nullptr, Node *rightSon = nullptr) :
                data(data), leftSon(leftSon), rightSon(rightSon) {}

        Data data;
        Node *leftSon, *rightSon;
    };

    template<typename ...Args>
    Node *makeNode(Args &&...args);

    template<typename Init>
    void reset(Node *node, IndexT left, IndexT right, const Init &init, bool build);

    void updateImpl(Node *node, IndexT left, IndexT right, IndexT from, IndexT to, const Operation &op);

    Data queryImpl(Node *node, IndexT left, IndexT right, IndexT from, IndexT to);

    void push(Node *node, IndexT left, IndexT right);
    void extend(Node *node);

    bool apply(const Operation &op, Node *node, IndexT left, IndexT right);

    Allocator<Node> alloc;
    IndexT n;
    Node *root;
};

/* ----------------------------------------------IMPLEMENTATION------------------------------------------------------ */
template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
SegmentTree<Data, Operation, Policy, Allocator, IndexT>::SegmentTree(IndexT n) : n(n), root(makeNode()) {
    if constexpr (!sparse) {
        reset(root, 0, n - 1, [&](Data& data, IndexT pos) { return data = Data(); }, true);
    }
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
template<typename T>
SegmentTree<Data, Operation, Policy, Allocator, IndexT>::SegmentTree(const std::vector<T> &v) : n(v.size()), root(makeNode()) {
    reset(root, 0, n - 1, [&](Data& data, IndexT pos) { return data = Data(v[pos]); }, true);
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
void SegmentTree<Data, Operation, Policy, Allocator, IndexT>::update(IndexT pos, const Operation &op) {
    assert(pos >= 0 && pos < n);
    updateImpl(root, 0, n - 1, pos, pos, op);
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
void SegmentTree<Data, Operation, Policy, Allocator, IndexT>::update(IndexT left, IndexT right, const Operation &op) requires lazy {
    assert(left >= 0 && right < n);
    if (left > right) {
        return;
    }
    updateImpl(root, 0, n - 1, left, right, op);
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
Data SegmentTree<Data, Operation, Policy, Allocator, IndexT>::query(IndexT pos) {
    assert(pos >= 0 && pos < n);
    return queryImpl(root, 0, n - 1, pos, pos);
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
Data SegmentTree<Data, Operation, Policy, Allocator, IndexT>::query(IndexT left, IndexT right) {
    assert(left >= 0 && right < n);
    if (left > right) {
        return Data();
    }
    return queryImpl(root, 0, n - 1, left, right);
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
template<typename ...Args>
SegmentTree<Data, Operation, Policy, Allocator, IndexT>::Node *SegmentTree<Data, Operation, Policy, Allocator, IndexT>::makeNode(Args &&...args) {
    Node *ptr = alloc.allocate(1);
    std::construct_at(ptr, std::forward<Args>(args)...);
    return ptr;
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
template<typename Init>
void SegmentTree<Data, Operation, Policy, Allocator, IndexT>::reset(Node *node, IndexT left, IndexT right, const Init &init, bool build) {
    if constexpr (lazy) {
        node->lazy = Operation();
    }
    if (left == right) {
        init(node->data, left);
    } else {
        IndexT middle = getMiddle(left, right);
        if (build) {
            node->leftSon = makeNode();
            node->rightSon = makeNode();
        }
        reset(node->leftSon, left, middle, init, build);
        reset(node->rightSon, middle + 1, right, init, build);
        node->data = Data(node->leftSon->data, node->rightSon->data);
    }
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
void SegmentTree<Data, Operation, Policy, Allocator, IndexT>::extend(SegmentTree::Node *node) {
    if constexpr (sparse) {
        if (!node->leftSon) { node->leftSon = makeNode(); }
        if (!node->rightSon) { node->rightSon = makeNode(); }
    }
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
void SegmentTree<Data, Operation, Policy, Allocator, IndexT>::push(SegmentTree::Node *node, IndexT left, IndexT right) {
    if constexpr (lazy) {
        if (left != right) {
            auto middle = getMiddle(left, right);
            assert(apply(node->lazy, node->leftSon, left, middle));
            assert(apply(node->lazy, node->rightSon, middle + 1, right));
            node->lazy = Operation();
        }
    }
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
void
SegmentTree<Data, Operation, Policy, Allocator, IndexT>::updateImpl(SegmentTree::Node *node, IndexT left, IndexT right, IndexT from,
                                                    IndexT to, const Operation &op) {
    if (to < left || right < from) { return; }
    if (from <= left && right <= to && apply(op, node, left, right)) { return; }
    assert(left != right);
    extend(node);
    push(node, left, right);
    auto middle = getMiddle(left, right);
    updateImpl(node->leftSon, left, middle, from, to, op);
    updateImpl(node->rightSon, middle + 1, right, from, to, op);
    node->data = Data(node->leftSon->data, node->rightSon->data);
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
bool SegmentTree<Data, Operation, Policy, Allocator, IndexT>::apply(const Operation &op, SegmentTree::Node *node, IndexT left,
                                                    IndexT right) {
    if (op.apply(node->data, left, right)) {
        if constexpr (lazy) {
            node->lazy = Operation(node->lazy, op);
        }
        return true;
    } else {
        return false;
    }
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
Data SegmentTree<Data, Operation, Policy, Allocator, IndexT>::queryImpl(SegmentTree::Node *node, IndexT left, IndexT right, IndexT from,
                                                        IndexT to) {
    if (to < left || right < from) { return Data(); }
    if (from <= left && right <= to) { return node->data; }
    assert(left != right);
    extend(node);
    push(node, left, right);
    auto middle = getMiddle(left, right);
    return Data(queryImpl(node->leftSon, left, middle, from, to),
                queryImpl(node->rightSon, middle + 1, right, from, to));
}

template<typename Data, typename Operation, typename Policy, template<typename> typename Allocator, typename IndexT>
std::istream& operator>>(std::istream &in, SegmentTree<Data, Operation, Policy, Allocator, IndexT> &tree) {
    tree.reset(tree.root, 0, tree.n - 1, [&](Data &data, IndexT pos) { in >> data; }, false);
    return in;
}

///* -----------------------------------------------ITERATIVE---------------------------------------------------------- */
template<typename Data, typename Operation, Basic Policy, template<typename> typename Allocator, typename IndexT>
class SegmentTree<Data, Operation, Policy, Allocator, IndexT> {
    static constexpr bool lazy = Policy::lazy;
public:
    explicit SegmentTree(IndexT n) : SegmentTree(std::vector<Data>(n)) {}

    template<typename T>
    explicit SegmentTree(const std::vector<T> &v) : n(v.size()), size(std::bit_ceil(n)), log(std::countr_zero(size)), nodes(2 * size) {
        reset([&](Data &data, IndexT pos) { data = v[pos]; });
    }

    void update(IndexT pos, const Operation &op) {
        assert(pos >= 0 && pos < n);
        pos += size;
        op.apply(nodes[pos].data);
        for (IndexT i = 1; i <= log; i++) {
            refresh(pos >> i);
        }
    }

    void update(IndexT left, IndexT right, const Operation &op) requires lazy {
        assert(0 <= left && right < n);
        if (left > right) {
            return;
        }
        left += size, right += size + 1;
        for (IndexT i = log; i > 0; i--) {
            if (((left >> i) << i) != left) { push(left >> i); }
            if (((right >> i) << i) != right) { push((right - 1) >> i); }
        }
        for (IndexT l = left, r = right, from = left - size, to = right - size - 1, len = 1; l < r; l >>= 1, r >>= 1, len <<= 1) {
            if (l & 1) {
                apply(op, l++, from, from + len - 1);
                from += len;
            }
            if (r & 1) {
                apply(op, --r, right, right - len + 1);
                right -= len;
            }
        }
        for (IndexT i = 1; i <= log; i++) {
            if (((left >> i) << i) != left) { refresh(left >> i); }
            if (((right >> i) << i) != right) { refresh((right - 1) >> i); }
        }
    }

    Data query(IndexT pos) {
        assert(0 <= pos && pos < n);
        pos += size;
        for (IndexT i = log; i > 0; i--) {
            push(pos >> i);
        }
        return nodes[pos].data;
    }

    Data query(IndexT left, IndexT right) {
        assert(0 <= left && right < n);
        if (left > right) {
            return Data();
        }
        left += size, right += size + 1;
        for (IndexT i = log; i > 0; i--) {
            if (((left >> i) << i) != left) { push(left >> i); }
            if (((right >> i) << i) != right) { push((right - 1) >> i); }
        }
        Data prefix, suffix;
        for (; left < right; left >>= 1, right >>= 1) {
            if (left & 1) { prefix = Data(prefix, nodes[left++].data); }
            if (right & 1) { suffix = Data(nodes[--right].data, suffix); }
        }
        return Data(prefix, suffix);
    }

    friend std::istream &operator>>(std::istream &in, SegmentTree<Data, Operation, Policy, Allocator, IndexT> &tree) {
        tree.reset([&](Data &data, IndexT pos) { in >> data; });
        return in;
    }

private:
    struct Node : public Lazy<Operation, Policy::lazy> {
        Node() = default;

        explicit Node(const Data &data) : data(data) {}

        Data data;
    };

    template<typename Init>
    void reset(const Init &init) {
        if constexpr (lazy) {
            for (auto &node : nodes) {
                node.lazy = Operation();
            }
        }
        for (IndexT i = 0; i < n; i++) {
            init(nodes[size + i].data, i);
        }
        for (IndexT i = size - 1; i > 0; i--) {
            refresh(i);
        }
    }

    void refresh(IndexT node) {
        nodes[node].data = Data(nodes[leftSon(node)].data, nodes[rightSon(node)].data);
    }

    void push(IndexT node) {
        if constexpr (lazy) {
            apply(nodes[node].lazy, leftSon(node));
            apply(nodes[node].lazy, rightSon(node));
            nodes[node].lazy = Operation();
        }
    }

    bool apply(const Operation &op, IndexT node) {
        op.apply(nodes[node].data);
        nodes[node].lazy = Operation(nodes[node].lazy, op);
    }

    static IndexT leftSon(IndexT node) {
        return 2 * node;
    }

    static IndexT rightSon(IndexT node) {
        return 2 * node + 1;
    }

    IndexT n, size, log;
    std::vector<Node> nodes;
};

/* -------------------------------------------------HELPER----------------------------------------------------------- */
template<typename T>
struct SumData {
    SumData() : sum(0) {}

    template<typename U>
    explicit SumData(const U &u) : sum(u) {}

    SumData(const SumData &left, const SumData &right) : sum(left.sum + right.sum) {}

    T sum;
};

template<typename T>
std::istream& operator>>(std::istream &in, SumData<T> &data) {
    in >> data.sum;
    return in;
}

template<typename T>
struct MaxData {
    MaxData() : max(std::numeric_limits<T>::min()) {}

    template<typename U>
    explicit MaxData(const U &u) : max(u) {}

    MaxData(const MaxData &left, const MaxData &right) : max(std::max(left.max, right.max)) {}

    T max;
};

template<typename T>
std::istream& operator>>(std::istream &in, MaxData<T> &data) {
    in >> data.max;
    return in;
}

template<typename T>
struct MinData {
    MinData() : min(std::numeric_limits<T>::max()) {}

    template<typename U>
    explicit MinData(const U &u) : min(u) {}

    MinData(const MinData &left, const MinData &right) : min(std::min(left.min, right.min)) {}

    T min;
};

template<typename T>
std::istream& operator>>(std::istream &in, MinData<T> &data) {
    in >> data.min;
    return in;
}

template<typename T>
struct SetOperation {
    SetOperation() = default;

    explicit SetOperation(const T &t) : val(t) {}

    SetOperation(const SetOperation &op1, const SetOperation &op2) : val(op2.val) {}

    bool apply(SumData<T> &data, auto left, auto right) const {
        if (val) {
            data.sum = *val * (right - left + 1);
        }
        return true;
    }

    bool apply(MaxData<T> &data, auto, auto) const {
        if (val) {
            data.max = *val;
        }
        return true;
    }

    bool apply(MinData<T> &data, auto, auto) const {
        if (val) {
            data.min = *val;
        }
        return true;
    }

    std::optional<T> val;
};

template<typename T>
struct AddOperation {
    AddOperation() : add(0) {}

    explicit AddOperation(const T &t) : add(t) {}

    AddOperation(const AddOperation &op1, const AddOperation &op2) : add(op1.add + op2.add) {}

    bool apply(SumData<T> &data, auto left, auto right) const {
        data.sum += add * (right - left + 1);
        return true;
    }

    bool apply(MaxData<T> &data, auto, auto) const {
        data.max += add;
        return true;
    }

    bool apply(MinData<T> &data, auto, auto) const {
        data.min += add;
        return true;
    }

    T add;
};

