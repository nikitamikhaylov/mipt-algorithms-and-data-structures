#include <iostream>
#include <vector>

class StringFromPrefix {
public:
    std::string GetAnswer(const std::vector<size_t>& prefix_function);
};

//Строит строку из префикс функции.
std::string StringFromPrefix::GetAnswer(const std::vector<size_t>& prefix_function) {
    std::string answer = "a";
    std::vector<char> possible_symbols_(prefix_function.size(), 'b');
    for (size_t i = 1; i < prefix_function.size(); ++i) {
        if (prefix_function[i] == 0) {
            answer += possible_symbols_[i - 1];
            ++possible_symbols_[i - 1];
            continue;
        }
        answer += answer[prefix_function[i] - 1];
        possible_symbols_[i] = possible_symbols_[prefix_function[i] - 1];
    }
    return answer;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    std::vector<size_t> prefix_function;
    size_t item;
    while (std::cin >> item) {
        prefix_function.push_back(item);
    }
    StringFromPrefix solution;
    std::string answer = solution.GetAnswer(prefix_function);
    std::cout << answer;
    return 0;
}