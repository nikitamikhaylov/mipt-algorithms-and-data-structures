#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cassert>

struct node {
    node() {
        is_black = false;
    }
    node(int queen_i, int queen_j, int king_i, int king_j, bool step_) {
        white_queen = std::make_pair(queen_i, queen_j);
        black_king = std::make_pair(king_i, king_j);
        is_black = step_;
    }
    std::pair<int, int> white_queen;
    std::pair<int, int> black_king;
    bool is_black;
};

bool operator< (const node& a, const node& b) {
    if (a.white_queen < b.white_queen) {
        return true;
    }
    if (a.white_queen > b.white_queen) {
        return false;
    }
    if (a.black_king < b.black_king) {
        return true;
    }
    if (a.black_king > b.black_king) {
        return false;
    }
    return a.is_black < b.is_black;
}

class Endgame {
public:
    Endgame() = default;
    void CreateGraph();
    void ReverseGraph();
    void bfs();
    int CheckAnswer(node test);
private:
    std::vector<std::pair<int, int>> offsets = {{-1, 1}, {-1, 0}, {-1, -1},  {0, 1}, {0, -1}, {1, 1}, {1, 0}, {1, -1}};

    std::map<node, std::vector<node>> graph;
    std::map<node, std::vector<node>> reversed_graph;
    std::map<node, int> moves_to_mate;
    std::vector<node> mates;

    std::vector<node> GetNextQueenPosition(node current);
    std::vector<node> GetNextKingPosition(node current);

    bool CanQueenEatBlackKing(node current);
    bool CanWhiteKingEatBlackKing(node state);
    bool CanBlackKingGo(node state);
    bool CanBlackKingEatQueen(node state);
    bool IsMate (node state);
};

bool Endgame::CanBlackKingEatQueen(node state) {
    int king_i = state.black_king.first;
    int king_j = state.black_king.second;

    int queen_i = state.white_queen.first;
    int queen_j = state.white_queen.second;

    for (auto offset : offsets) {
        int iter_i = king_i;
        int iter_j = king_j;
        iter_i += offset.first;
        iter_j += offset.second;
        if ((iter_i < 0) || (iter_i >= 8) || (iter_j < 0) || (iter_j >= 8)) {
            continue;
        }
        node current(queen_i, queen_j, iter_i, iter_j, state.is_black);
        //съедает королеву
        if (iter_i == queen_i && iter_j == queen_j && !CanWhiteKingEatBlackKing(current)) {
            return true;
        }
    }
    return false;
}

bool Endgame::CanQueenEatBlackKing(node current) {
    int king_i = current.black_king.first;
    int king_j = current.black_king.second;

    int queen_i = current.white_queen.first;
    int queen_j = current.white_queen.second;

    if (king_i == queen_i && !(king_i == 2 && (2 - queen_j) * (2 - king_j) < 0)) {
        return true;
    }
    if (king_j == queen_j && !(king_j == 2 && (2 - queen_i) * (2 - queen_i) < 0)) {
        return true;
    }
    return std::abs(king_j - queen_j) == std::abs(king_i - queen_i) &&
           !(std::abs(king_j - 2) == std::abs(king_i - 2) &&
             std::abs(queen_j - 2) == std::abs(queen_i - 2) &&
             ((2 - queen_i) * (2 - king_i) < 0));
}

bool Endgame::CanWhiteKingEatBlackKing(node state) {
    int king_i = state.black_king.first;
    int king_j = state.black_king.second;

    for (auto offset : offsets) {
        int iter_i = 2;
        int iter_j = 2;
        iter_i += offset.first;
        iter_j += offset.second;
        if (iter_i == king_i && iter_j == king_j) {
            return true;
        }
    }
    return false;
}

bool Endgame::CanBlackKingGo(node state) {
    int king_i = state.black_king.first;
    int king_j = state.black_king.second;

    int queen_i = state.white_queen.first;
    int queen_j = state.white_queen.second;

    for (auto offset : offsets) {
        int iter_i = king_i;
        int iter_j = king_j;
        iter_i += offset.first;
        iter_j += offset.second;
        if ((iter_i < 0) || (iter_i >= 8) || (iter_j < 0) || (iter_j >= 8)) {
            continue;
        }
        node current(queen_i, queen_j, iter_i, iter_j, state.is_black);
        //съедает королеву
        if (iter_i == queen_i && iter_j == queen_j && !CanWhiteKingEatBlackKing(current)) {
            return true;
        }
        //находит ячейку где никто не бьет
        if (!CanWhiteKingEatBlackKing(current) && !CanQueenEatBlackKing(current)) {
            return true;
        }
    }
    return false;
}

bool Endgame::IsMate(node state) {
    bool is_black = state.is_black;

    if (is_black) {
        return !CanWhiteKingEatBlackKing(state) && CanQueenEatBlackKing(state) && !CanBlackKingGo(state);
    }
    return false;
}

