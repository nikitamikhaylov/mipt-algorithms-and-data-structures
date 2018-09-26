#include <iostream>
#include <vector>
#include <string>

class Solution {
public:
    int64_t GetAnswer(const std::string& str);
private:
    void GetSuffixArray(const std::string& str);
    void GetLCPArray(const std::string& str);

    std::vector<size_t> suffix_arr;
    std::vector<size_t> lcp_arr;
    size_t alphabet_size = 256;
};

void Solution::GetSuffixArray(const std::string &str) {
    suffix_arr.assign(str.size(), 0);
    //Массив классов эквивалентности суффиксов.
    std::vector<size_t> classes(str.size());
    //Массив для временных данных.
    std::vector<size_t> buffer(str.size());
    //Массив для карманов сортировки.
    std::vector<size_t> buck(std::max(alphabet_size, (size_t)str.size()));
    //Первая итерация.
    for (char i : str) {
        buck[i]++;
    }
    //Преобразуем массив так, чтобы каждый элемент указывал на положение в массиве первой данной буквы
    size_t sum = 0;
    for (size_t i(0); i < alphabet_size; ++i) {
        sum += buck[i];
        buck[i] = sum - buck[i];
    }
    //Теперь заполним массив arr: Теперь в нем суффиксы отсортированы по первой букве
    for (size_t i(0); i < str.size(); ++i) {
        suffix_arr[buck[str[i]]++] = i;
    }
    //Теперь проставляем цвета: цвет увеличивается на 1 если следующая буква - другая
    classes[suffix_arr[0]] = 0;
    for (size_t i(1); i < str.size(); ++i) {
        classes[suffix_arr[i]] = classes[suffix_arr[i - 1]] + (str[suffix_arr[i]] != str[suffix_arr[i - 1]]);
    }

    size_t cn = classes[suffix_arr[str.size() - 1]] + 1;

    //Шаг второй - постепенное расширение подстрок
    //в начале цикла отсортированы подстроки длины l, а в конце - длины 2l

    for (size_t l = 1; l < str.size(); l *= 2)
    {
        //Обнуляем массив buck  и заполняем для сортировки по classes.

        buck.assign(str.size(), 0);

        for (size_t i(0); i < str.size(); ++i) {
            buck[classes[i]]++;
        }
        sum = 0;
        for (size_t i(0); i < cn; ++i) {
            sum += buck[i];
            buck[i] = sum - buck[i];
        }

        //Строим новый массив в buf (не забываем сдвинуть указатель по модулю на l влево), затем копируем его в arr.
        for (size_t i(0); i < str.size(); i++) {
            buffer[buck[classes[(suffix_arr[i] + str.size() - l) % str.size()]]++] =
                    (suffix_arr[i] + str.size() - l) % str.size();
        }
        suffix_arr = buffer;

        //Теперь перекрашиваем массив classes: заполняем массив buf,
        // увеличиваем цвет на единицу если один из цветов отличается, затем копируем
        buffer[suffix_arr[0]] = 0;
        for (size_t i(1); i < str.size(); i++) {
            buffer[suffix_arr[i]] = buffer[suffix_arr[i - 1]] +
                    (classes[suffix_arr[i]] != classes[suffix_arr[i - 1]] ||
                            classes[(suffix_arr[i] + l) % str.size()]
                            != classes[(suffix_arr[i - 1] + l) % str.size()]);
        }

        cn = buffer[suffix_arr[str.size() - 1]] + 1;
        classes = buffer;
    }
}


void Solution::GetLCPArray(const std::string& str){
    std::vector <size_t> back_suff_arr(str.size());
    for (size_t i(0); i < str.size(); ++i) {
        back_suff_arr[suffix_arr[i]] = i;
    }
    lcp_arr.assign(str.size(), 0);
    size_t z = 0;
    for (size_t i = 0; i < str.size(); i++) {
        size_t j = back_suff_arr[i];
        if (z != 0) {
            --z;
        }
        if (j != str.size() - 1) {
            while (z < str.size() && str[(suffix_arr[j] + z) % str.size()] ==
                                             str[(suffix_arr[j + 1] + z) % str.size()]) {
                z++;
            }
            lcp_arr[j + 1] = z;
        }
    }
}

int64_t Solution::GetAnswer(const std::string& str) {
    GetSuffixArray(str);
    GetLCPArray(str);
    int64_t answer = 0;
    for (size_t i(1); i < str.size(); ++i) {
        answer += str.size() - 1 - suffix_arr[i];
    }
    for (size_t i(0); i < str.size(); ++i) {
        answer -= lcp_arr[i];
    }
    return answer;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    std::string str;
    std::cin >> str;
    str += '$';
    Solution solve;
    std::cout << solve.GetAnswer(str);
    return 0;
}

