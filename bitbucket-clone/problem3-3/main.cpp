#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

struct RadiusVector {
    RadiusVector() : x(0), y(0) {}
    explicit RadiusVector(double x_, double y_) : x(x_), y(y_) {};

    //Длина вектора
    double Length() {
        return sqrt(x * x + y * y);
    }

    double x, y;
};

//Векторная сумма
RadiusVector operator + (const RadiusVector &v1, const RadiusVector &v2)
{
    return RadiusVector(v1.x + v2.x, v1.y + v2.y);
}

//Векторная разность
RadiusVector operator - (const RadiusVector &v1, const RadiusVector &v2)
{
    return RadiusVector(v1.x - v2.x, v1.y - v2.y);
}

//Векторное произведение
double operator * (const RadiusVector &v1, const RadiusVector &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

//Полярный угол между двумя векторами
double polar_angle (RadiusVector first, RadiusVector second)
{
    RadiusVector p = second - first;
    double alpha = atan2(p.y, p.x);
    if (alpha < 0) alpha += 2 * M_PI;
    return alpha;
}


//Проверка, лежит ли точка внутри многоугольника
bool CheckIfBelongs(const std::vector<RadiusVector>& polygon, RadiusVector point)
{
    double mult = (polygon[1] - polygon[0]) * (point - polygon[1]);
    bool sign = mult > 0;
    for (size_t i(2); i < polygon.size(); ++i) {
        mult = (polygon[i] - polygon[i - 1]) * (point - polygon[i]);
        if (sign != (mult > 0)) {
            return false;
        }
    }
    mult = (polygon[0] - polygon[polygon.size() - 1]) * (point - polygon[0]);
    return sign == (mult > 0);
}


//Проверка, есть ли пересечение между многоугольками
bool IsIntersection(std::vector<RadiusVector>& polygon1, std::vector<RadiusVector>& polygon2)
{
    //Вычисление суммы Минковского
    size_t iter1 = 0, iter2 = 0;
    std::vector<RadiusVector> answer;
    size_t n = polygon1.size();
    size_t m = polygon2.size();
    while ((iter1 < n ) || (iter2 < m)) {
        answer.push_back(polygon1[iter1] + polygon2[iter2]);
        double firstPolygonAngle = polar_angle(polygon1[iter1], polygon1[(iter1 + 1) % n]) ;
        double secondPolygonAngle = polar_angle(polygon2[iter2], polygon2[(iter2 + 1) % m]);
        if (firstPolygonAngle < secondPolygonAngle && iter1 != n) {
            ++iter1;
        } else {
            if (firstPolygonAngle > secondPolygonAngle && iter2 != m) {
                ++iter2;
            } else {
                ++iter1;
                ++iter2;
            }
        }
    }

    return CheckIfBelongs(answer, RadiusVector(0,0));
}

//Нахождение крайней левой точки и изменение порядка обхода.
void SortPolygon(std::vector<RadiusVector>& polygon)
{
    std::reverse(polygon.begin(), polygon.end());
    RadiusVector minPoint = polygon[0];
    size_t index = 0;
    for (size_t i(0); i < polygon.size(); ++i) {
        auto item = polygon[i];
        if (item.y < minPoint.y || (item.y == minPoint.y && item.x < minPoint.x)) {
            minPoint = item;
            index = i;
        }
    }
    std::rotate(polygon.begin(), polygon.begin() + index, polygon.end() );
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    size_t n, m;
    std::cin >> n;
    std::vector<RadiusVector> first(n);
    for (size_t i(0); i < n; ++i) {
        std::cin >> first[i].x >> first[i].y;
    }

    std::cin >> m;
    std::vector<RadiusVector> second(m);
    double x, y;
    for (size_t i(0); i < m; ++i) {
        std::cin >> x >> y;
        second[i].x = -1 * x;
        second[i].y = -1 * y;
    }

    //Не влияет на асимптотику, так как O(n)
    SortPolygon(first);
    SortPolygon(second);

    std::cout << (IsIntersection(first, second) ? "YES" : "NO");

    return 0;
}