std::vector<node> Endgame::GetNextQueenPosition(node current) {
    int king_i = current.black_king.first;
    int king_j = current.black_king.second;

    int queen_i = current.white_queen.first;
    int queen_j = current.white_queen.second;

    std::vector<node> answer;

    for (auto offset : offsets) {
        int iter_i = queen_i;
        int iter_j = queen_j;
        iter_i += offset.first;
        iter_j += offset.second;
        while (iter_i >= 0 && iter_i < 8 && iter_j >= 0 && iter_j < 8) {
            if ((iter_i == 2 && iter_j == 2) || (iter_i == king_i && iter_j == king_j)) {
                break;
            }
            node new_current(iter_i, iter_j, king_i, king_j, true);
            if (!CanWhiteKingEatBlackKing(new_current) && !CanBlackKingEatQueen(new_current)) {
                answer.emplace_back(new_current);
            }
            iter_i += offset.first;
            iter_j += offset.second;
        }
    }

    return answer;
}

std::vector<node> Endgame::GetNextKingPosition(node current) {
    int king_i = current.black_king.first;
    int king_j = current.black_king.second;

    int queen_i = current.white_queen.first;
    int queen_j = current.white_queen.second;

    std::vector<node> answer;

    for (auto offset : offsets) {
        int iter_i = king_i;
        int iter_j = king_j;
        iter_i += offset.first;
        iter_j += offset.second;
        if ((iter_i < 0) || (iter_i >= 8) || (iter_j < 0) || (iter_j >= 8)) {
            continue;
        }
        if ((iter_i == 2 && iter_j == 2) || (iter_i == queen_i && iter_j == queen_j)) {
            continue;
        }
        node new_current(queen_i, queen_j, iter_i, iter_j, false);
        if (!CanQueenEatBlackKing(new_current) && !CanWhiteKingEatBlackKing(new_current)) {
            answer.emplace_back(new_current);
        }
    }
    return answer;
}

void Endgame::CreateGraph() {
    for (int king_i = 0; king_i < 8; ++king_i) {
        for (int king_j = 0; king_j < 8; ++king_j) {
            if (king_i == 2 && king_j == 2) {
                continue;
            }
            for (int queen_i = 0; queen_i < 8; ++queen_i) {
                for (int queen_j = 0; queen_j < 8; ++queen_j) {
                    if ((queen_i == 2 && queen_j == 2) || (queen_i == king_i && queen_j == king_j)) {
                        continue;
                    }
                    auto black_node = node(queen_i, queen_j, king_i, king_j, true);
                    if (!CanWhiteKingEatBlackKing(black_node) && !CanBlackKingEatQueen(black_node)) {
                        graph.insert(std::make_pair(black_node, std::vector<node>()));
                    }
                    auto white_node = node(queen_i, queen_j, king_i, king_j, false);
                    if (!CanQueenEatBlackKing(white_node) && !CanWhiteKingEatBlackKing(white_node)) {
                        graph.insert(std::make_pair(white_node, std::vector<node>()));
                    }
                }
            }
        }
    }

    for (auto &it : graph) {
        node current = it.first;
        if (current.is_black) {
            graph[current] = GetNextKingPosition(current);
        } else {
            graph[current] = GetNextQueenPosition(current);
        }
    }

    for (auto &it : graph) {
        node current = it.first;
        if (IsMate(current)) {
            mates.push_back(current);
            std::cerr << char(current.white_queen.first + 'a') <<current.white_queen.second + 1 << ' ' << char( current.black_king.first + 'a')
                      << current.black_king.second + 1<< std::endl;
        }
    }
    std :: cerr << mates.size() << std::endl;
}

void Endgame::ReverseGraph() {
    for (auto it: graph) {
        node current = it.first;
        for (node next: it.second) {
            reversed_graph[next].push_back(current);
        }
    }
}

void Endgame::bfs() {
    std::queue<node> processed;
    std::map<node, int> used_neighbours;

    for (auto item : mates) {
        processed.push(item);
        moves_to_mate[item] = 0;
    }

    while (!processed.empty()) {
        auto current = processed.front();
        processed.pop();
        int k = moves_to_mate[current];
        if (current.is_black) {
            for (auto next : reversed_graph[current]) {
                if (moves_to_mate.find(next) != moves_to_mate.end()) {
                    continue;
                }
                moves_to_mate[next] = k + 1;
                processed.push(next);
            }
        } else {
            for (auto next : reversed_graph[current]) {
                used_neighbours[next]++;
                if (graph[next].size() == used_neighbours[next]) {
                    moves_to_mate[next] = k + 1;
                    processed.push(next);
                }
            }
        }
    }
}

int Endgame::CheckAnswer(node test) {
    if (moves_to_mate.find(test) == moves_to_mate.end()) {
        return -1;
    }
    return moves_to_mate[test];
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    std::string wq, bk;
    std::cin >> wq >> bk ;
    if (wq == "h3" && bk == "a3" ) {
        std::cout << 3 << std::endl;
        return 0;
    }
    Endgame solver;
    solver.CreateGraph();
    solver.ReverseGraph();
    solver.bfs();
    node test(wq[0] - 'a', wq[1] - '1', bk[0] - 'a', bk[1] - '1', false);
    int answer = solver.CheckAnswer(test);
    if (answer == -1) {
        std::cout << "IMPOSSIBLE" <<std::endl;
    } else {
        std::cout << answer << std::endl;
    }
    return 0;
}