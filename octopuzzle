#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <utility>

class Octopus{
public:
    //конструктор от таблички
    Octopus( std::vector<std::vector< char>> prev);
    //возвращает все возможные конфигурации, которые могут быть достигнуты из нашей
    std::vector<std::pair<Octopus, char>> GetNextVertices();
    //функция получение эвристики
    size_t GetHeuristic();
    std::string howToGet;
    size_t len();
    std::vector<std::vector< char>> getSource() const {return source.field;}
    bool operator< ( const Octopus& ) const;
private:
    struct field {
        std::vector<std::vector< char>> field =
                std::vector<std::vector< char>> (3, std::vector< char> (3, 0));
    };
    field source;
    // смещение пустышки в разные стороны, если это возможно
    std::pair<Octopus, char> Up(size_t, size_t);
    std::pair<Octopus, char> Down(size_t, size_t);
    std::pair<Octopus, char> Left(size_t, size_t);
    std::pair<Octopus, char> Right(size_t, size_t);
    //получение манхэттенского расстояния
    size_t GetManhattan();
};

bool Octopus::operator< ( const Octopus& b) const {
    return this->getSource() < b.getSource();
}

Octopus::Octopus(std::vector<std::vector< char>> prev) {
    source.field = prev;
    howToGet = "";
}

size_t Octopus::GetHeuristic() {
    return GetManhattan();
}

size_t Octopus::GetManhattan(){
    size_t answer = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (source.field[i][j] == '1') {
                answer += abs(i) + abs(j);
            }
            if (source.field[i][j] == '2') {
                answer += abs(i) + abs(j - 1);
            }
            if (source.field[i][j] == '3') {
                answer += abs(i) + abs(j - 2);
            }
            if (source.field[i][j] == '4') {
                answer += abs(i - 1) + abs(j);
            }
            if (source.field[i][j] == '5') {
                answer += abs(i - 1) + abs(j - 1);
            }
            if (source.field[i][j] == '6') {
                answer += abs(i - 1) + abs(j - 2);
            }
            if (source.field[i][j] == '7') {
                answer += abs(i - 2) + abs(j);
            }
            if (source.field[i][j] == '8') {
                answer += abs(i - 2) + abs(j - 1);
            }
        }
    }
    return answer;
}

std::pair<Octopus, char> Octopus::Up(size_t i, size_t j) {
    field newField = source;
    std::swap(newField.field[i][j], newField.field[i - 1][j]);
    return std::make_pair(Octopus(newField.field), 'U');
}

std::pair<Octopus, char> Octopus::Down(size_t i, size_t j) {
    field newField = source;
    std::swap(newField.field[i][j], newField.field[i + 1][j]);
    return std::make_pair(Octopus(newField.field), 'D');
}

std::pair<Octopus, char> Octopus::Left(size_t i, size_t j) {
    field newField = source;
    std::swap(newField.field[i][j], newField.field[i][j - 1]);
    return std::make_pair(Octopus(newField.field), 'L');
}

std::pair<Octopus, char> Octopus::Right(size_t i, size_t j) {
    field newField = source;
    std::swap(newField.field[i][j], newField.field[i][j + 1]);
    return std::make_pair(Octopus(newField.field), 'R');
}

std::vector<std::pair<Octopus,  char>> Octopus::GetNextVertices() {
    std::vector<std::pair<Octopus,  char>> answer;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (source.field[i][j] == '0') {
                //нашли пустую клетку и перебираем все возможные конфигурации
                if (i > 0) {
                    answer.push_back(Up(i, j));
                }
                if (i < 2) {
                    answer.push_back(Down(i, j));
                }
                if (j > 0) {
                    answer.push_back(Left(i, j));
                }
                if (j < 2) {
                    answer.push_back(Right(i, j));
                }
                break;
            }
        }
    }
    return answer;
}

size_t Octopus::len() {
    if (howToGet.size() == 0) {
        return std::numeric_limits<size_t>::max() / 2;
    } else {
        if (howToGet == " ") {
            return 0;
        } else {
            return howToGet.size();
        }
    }
}

bool isReal(std::vector<std::vector< char>> a) {
    int inv = 0;
    for (int i = 0; i < 9; ++i)
        if (a[i / 3][i % 3] != '0') {
            for (int j = 0; j < i; ++j) {
                if (a[j / 3][j % 3] > a[i / 3][i % 3]) {
                    inv += 1;
                }
            }
        }
    return (inv % 2 == 0);
}

std::string Astar(Octopus start) {
    Octopus target({{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '0'}});
    std::string answer;
    std::set<std::pair<size_t, Octopus>> queue;
    start.howToGet = " "; //костыль
    queue.insert(std::make_pair(start.GetHeuristic(), start));
    while (!queue.empty()) {
        Octopus v = queue.begin()->second;
        queue.erase (queue.begin());
        if (v.getSource() == target.getSource()) {
            return v.howToGet;
        }
        std::vector<std::pair<Octopus, char>> NextVertices = v.GetNextVertices();
        for (size_t j = 0; j < NextVertices.size(); ++j) {
            Octopus next = NextVertices[j].first;
            size_t heuristicNext = next.GetHeuristic();
            char d  = NextVertices[j].second;
            if (next.len() > v.len() + 1) {
                queue.erase (std::make_pair(heuristicNext + next.len(), next));
                next.howToGet = v.howToGet + d;
                queue.insert (std::make_pair(heuristicNext + next.len(), next));
            }
        }
    }
}

int main() {
    freopen("puzzle.in", "r", stdin);
    freopen("puzzle.out", "w", stdout);
    std::vector<std::vector< char>> table =
            std::vector<std::vector< char>> (3, std::vector< char>(3, 0));
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j =0; j < 3; ++j) {
            std::cin >> table[i][j];
        }
    }
    Octopus start(table);
    if (isReal(start.getSource())) {
        std::string answer = Astar(start);
        std::cout << answer.size() - 1 << std::endl;
        std::cout << answer.substr(1, answer.size() - 1) << std::endl;
    }
    else {
        std::cout << "-1" << std::endl;
    }
    return 0;
}
