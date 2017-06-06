#include <vector>
#include <algorithm>
#include <iostream>

class LCA_RMQ {
public:
    long long answer(int a_1, int a_2, long long x, long long y, long long z,
                     std::vector<std::pair<int, int>>& arr, int n, int m);
private:
    void preprocess(std::vector<std::pair<int, int>>& arr, int n);
    int query(std::vector<std::pair<int, int>>& arr, int L, int R);
    std::vector<std::vector<int>> lookup;
};

void LCA_RMQ::preprocess(std::vector<std::pair<int, int>>& arr, int n)
{
    lookup.resize(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        lookup[i].resize(static_cast<size_t>(ceil(log2(n)) + 1));
    }
    for (int i = 0; i < n; i++)
        lookup[i][0] = i;
    for (int j=1; (1<<j)<=n; j++) {
        for (int i=0; (i+(1<<j)-1) < n; i++) {
            if (arr[lookup[i][j-1]].second  < arr[lookup[i + (1<<(j-1))][j-1]].second)
                lookup[i][j] = lookup[i][j-1];
            else
                lookup[i][j] = lookup[i + (1 << (j-1))][j-1];
        }
    }
}

int LCA_RMQ::query(std::vector<std::pair<int, int>>& arr, int L, int R)
{
    if (L > R) {
        std::swap(L, R);
    }
    int j = (int)log2(R-L+1);
    if (arr[lookup[L][j]].second <= arr[lookup[R-(1<<j)+1][j]].second) {
        return arr[lookup[L][j]].first;
    } else {
        return arr[lookup[R-(1<<j)+1][j]].first;
    }
}

long long LCA_RMQ::answer(int a_1, int a_2, long long x, long long y, long long z,
                          std::vector<std::pair<int, int>>& arr, int n, int m) {
    preprocess(arr, arr.size());
    std::vector<int> first(n, -1);
    for(int i = 0; i < arr.size(); ++i) {
        if (first[arr[i].first] == -1) {
            first[arr[i].first] = i;
        }
    }
    std::vector<long long> aNumbers(4);
    aNumbers[0] = a_1;
    aNumbers[1] = a_2;
    long long answerSum = 0;
    int left, right;
    left = first[a_1];
    right = first[a_2];
    int previous = query(arr,left,right);
    answerSum += previous;
    for (int i = 1; i < m; ++i) {
        for (int j = 2; j < 4; ++j) {
            aNumbers[j] = (x * aNumbers[j - 2] + y * aNumbers[j - 1] + z) % n;
        }
        left = first[(aNumbers[2] + previous) % n];
        right = first[aNumbers[3]];
        previous = query(arr, left, right);
        answerSum += previous;
        for (int j = 2; j < 4; ++j) {
            aNumbers[j - 2] = aNumbers[j];
        }
    }
    return answerSum;
}

void dfs(int v, std::vector<std::vector<int>>& graph, int depth, std::vector<bool>& visited,
         std::vector<std::pair<int, int>>& queue) {
    visited[v] = true;
    queue.push_back(std::make_pair(v, depth));
    for (int i = 0; i < graph[v].size(); ++i) {
        if (!visited[graph[v][i]]) {
            dfs(graph[v][i], graph, depth + 1, visited, queue);
            queue.push_back(std::make_pair(v, depth));
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n);
    int parent;
    for (int i = 1; i < n; ++i) {
        std::cin >> parent;
        graph[parent].push_back(i);
    }
    std::vector<bool> visited(n);
    std::vector<std::pair<int, int>> queue;
    dfs(0, graph, 0, visited, queue);
    int a_1, a_2;
    int x, y, z;
    std::cin >> a_1 >> a_2;
    std::cin >> x >> y >> z;
    LCA_RMQ algo;
    std::cout << algo.answer(a_1, a_2, x, y, z, queue, n, m);
    return 0;
}
