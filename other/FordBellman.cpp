#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>

double INF = std::numeric_limits<double>::max();

template<typename T>
class IGraph {
public:
    virtual void AddEdge( size_t from, size_t to, T wetght) = 0;
    virtual size_t Size() const = 0;
    virtual void GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const = 0;
};

template<typename T>
class CMatrixGraph: public IGraph<T> {
public:
    explicit CMatrixGraph(size_t size);
    //CGraph
    virtual void AddEdge( size_t from, size_t to, T weight) override;
    virtual size_t Size() const override;
    virtual void GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const override;
private:
    std::vector<std::vector<T>> matrix;
};

template<typename T>
CMatrixGraph<T>::CMatrixGraph(size_t _verticesCount)
{
    matrix.resize(_verticesCount);
    for (int i = 0; i < _verticesCount; ++i) {
        matrix[i].resize(_verticesCount, NULL);
    }
}

template<typename T>
void CMatrixGraph<T>::AddEdge(size_t from, size_t to, T weight)
{
    matrix[from][to] = weight;
}

template<typename T>
size_t CMatrixGraph<T>::Size() const
{
    return matrix.size();
}

template<typename T>
void CMatrixGraph<T>::GetNextVertices(size_t vertex, std::vector<std::pair<size_t, T>> &output) const
{
    output.clear();
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[vertex][i]) {
            output.push_back(std::make_pair(i, matrix[vertex][i]));
        }
    }
}

template<typename T, template<typename> class GraphType>
std::vector<T> CalcFordBellman(const GraphType<T>& graph, size_t start)
{
    std::vector<T> result(graph.Size(), std::numeric_limits<T>::max());
    result[start] = 0;
    std::vector<std::pair<size_t, T>> edges;
    for (size_t i = 0; i < graph.Size(); ++i) {
        const bool isLast = (i == graph.Size() - 1);
        bool flag = true;
        for(size_t j = 0; j < graph.Size(); ++j) {
            graph.GetNextVertices(j, edges);
            for (size_t k = 0; k < edges.size(); k++) {
                const std::pair<size_t, T> & edge = edges[k] ;
                if (result[j] != std::numeric_limits<T>::max() && result[edge.first] > result[j] + edge.second) {
                    if (isLast){
                        return std::vector<T>();
                    }
                    result[edge.first] = result[j] + edge.second;
                    flag=false;
                }
            }
        }
        if (flag){
            break;
        }
    }
    return result;
}

int main()
{
    size_t n;
    std::cin >> n;
    CMatrixGraph<double> graph(n);
    for(size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < n; ++j) {
            if (i != j) {
                double x;
                std::cin >> x;
                if (x == -1){
                    graph.AddEdge(i, j, INF);
                }
                graph.AddEdge(i, j, log(1 / x));
            }
            else {
                graph.AddEdge(i, j, 0);
            }
        }
    }
    std::vector<double> result = CalcFordBellman(graph, 0);
    if (result.size() == 0) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
