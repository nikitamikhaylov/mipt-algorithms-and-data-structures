#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();

template<typename T>
class IGraph {
public:
    virtual void AddEdge( int from, int to, T weight) = 0;
    virtual int VerticesCount() const = 0;
    virtual void GetNextVertices(int vertex, std::vector<int> &output) const = 0;
};

template<typename T>
class CListGraph: public IGraph<T> {
public:
    explicit CListGraph(int _verticesCount);
    CListGraph(const CListGraph<T>& other);
    virtual void AddEdge(int from, int to, T weight) override;
    void PlusEdge(int from, int to, T weight);
    void print();
    virtual int VerticesCount() const override { return (int)vertices.size(); };
    int EdgesCount() const { return edges_count; };
    virtual void GetNextVertices(int vertex, std::vector<int> &output) const override;
    T GetCapacity(int from, int to) const;
    bool Has(int from, int to) const;
private:
    struct CVertex {
        std::vector<int> nextVertices;
    };
    std::vector<CVertex> vertices;
    std::vector<std::vector<T>> capacities;
    int edges_count;
};

template<typename T>
CListGraph<T>::CListGraph(int _verticesCount)
{
    capacities.resize(_verticesCount);
    for (int i = 0; i < _verticesCount; ++i) {
        capacities[i].assign(_verticesCount, INF);
    }
    vertices.resize(_verticesCount);
    edges_count = 0;
}

template<typename T>
CListGraph<T>::CListGraph(const CListGraph<T>& other)
{
    int _verticesCount = other.VerticesCount();
    capacities.resize(_verticesCount);
    for (int i = 0; i < _verticesCount; ++i) {
        capacities[i].assign(_verticesCount, INF);
    }
    vertices.resize(_verticesCount);
    for (int i = 0; i < _verticesCount; i++) {
        other.GetNextVertices(i, vertices[i].nextVertices);
        for (int j = 0; j < vertices[i].nextVertices.size(); ++j) {
            capacities[i][vertices[i].nextVertices[j]] = other.GetCapacity(i, vertices[i].nextVertices[j]);
        }
    }
    edges_count = other.EdgesCount();
}

template<typename T>
void CListGraph<T>::AddEdge(int from, int to, T weight)
{
    vertices[from].nextVertices.insert(vertices[from].nextVertices.end(), to);
    capacities[from][to] = weight;
    ++edges_count;
}

template<typename T>
void CListGraph<T>::PlusEdge(int from, int to, T weight)
{
    capacities[from][to] += weight;
}

template<typename T>
void CListGraph<T>::GetNextVertices(int vertex, std::vector<int> &output) const
{
    output = vertices[vertex].nextVertices;
}

template<typename T>
T CListGraph<T>::GetCapacity(int from, int to) const {
    return capacities[from][to] == INF? 0 : capacities[from][to];
}

template<typename T>
bool CListGraph<T>::Has(int from, int to) const {
    return !(capacities[from][to] == INF);
}

class Dinitza {
public:
    Dinitza(CListGraph<int>& other, int start, int finish);
    int getAnswer() {return findMaxFlow();};
private:
    CListGraph<int>& graph;
    int verticesCount;
    int edgesCount;
    std::vector<std::vector<int>> flow;
    std::vector<int> p;
    std::vector<int> d;
    int start, finish;
    bool bfs();
    int dfs(int u, int minCapacity);
    int findMaxFlow();
};

Dinitza::Dinitza(CListGraph<int>& other, int start, int finish):
        graph(other), verticesCount(other.VerticesCount()), edgesCount(other.EdgesCount()), start(start), finish(finish) {
    flow.resize(verticesCount);
    for(int i = 0; i < verticesCount; ++i) {
        flow[i].assign(verticesCount, 0);
    }
    p.resize(verticesCount);
    d.resize(verticesCount);
};


bool Dinitza::bfs() {
    d.assign(verticesCount, INF);
    d[start] = 0;
    std::queue<int> queue;
    queue.push(start);
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        std::vector<int> next;
        graph.GetNextVertices(u, next);
        for(int i = 0; i < next.size(); ++i){
            int v = next[i];
            int capacity = graph.GetCapacity(u, next[i]);
            if (flow[u][v] < capacity && d[v] == INF) {
                d[v] = d[u] + 1;
                queue.push(v);
            }
        }
    }
    return d[finish] != INF;
}

int Dinitza::dfs(int u, int minCapacity) {
    if (u == finish || minCapacity == 0) {
        return minCapacity;
    }
    for(int v = p[u]; v < verticesCount; ++v) {
        if (d[v] == d[u] + 1) {
            int delta = dfs(v, std::min(minCapacity, graph.GetCapacity(u, v) - flow[u][v]));
            if (delta != 0) {
                flow[u][v] += delta;
                flow[v][u] -= delta;
                return delta;
            }
        }
        p[u]++;
    }
    return 0;
}

int Dinitza::findMaxFlow() {
    int maxFlow = 0;
    while(bfs()) {
        p.assign(verticesCount, 0);
        int flow = dfs(start, INF);
        while (flow != 0) {
            maxFlow += flow;
            flow = dfs(start, INF);
        }
    }
    return maxFlow;
};

int main() {
    freopen("maxflow.in", "r", stdin);
    freopen("maxflow.out", "w", stdout);
    int n, m;
    std::cin >> n >> m;
    int x, y, w;
    CListGraph<int> graph(n);
    for(int i = 0; i < m; ++i) {
        std::cin >> x >> y >> w;
        --x;
        --y;
        if (!graph.Has(x, y)) {
            graph.AddEdge(x, y, w);
            graph.AddEdge(y, x, 0);
        } else {
            graph.PlusEdge(x, y, w);
        }
    }
    Dinitza calcDinitza(graph, 0, n - 1);
    std::cout << calcDinitza.getAnswer() << std::endl;
    return 0;
}
