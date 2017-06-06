#include <iostream>
#include <limits>
#include <vector>
#include <unordered_map>
#include <queue>

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
void CListGraph<T>::print() {
    for (int i = 0; i < vertices.size(); ++i) {
        std::cout << i << ": ";
        for (int j = 0; j < vertices[i].nextVertices.size(); ++j) {
            std::cout << '(' << vertices[i].nextVertices[j].first << ',' << vertices[i].nextVertices[j].second << ") ";
        }
        std::cout << std::endl;
    }
}

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
    int getAnswer(std::vector<int>& cut);
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

int  Dinitza::getAnswer(std::vector<int>& cut) {
    int answer = findMaxFlow();
    cut = d;
    return answer;
}


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

void createGraph(std::string sample, std::string pattern) {
    std::unordered_map<int, int> sample_map;
    int countOfVertices = 1;
    for (int i = 0; i < sample.size(); ++i) {
        if (sample[i] == '?') {
            sample_map[i] = countOfVertices++;
        }
    }

    std::unordered_map<int, int> pattern_map;
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '?') {
            pattern_map[i] = countOfVertices++;
        }
    }
    countOfVertices++;
    CListGraph<int> graph(countOfVertices);
    //'0' - нулевая вершина
    //'1' - последняя вершина
    int answer = 0;
    for (int i = 0; i < sample.size() - pattern.size() + 1; ++i) {
        for (int j = 0; j < pattern.size(); ++j) {
            if (sample[i + j] == '?'  && pattern[j] == '?') {
                int x = sample_map[i + j];
                int y = pattern_map[j];
                graph.AddEdge(x, y, 1);
                graph.AddEdge(y, x, 1);
            } else {
                if (sample[i + j] == '?') {
                    if (pattern[j] == '0') {
                        int x = 0;
                        int y = sample_map[i + j];
                        if (!graph.Has(x, y)) {
                            graph.AddEdge(x, y, 1);
                            graph.AddEdge(y, x, 0);
                        } else {
                            graph.PlusEdge(x, y, 1);
                        }
                    } else {
                        int x = sample_map[i + j];
                        int y = countOfVertices - 1;
                        if (!graph.Has(x, y)) {
                            graph.AddEdge(x, y, 1);
                            graph.AddEdge(y, x, 0);
                        } else {
                            graph.PlusEdge(x, y, 1);
                        }
                    }
                }
                if (pattern[j] == '?') {
                    if (sample[i + j] == '0') {
                        int x = 0;
                        int y = pattern_map[j];
                        if (!graph.Has(x, y)) {
                            graph.AddEdge(x, y, 1);
                            graph.AddEdge(y, x, 0);
                        } else {
                            graph.PlusEdge(x, y, 1);
                        }
                    } else {
                        int x = pattern_map[j];
                        int y = countOfVertices - 1;
                        if (!graph.Has(x, y)) {
                            graph.AddEdge(x, y, 1);
                            graph.AddEdge(y, x, 0);
                        } else {
                            graph.PlusEdge(x, y, 1);
                        }
                    }
                }
            }
            if ((sample[i + j] == '0' && pattern[j] == '1') || (sample[i + j] == '1' && pattern[j] == '0')) {
                answer++;
            }
        }
    }
    std::unordered_map<int, int> reverse_sample_map;
    for (auto& x: sample_map) {
        reverse_sample_map[x.second] = x.first;
    }

    std::unordered_map<int, int> reverse_pattern_map;
    for (auto& x: pattern_map) {
        reverse_pattern_map[x.second] = x.first;
    }

    std::vector<int> cut;
    
    Dinitza algo(graph, 0, countOfVertices - 1);
    std::cout << answer + algo.getAnswer(cut) << std::endl;

    for (int i = 0; i < cut.size(); ++i) {
        if (cut[i] != INF) {
            if (reverse_sample_map.find(i) != reverse_sample_map.end()) {
                sample[reverse_sample_map[i]] = '0';
            }
            if (reverse_pattern_map.find(i) != reverse_pattern_map.end()) {
                pattern[reverse_pattern_map[i]] = '0';
            }
        }
    }
    
    for (int i = 0; i < sample.size(); ++i) {
        if (sample[i] == '?') {
            sample[i] = '1';
        }
    }

    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '?') {
            pattern[i] = '1';
        }
    }
    std::cout << sample << std::endl;
    std::cout << pattern << std::endl;

}

int main() {
    std::string sample, pattern;
    std::cin >> sample >> pattern;
    createGraph(sample, pattern);
    return 0;
}
