#include <algorithm>
#include <iostream>
#include <vector>

//Класс-механизм для решения поставленной задачи.
class Solution {
public:
    void GetAnswer(std::string& pattern, std::string& text, std::vector<size_t>& answer);
private:
    std::vector<size_t> СalculatePrefixFunction(std::string& str);
};

//По заданной строке вычисляет префикс функцию.
std::vector<size_t> Solution::СalculatePrefixFunction(std::string& str) {
    std::vector<size_t> prefix(str.size(), 0);
    size_t k;
    for (size_t iter = 1; iter < str.size(); ++iter) {
        k = prefix[iter - 1];
        while (k > 0 && str[iter] != str[k]) {
            k = prefix[k - 1];
        }
        if (str[iter] == str[k]) {
            k++;
        }
        prefix[iter] = k;
    }
    return prefix;
}


//Получает ответ, результат кладет в вектор, ссылка на который была передана в качествве аргумента.
//Для получения ответа используется алгоритм Кнута-Морриса-Пратта.
void Solution::GetAnswer(std::string& pattern, std::string& text, std::vector<size_t>& answer) {
    std::vector<size_t> function = СalculatePrefixFunction(pattern);
    size_t k = 0;
    for (size_t iter = 0; iter < text.size(); ++iter) {
        //С помощью префикс функции делаем сдвиги шаблона относительно текста.
        while (k > 0 && pattern[k] != text[iter]) {
            k = function[k - 1];
        }
        if (pattern[k] == text[iter]) {
            ++k;
        }
        //Нашли подстроку.
        if (k == pattern.size()) {
            answer.push_back(iter - k + 1);
        }
    }
}

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    Solution solution;
    std::vector<size_t> answer;
    solution.GetAnswer(pattern, text, answer);
    for (auto item: answer) {
        std::cout << item << ' ';
    }
    return 0;
}