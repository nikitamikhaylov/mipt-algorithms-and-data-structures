#include <algorithm>
#include <iostream>
#include <vector>

//Класс преобразований между префикс-функцией, z-функцией и строкой.
class PrefixAndZ {
public:
    //Метод для проверки всех преобразований.
    void Check(std::string& str);

    //Метод для получения ответа на задачу.
    std::string GetAnswer(const std::vector<size_t>& z_function);

private:
    void PrefixFromZ(std::vector<size_t>& prefix_function, const std::vector<size_t>& z_function);

    void ZFromPrefix(std::vector<size_t>& z_function, const std::vector<size_t>& prefix_function);

    //Метод выражается через другие по транзитивности. Асимптотика при этом не меняется.
    void PrefixFromString(std::vector<size_t>& prefix_function, const std::string& str);

    void ZFromString(std::vector<size_t>& z_function, const std::string& str);

    void StringFromPrefix(std::string& str, const std::vector<size_t>& prefix_function);

    //Получен так же по транзитивности.
    void StringFromZ(std::string& str, const std::vector<size_t>& z_function);
};

void PrefixAndZ::Check(std::string &str) {
    std::vector<size_t> prefix_function;
    std::vector<size_t> z_function;

    std::cerr << "ZFromString" << std::endl;
    ZFromString(z_function, str);

    std::cerr << "PrefixFromZ" << std::endl;
    PrefixFromZ(z_function, prefix_function);

    std::cerr << "StringFromPrefix" << std::endl;
    StringFromPrefix(str, prefix_function);

    std::cerr << str << std::endl;

    std::cerr << "PrefixFromString" << std::endl;
    PrefixFromString(prefix_function, str);

    std::cerr << "ZFromPrefix" << std::endl;
    ZFromPrefix(z_function, prefix_function);

    std::cerr << "StringFromZ" << std::endl;
    StringFromZ(str, z_function);

    std::cerr << str << std::endl;
}

std::string PrefixAndZ::GetAnswer(const std::vector<size_t>& z_function) {
    std::string answer;
    StringFromZ(answer, z_function);
    return answer;
}

void PrefixAndZ::PrefixFromZ(std::vector<size_t>& prefix_function, const std::vector<size_t>& z_function) {
    prefix_function.resize(z_function.size(), 0);
    for (size_t i = 1; i < z_function.size(); ++i) {
        if (z_function[i] != 0) {
            for (int j = z_function[i] - 1; j >= 0 && !(prefix_function[i + j]); j--) {
                prefix_function[i + j] = static_cast<size_t>(j + 1);
            }
        }
    }
}

void PrefixAndZ::ZFromPrefix(std::vector<size_t>& z_function, const std::vector<size_t>& prefix_function) {
    z_function.resize(prefix_function.size(), 0);
    for (size_t i = 1; i < prefix_function.size(); ++i) {
        if (prefix_function[i] > 0) {
            z_function[i - prefix_function[i] + 1] = prefix_function[i];
        }
    }
    z_function[0] = prefix_function.size();
    size_t i = 1;
    size_t tmp = 0;
    while (i < prefix_function.size()) {
        tmp = i;
        if (z_function[i] > 0) {
            for (size_t j = 1; j < z_function[i]; ++j) {
                if (z_function[i + j] > z_function[j]) {
                    break;
                }
                z_function[i + j] = std::min(z_function[j], z_function[i] - j);
                tmp = i + j;
            }
        }
        i = tmp + 1;
    }
}

void PrefixAndZ::PrefixFromString(std::vector<size_t>& prefix_function, const std::string& str) {
    std::vector<size_t> z_function;
    ZFromString(z_function, str);
    PrefixFromZ(prefix_function, z_function);
}

void PrefixAndZ::StringFromPrefix(std::string& str, const std::vector<size_t>& prefix_function) {
    str = "a";
    std::vector<char> possible_symbols_(prefix_function.size(), 'b');
    for (size_t i = 1; i < prefix_function.size(); ++i) {
        if (prefix_function[i] == 0) {
            str += possible_symbols_[i - 1];
            ++possible_symbols_[i - 1];
            continue;
        }
        str += str[prefix_function[i] - 1];
        possible_symbols_[i] = possible_symbols_[prefix_function[i] - 1];
    }
}

void PrefixAndZ::ZFromString(std::vector<size_t>& z_function, const std::string& str) {
    z_function.resize(str.size(), 0);
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        z_function[i] = std::max(static_cast<size_t>(0), std::min(right - i, z_function[i - left]));
        while (i + z_function[i] < str.size() &&
                str[z_function[i]] == str[i + z_function[i]]) {
            ++z_function[i];
        }
        if (i + z_function[i] >= right) {
            left = i;
            right = i + z_function[i];
        }
    }
}

void PrefixAndZ::StringFromZ(std::string& str, const std::vector<size_t>& z_function) {
    std::vector<size_t> prefix_function;
    PrefixFromZ(prefix_function, z_function);
    StringFromPrefix(str, prefix_function);
}


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    std::vector<size_t> function;
    size_t item;
    while (std::cin >> item) {
        function.push_back(item);
    }
    PrefixAndZ solution;
    std::string answer = solution.GetAnswer(function);
    std::cout << answer;

//Чтобы проверить все методы класса, нужно на вход подать строку и вызвать метод Check.
/*
    std::string s;
    std::cin >> s;
    PrefixAndZ solution;
    solution.Check(s);
*/
    return 0;
}