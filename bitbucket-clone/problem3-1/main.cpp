#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

struct Point {
    Point() = default;
    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {};
    double x, y, z;
};

class Segment {
public:
    Segment() = default;

    Segment(Point a, Point b) : a_(a), b_(b) {};

    //Длина отрезка
    double Length() {
        double x_diff = b_.x - a_.x;
        double y_diff = b_.y - a_.y;
        double z_diff = b_.z - a_.z;
        return sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
    }

    double ChangeFirstPoint(Point newLeft) {
        a_ = newLeft;
    }

    double ChangeSecondPoint(Point newRight) {
        b_ = newRight;
    }

    //Функция, которая разбивает отрезок на count равных частей и возвращает разделительные точки
    std::vector<Point> Split(size_t count) {
        count--;
        std::vector<Point> answer(count);
        double x_tmp, y_tmp, z_tmp;
        for (size_t iter(0); iter < count; ++iter) {
            size_t n = iter + 1;
            size_t m = count - iter;
            x_tmp = (m * a_.x + n * b_.x) / (n + m);
            y_tmp = (m * a_.y + n * b_.y) / (n + m);
            z_tmp = (m * a_.z + n * b_.z) / (n + m);
            answer[iter] = Point(x_tmp, y_tmp, z_tmp);
        }
        return answer;
    }
private:
    //Концы отрезка
    Point a_, b_;
};

class MinDistance {
public:
    MinDistance() = default;

    //Тернарный поиск, где функция - минимальное расстояние от выбранной точки до второго отрезка
    double DistanceBetweenSegments(Segment, Segment);
private:
    //Тернарный поиск, где функция - минимальное расстояние от выбранной точки до точки на первом отрезке
    double DistanceBetweenSegmentAndPoint(Point, Segment);

    //Расстояние между точками
    double DistanceBetweenPoints(Point, Point);

    //Точки, в которых достигается минимальное расстояние между отрезками
    Point first_point_, second_point_;
};

double MinDistance::DistanceBetweenPoints(Point first, Point second) {
    Segment tmp(first, second);
    return tmp.Length();
}

double MinDistance::DistanceBetweenSegmentAndPoint(Point point, Segment segment) {
    std::vector<Point> split_points;
    //Поскольку исходя из ограничений входных данных и точности, тернарный поиск сойдется за такое кол-во итераций.
    for (size_t i(0); i < 100; ++i){
        split_points = segment.Split(3);
        double f_a = DistanceBetweenPoints(split_points[0], point);
        double f_b = DistanceBetweenPoints(split_points[1], point);
        if (f_a < f_b) {
            segment.ChangeSecondPoint(split_points[1]);
        } else {
            segment.ChangeFirstPoint(split_points[0]);
        }
    }
    second_point_ = segment.Split(2)[0];
    return DistanceBetweenPoints(second_point_, point);
}

double MinDistance::DistanceBetweenSegments(Segment first, Segment second) {
    std::vector<Point> split_points;
    //Поскольку исходя из ограничений входных данных и точности, тернарный поиск сойдется за такое кол-во итераций.
    for (size_t i(0); i < 100; ++i) {
        split_points = first.Split(3);
        double f_a = DistanceBetweenSegmentAndPoint(split_points[0], second);
        double f_b = DistanceBetweenSegmentAndPoint(split_points[1], second);
        if (f_a < f_b) {
            first.ChangeSecondPoint(split_points[1]);
        } else {
            first.ChangeFirstPoint(split_points[0]);
        }
    }
    first_point_ = first.Split(2)[0];
    return DistanceBetweenPoints(first_point_, second_point_);
}

int main() {
    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::cin >> x1 >> y1 >> z1;
    std::cin >> x2 >> y2 >> z2;
    std::cin >> x3 >> y3 >> z3;
    std::cin >> x4 >> y4 >> z4;
    Segment first(Point(x1, y1, z1), Point(x2, y2, z2));
    Segment second(Point(x3, y3, z3), Point(x4, y4, z4));
    MinDistance solve();
    double answer = solve().DistanceBetweenSegments(first, second);
    std::cout << std::fixed << std::setprecision(8) << answer << std::endl;
    return 0;
}