#include <iostream>
#include <climits>
#include <vector>

const int INIT_VALUE = 0;
const int NEUTRAL_VALUE = INT_MIN;
const int NEUTRAL_DELTA = 0;

class SegmentTree {
public:
    SegmentTree(int n, std::vector<int>& a);
    int query(int a, int b);
    void modify(int a, int b, int delta);
    void init(std::vector<int> &a);
private:
    int n;
    std::vector<int> value;
    std::vector<int> delta;

    void modify(int a, int b, int delta, int root, int left, int right);
    int query(int a, int b, int root, int left, int right);
    void init(int root, int left, int right, std::vector<int> &a);

    int joinValues(int leftValue, int rightValue);
    int joinDeltas(int oldDelta, int newDelta);
    int joinValueWithDelta(int value, int delta);
    void pushDelta(int root);
};
SegmentTree::SegmentTree(int n, std::vector<int>& a): n(n){
    int nMax= 1;
    while (nMax < n) {
        nMax <<= 1;
    }
    value.resize(static_cast<size_t>(2 * nMax));
    delta.resize(static_cast<size_t>(2 * nMax));
    init(a);
}

int SegmentTree::joinValues(int leftValue, int rightValue) {
    return std::max(leftValue, rightValue);
}

int SegmentTree::joinDeltas(int oldDelta, int newDelta) {
    return oldDelta + newDelta;
}

int SegmentTree::joinValueWithDelta(int value, int delta) {
    return value + delta;
}

void SegmentTree::init(int root, int left, int right, std::vector<int> &a) {
    if (left == right) {
        value[root] = left >= a.size() ? INIT_VALUE: a[left];
        delta[root] = NEUTRAL_DELTA;
    } else {
        init(2 * root + 1, left, (left + right) / 2, a);
        init(2 * root + 2, (left + right) / 2 + 1, right, a);
        value[root] = joinValues(value[2 * root + 1], value[2 * root + 2]);
        delta[root] = NEUTRAL_DELTA;
    }
}

void SegmentTree::init(std::vector<int> &a) {
    init(0, 0, n - 1, a);
}

void SegmentTree::pushDelta(int root) {
    delta[2 * root + 1] = joinDeltas(delta[2 * root + 1], delta[root]);
    delta[2 * root + 2] = joinDeltas(delta[2 * root + 2], delta[root]);
    value[2 * root + 1] = joinValueWithDelta(value[2 * root + 1], delta[root]);
    value[2 * root + 2] = joinValueWithDelta(value[2 * root + 2], delta[root]);
    delta[root] = NEUTRAL_DELTA;
}

int SegmentTree::query(int a, int b, int root, int left, int right) {
    if (a > right || b < left)
        return NEUTRAL_VALUE;
    if (a <= left && right <= b)
        return value[root];
    pushDelta(root);
    return joinValues(query(a, b, root * 2 + 1, left, (left + right) / 2),
                      query(a, b, root * 2 + 2, (left + right) / 2 + 1, right));
}

int SegmentTree::query(int a, int b) {
    return query(a, b, 0, 0, n - 1);
}

void SegmentTree::modify(int a, int b, int add, int root, int left, int right) {
    if (a > right || b < left)
        return;
    if (a <= left && right <= b) {
        delta[root] = joinDeltas(delta[root], add);
        value[root] = joinValueWithDelta(value[root], add);
        return;
    }
    pushDelta(root);
    modify(a, b, add, 2 * root + 1, left, (left + right) / 2);
    modify(a, b, add, 2 * root + 2, (left + right) / 2 + 1, right);
    value[root] = joinValues(value[2 * root + 1], value[2 * root + 2]);
}

void SegmentTree::modify(int a, int b, int add) {
    modify(a, b, add, 0, 0, n - 1);
}

int main() {
    int n;
    std::cin >> n;
    n--;
    std::vector<int> a(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    SegmentTree tree(n, a);
    int capacity;
    std::cin >> capacity;
    int m;
    std::cin >> m;
    int from, to, tickets;
    for(int i = 0; i < m; ++i) {
        std::cin >> from >> to >> tickets;
        to--;
        if (tree.query(from, to) + tickets > capacity) {
            std::cout << i << std::endl;
        } else {
            tree.modify(from, to, tickets);
        }
    }
    return 0;
}
