#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>

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
    void GetNextWeights(size_t vertex, std::vector<T> &output) const;
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

template<typename T>
void CMatrixGraph<T>::GetNextWeights(size_t vertex, std::vector<T> &output) const
{
    output.clear();
    for (int i = 0; i < matrix.size(); ++i) {
        output.push_back(matrix[vertex][i]);

    }
}


template<typename T, template<typename> class GraphType>
std::vector<std::vector<T>> CalcFloyd(const GraphType<T>& graph)
{
    std::vector<std::vector<T>> result(graph.Size());
    for(size_t i = 0; i < graph.Size(); ++i) {
        graph.GetNextWeights(i, result[i]);
    }
    for (int k = 0; k < graph.Size(); ++k)
        for (int i = 0; i < graph.Size(); ++i)
            for (int j = 0; j < graph.Size(); ++j)
                result[i][j] = std::min(result[i][j], result[i][k] + result[k][j]);
    return result;
}

int main()
{
    size_t n;
    std::cin >> n;
    CMatrixGraph<int> graph(n);
    for(size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < n; ++j) {
            int x;
            std::cin >> x;
            graph.AddEdge(i, j, x);
        }
    }
    std::vector<std::vector<int>> result = CalcFloyd(graph);

    for (size_t i = 0; i < n; ++i) {
        for(size_t j = 0; j < n; ++j) {
            std::cout << result[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
