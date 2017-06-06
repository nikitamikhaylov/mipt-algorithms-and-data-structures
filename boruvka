#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

struct CEdge {
    size_t from;
    size_t to;
    size_t weight;
};

const size_t INF = std::numeric_limits<size_t>::max();

template<typename T>
class IGraph {
public:
    virtual void AddEdge( size_t from, size_t to, T weight) = 0;
    virtual size_t VerticesCount() const = 0;
    virtual void GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const = 0;
};

template<typename T>
class CListGraph: public IGraph<T> {
public:
    explicit CListGraph(size_t _verticesCount);
    virtual void AddEdge(size_t from, size_t to, T weight) override;
    virtual size_t VerticesCount() const override { return (int)vertices.size(); };
    size_t EdgesCount() const { return edges_count; };
    virtual void GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const override;
    size_t FindComp();
    std::vector<CEdge> GetEdges() const;
    size_t GetComp(size_t current) const {return components[current];};
    T GetWeight(size_t from, size_t to) const;
    bool Has(size_t from, size_t to) const;
    size_t SumOfWeight() const;
private:
    struct CVertex {
        std::vector<std::pair<size_t, T>> NextVertices;
    };
    std::vector<CVertex> vertices;
    size_t edges_count;
    std::vector<size_t> components;
    void dfs(size_t current, std::vector<bool>& visited, size_t lastComponent);
};

template<typename T>
CListGraph<T>::CListGraph(size_t _verticesCount)
{
    vertices.resize(_verticesCount);
    edges_count = 0;
}

template<typename T>
void CListGraph<T>::AddEdge(size_t from, size_t to, T weight)
{
    vertices[from].NextVertices.insert(vertices[from].NextVertices.end(), std::make_pair(to, weight));
    ++edges_count;
}

template<typename T>
void CListGraph<T>::GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const
{
    output = vertices[vertex].NextVertices;
}

template<typename T>
size_t CListGraph<T>::FindComp() {
    std::vector<bool> visited(vertices.size(), false);
    components.assign(vertices.size(), 0);
    size_t lastComponent = 0;
    for(size_t i = 0; i < vertices.size(); ++i) {
        if (!visited[i]) {
            dfs(i, visited, lastComponent);
            ++lastComponent;
        }
    }
    return lastComponent;
}

template<typename T>
void CListGraph<T>::dfs(size_t current, std::vector<bool>& visited, size_t lastComponent) {
    visited[current] = true;
    components[current] = lastComponent;
    std::vector<std::pair<size_t, T>> next;
    this->GetNextVertices(current, next);
    for(size_t i = 0; i < next.size(); ++i) {
        size_t v = next[i].first;
        if (!visited[v]) {
            components[v] = lastComponent;
            dfs(v, visited, lastComponent);
        }
    }
}

template<typename T>
std::vector<CEdge> CListGraph<T>::GetEdges() const {
    std::vector<CEdge> answer;
    for(size_t i = 0; i < vertices.size(); ++i) {
        for(size_t j = 0; j < vertices[i].NextVertices.size(); ++j) {
            CEdge v;
            v.from = i;
            v.to = vertices[i].NextVertices[j].first;
            v.weight = vertices[i].NextVertices[j].second;
            answer.push_back(v);
        }
    }
    return answer;
}

template<typename T>
T CListGraph<T>::GetWeight(size_t from, size_t to) const {
    for(size_t j = 0; j < vertices[from].NextVertices.size(); ++j) {
        if (vertices[from].NextVertices[j].first == to) {
            return vertices[from].NextVertices[j].second;
        }
    }
    return INF;
}

template<typename T>
bool CListGraph<T>::Has(size_t from, size_t to) const {
    for(size_t j = 0; j < vertices[from].NextVertices.size(); ++j) {
        if (vertices[from].NextVertices[j].first == to) {
            return true;
        }
    }
    return false;
}

template<typename T>
size_t CListGraph<T>::SumOfWeight() const {
    size_t  answer = 0;
    for(size_t i = 0; i < vertices.size(); ++i) {
        for(size_t j = 0; j < vertices[i].NextVertices.size(); ++j) {
            answer += vertices[i].NextVertices[j].second;
        }
    }
    return answer / 2;
}

template<typename T>
size_t Boruvka(CListGraph<T> graph) {
    size_t n = graph.VerticesCount();
    CListGraph<T> tree(n);
    while (tree.EdgesCount() < 2*(n - 1)){
        size_t countComponents = tree.FindComp();
        CEdge example;
        example.from = 0;
        example.to = 0;
        example.weight = INF;
        std::vector<CEdge> MinEdge(countComponents, example);
        std::vector<CEdge> edges = graph.GetEdges();
        for(size_t i = 0; i < edges.size(); ++i) {
            size_t u = edges[i].from;
            size_t v = edges[i].to;
            size_t weight = edges[i].weight;
            if (tree.GetComp(u) != tree.GetComp(v)) {
                if (MinEdge[tree.GetComp(u)].weight > weight) {
                    MinEdge[tree.GetComp(u)].from = u;
                    MinEdge[tree.GetComp(u)].to = v;
                    MinEdge[tree.GetComp(u)].weight = weight;
                }
                if (MinEdge[tree.GetComp(v)].weight > weight) {
                    MinEdge[tree.GetComp(v)].from = v;
                    MinEdge[tree.GetComp(v)].to = u;
                    MinEdge[tree.GetComp(v)].weight = weight;
                }
            }
        }
        for(size_t k = 0; k < countComponents; ++k) {
            if (!tree.Has(MinEdge[k].from, MinEdge[k].to)) {
                tree.AddEdge(MinEdge[k].from, MinEdge[k].to, MinEdge[k].weight);
                tree.AddEdge(MinEdge[k].to, MinEdge[k].from, MinEdge[k].weight);
            }
        }
    }
    return tree.SumOfWeight();
}

int main() {
	freopen("kruskal.in", "r", stdin);
    freopen("kruskal.out", "w", stdout);
    size_t n, m;
    std::cin >> n >> m;
    size_t x, y, w;
    CListGraph<size_t> graph(n);
    for(size_t i = 0; i < m; ++i) {
        std::cin >> x >> y >> w;
        --x;
        --y;
        if (!graph.Has(x, y) or (graph.Has(x, y) && graph.GetWeight(x, y) > w)) {
            graph.AddEdge(x, y, w);
            graph.AddEdge(y, x, w);
        }
    }
    size_t answer = Boruvka(graph);
    std::cout << answer;
    return 0;
}
