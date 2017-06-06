#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>

struct CNode {
    CNode *left;
    CNode *right;
    int priority;
    size_t size;
    std::string value;
    explicit CNode(const std::string value): left(nullptr), right(nullptr), value(value) {
        priority = rand();
        size = 1;
    }
};

class Treap {
public:
    Treap(): root(nullptr) {}
    ~Treap() { clear_tree(root); }
    void insert(int pos, const std::string &value);
    void remove(int pos);
    std::string getAt(int pos);
private:
    CNode *root;
    size_t getSize(CNode *Cnode) { return Cnode ? Cnode->size: 0; }
    void recalc(CNode *Cnode) { Cnode->size = 1 + getSize(Cnode->left) + getSize(Cnode->right); }
    CNode* Merge(CNode *left, CNode *right);
    std::pair<CNode*, CNode*> Split(CNode *Cnode, int k);
    void clear_tree(CNode *Cnode);
};

void Treap::insert(int pos, const std::string &value) {
    CNode *left, *right;
    std::tie(left, right) = Split(root, pos);
    root = Merge(Merge(left, new CNode(value)), right);
}

void Treap::remove(int pos) {
    CNode *left, *mid, *right;
    std::tie(left, right) = Split(root, pos + 1);
    std::tie(left, mid) = Split(left, pos);
    delete mid;
    root = Merge(left, right);
}

std::string Treap::getAt(int pos) {
    CNode *left, *mid, *right;
    std::tie(left, right) = Split(root, pos + 1);
    std::tie(left, mid) = Split(left, pos);
    std::string answer = mid->value;
    root = Merge(left, Merge(mid, right));
    return answer;
}

CNode* Treap::Merge(CNode *left, CNode *right) {
    if (!right)
        return left;
    if (!left)
        return right;
    if (left->priority > right->priority) {
        left->right = Merge(left->right, right);
        recalc(left);
        return left;
    } else {
        right->left = Merge(left, right->left);
        recalc(right);
        return right;
    }
}

std::pair<CNode*, CNode*> Treap::Split(CNode *node, int k) {
    if (!node)
        return {nullptr, nullptr};
    if (static_cast<int>(getSize(node->left)) < k) {
        CNode *left, *right;
        std::tie(left, right) = Split(node->right, k - 1 - getSize(node->left));
        node->right = left;
        recalc(node);
        return {node, right};
    } else {
        CNode *left, *right;
        std::tie(left, right) = Split(node->left, k);
        node->left = right;
        recalc(node);
        return {left, node};
    }
}

void Treap::clear_tree(CNode *Cnode) {
    if (Cnode) {
        clear_tree(Cnode->left);
        clear_tree(Cnode->right);
        delete Cnode;
    }
}

int main() {
    Treap tr;
    std::string x;
    int y;
    std::string s;
    int n;
    std::cin >> n;
    for(int i = 0; i < n; ++i) {
        std::cin >> x >> y;
        if (x == "+") {
            std::cin >> s;
            tr.insert(y, s);
        }
        if (x == "?") {
            std::cout << tr.getAt(y) << std::endl;
        }
        if (x == "-") {
            tr.remove(y);
        }
    }
}
