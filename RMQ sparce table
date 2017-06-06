#include <vector>
#include <algorithm>
#include <iostream>

struct Query
{
    int L, R;
};

class RMQ {
public:
    void answer(std::vector<int>& arr, int n, std::vector<Query>& q , int m);
private:
    void preprocess(std::vector<int>& arr, int n);
    int query(std::vector<int>& arr, int L, int R);
    std::vector<std::vector<std::pair < int, int>>> lookup;
};

void RMQ::preprocess(std::vector<int>& arr, int n)
{
    lookup.resize(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        lookup[i].resize(static_cast<size_t>(ceil(log2(n)) + 1));
    }
    for (int i = 0; i < n; i++) {
        lookup[i][0].first = i;
        lookup[i][0].second = i;
    }
    for (int j=1; (1<<j)<=n; j++) {
        for (int i=0; (i+(1<<j)-1) < n; i++) {
            std::pair<int, int> x = lookup[i][j-1];
            std::pair<int, int> y = lookup[i + (1<<(j-1))][j-1];
            std::vector<int> to_sort(4);
            to_sort[0] = x.first;
            to_sort[1] = y.first;
            to_sort[2] = x.second;
            to_sort[3] = y.second;
            std::sort(to_sort.begin(), to_sort.end(), [&arr](int a, int b)->bool{return arr[a] < arr[b];});
            lookup[i][j].first = to_sort[0];
            if (to_sort[0] != to_sort[1]) {
                lookup[i][j].second = to_sort[1];
            } else {
                lookup[i][j].second = to_sort[2];
            }
        }
    }
}

int RMQ::query(std::vector<int>& arr, int L, int R)
{
    int j = (int)log2(R-L+1);
    std::pair<int, int> x = lookup[L][j];
    std::pair<int, int> y = lookup[R-(1<<j)+1][j];
    std::vector<int> to_sort(4);
    to_sort[0] = x.first;
    to_sort[1] = y.first;
    to_sort[2] = x.second;
    to_sort[3] = y.second;
    std::sort(to_sort.begin(), to_sort.end(), [&arr](int a, int b)->bool{return arr[a] < arr[b];});
    if (to_sort[0] != to_sort[1]) {
        return arr[to_sort[1]];
    } else if (to_sort[0] != to_sort[2]){
        return arr[to_sort[2]];
    } else {
        return arr[to_sort[3]];
    }
}

void RMQ::answer(std::vector<int>& arr, int n, std::vector<Query>& q , int m)
{
    preprocess(arr, n);
    for (int i=0; i<m; i++) {
        int L = q[i].L - 1, R = q[i].R - 1;
        std::cout << query(arr, L, R) << std::endl;
    }
}

int main()
{
    int n, m;
    std::cin >> n >> m;
    std::vector<int> a(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    std::vector<Query> q(static_cast<size_t>(m));
    for (int i = 0; i < m; ++i) {
        std::cin >> q[i].L >> q[i].R;
    }
    RMQ algo;
    algo.answer(a, n, q, m);
    return 0;
}
