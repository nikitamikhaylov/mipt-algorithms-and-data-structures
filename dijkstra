#include <iostream>
#include <vector>
#include <set>
#include <limits>

const size_t INF = std::numeric_limits<size_t>::max();

template<typename T>
class IGraph {
public:
    virtual void AddEdge( size_t from, size_t to, T weight) = 0;
    virtual size_t Size() const = 0;
    virtual void GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const = 0;
};

template<typename T>
class CListGraph: public IGraph<T> {
public:
    explicit CListGraph(size_t _verticesCount);
    virtual void AddEdge(size_t from, size_t to, T weight) override;
    virtual size_t Size() const override { return (int)vertices.size(); };
    virtual void GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const override;
private:
    struct CVertex {
        std::vector<std::pair<size_t, T>> NextVertices;
    };
    std::vector<CVertex> vertices;
};

template<typename T>
CListGraph<T>::CListGraph(size_t _verticesCount)
{
    vertices.resize(_verticesCount);
}

template<typename T>
void CListGraph<T>::AddEdge(size_t from, size_t to, T weight)
{
    auto first = std::lower_bound(vertices[from].NextVertices.begin(), vertices[from].NextVertices.end(), std::make_pair(to, weight));
    vertices[from].NextVertices.insert(first, std::make_pair(to, weight));
}

template<typename T>
void CListGraph<T>::GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const
{
    output = vertices[vertex].NextVertices;
}


template<typename T, template<typename> class GraphType>
std::vector<T> CalcDijkstra(const GraphType<T>& graph, size_t start) {
    std::vector<size_t> distances(graph.Size(), INF);
    distances[start] = 0;
    std::set<std::pair<size_t,size_t> > queue;
    queue.insert (std::make_pair(distances[start], start));
    while (!queue.empty()) {
        size_t v = queue.begin()->second;
        queue.erase (queue.begin());
        std::vector<std::pair<size_t, T>> NextVertices;
        graph.GetNextVertices(v, NextVertices);
        for (size_t j = 0; j < NextVertices.size(); ++j) {
            size_t next = NextVertices[j].first, len = NextVertices[j].second;
            if (distances[v] + len < distances[next]) {
                queue.erase (std::make_pair(distances[next], next));
                distances[next] = distances[v] + len;
                queue.insert (std::make_pair(distances[next], next));
            }
        }
    }
    return distances;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    CListGraph<size_t> graph(n);
    size_t from, to, weight;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }
    size_t start, finish;
    std::cin >> start >> finish;
    std::vector<size_t> answer = CalcDijkstra(graph, start);
    std::cout << answer[finish] << std::endl;
}
