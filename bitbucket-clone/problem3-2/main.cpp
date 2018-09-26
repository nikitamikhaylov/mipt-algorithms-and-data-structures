#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

template <typename T>
class RadiusVector {
public:
    T x, y, z;
    //Нумерация точек с 1.
    size_t number;

    RadiusVector() = default;
    RadiusVector(T x_, T y_, T z_) : x(x_), y(y_), z(z_) { number = 0; }
    RadiusVector(T x_, T y_, T z_, size_t number_) : x(x_), y(y_), z(z_), number(number_) {};

    double Length() {
        return sqrt(x * x + y * y + z * z);
    }
};

template <typename T>
RadiusVector<T> operator - (const RadiusVector<T>& v1, const RadiusVector<T>& v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

template <typename T>
RadiusVector<T> operator + (const RadiusVector<T>& v1, const RadiusVector<T>& v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

template <typename T>
bool operator == (const RadiusVector<T>& v1, const RadiusVector<T>& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

template <typename T>
bool operator != (const RadiusVector<T>& v1, const RadiusVector<T>& v2) {
    return !(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

template <typename T>
bool operator < (const RadiusVector<T>& v, const RadiusVector<T>& u) {
    return atan2(v.y, v.x) < atan2(u.y, u.x);
}

//Векторное произведение
template <typename T>
RadiusVector<T> operator * (const RadiusVector<T>& v, const RadiusVector<T>& u) {
    T new_x = v.y * u.z - u.y * v.z;
    T new_y = -1 * (v.x * u.z - u.x * v.z);
    T new_z = v.x * u.y - u.x * v.y;
    return {new_x, new_y, new_z};
}


//Скалярное произведение
template <typename T>
T scalar(const RadiusVector<T>& v, const RadiusVector<T>& u) {
    return v.x * u.x + v.y * u.y + v.z * u.z;
}

template <typename T>
struct Triangle {
    Triangle() = default;
    Triangle(RadiusVector<T> a_, RadiusVector<T> b_, RadiusVector<T> c_): a(a_), b(b_), c(c_) {};
    RadiusVector<T> a, b, c;
};

template <typename T>
bool operator == (const Triangle<T>& first, const Triangle<T>& second) {
    return (first.a == second.a && first.b == second.b && first.c == second.c) ||
           (first.a == second.b && first.b == second.c && first.c == second.a) ||
           (first.a == second.c && first.b == second.a && first.c == second.b) ||
           (first.a == second.a && first.b == second.c && first.c == second.b) ||
           (first.a == second.c && first.b == second.b && first.c == second.a) ||
           (first.a == second.b && first.b == second.a && first.c == second.c);
}

template <typename T>
class ConvexHull3DGiftWrapping {
public:
    std::vector<std::vector<size_t>> GetAnswer(std::vector<RadiusVector<T>>& points);
private:
    RadiusVector<T> FindP_0(const std::vector<RadiusVector<T>>& points);
    RadiusVector<T> FindP_1(const std::vector<RadiusVector<T>>& points, RadiusVector<T> p_0);
    RadiusVector<T> FindP_2(const std::vector<RadiusVector<T>>& points, RadiusVector<T> p_0, RadiusVector<T> p_1);

    Triangle<T> FindFirstFace(const std::vector<RadiusVector<T>>& points);

    void FaceProcessing(const std::vector<RadiusVector<T>>& points,
                               std::vector<Triangle<T>>& to_do,
                               std::vector<Triangle<T>>& answer,
                               RadiusVector<T> a, RadiusVector<T> b, RadiusVector<T> c);

    double polar_angle_z (RadiusVector<T> first, RadiusVector<T> second);
    int PlaneEquation(int A, int B, int C, int D, RadiusVector<T> p);
    double cos_angle(RadiusVector<T> first, RadiusVector<T> second);
    bool IsUsed(const std::vector<Triangle<T>>& visited_faces, Triangle<T> face);
};

//Поиск точки с минимальной z-коорданатой
template <typename T>
RadiusVector<T> ConvexHull3DGiftWrapping<T>::FindP_0(const std::vector<RadiusVector<T>>& points) {
    auto min_point = RadiusVector<T>(std::numeric_limits<int>::max(),
                            std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for (auto curr: points) {
        if (curr.z < min_point.z) {
            min_point = curr;
        }
    }
    return min_point;
}

//Полярный угол с осью OZ
template <typename T>
double ConvexHull3DGiftWrapping<T>::polar_angle_z (RadiusVector<T> first, RadiusVector<T> second) {
    auto p = second - first;
    return acos(p.z/(sqrt(scalar(p, p))));
}

template <typename T>
RadiusVector<T> ConvexHull3DGiftWrapping<T>::FindP_1(const std::vector<RadiusVector<T>>& points, RadiusVector<T> p_0) {
    double max_angle = std::numeric_limits<double>::min();
    RadiusVector<T> max_angle_point{};
    for (auto curr: points) {
        if (curr != p_0 && polar_angle_z(p_0, curr) > max_angle) {
            max_angle = polar_angle_z(p_0, curr);
            max_angle_point = curr;
        }
    }
    return max_angle_point;
}

template <typename T>
int ConvexHull3DGiftWrapping<T>::PlaneEquation(int A, int B, int C, int D, RadiusVector<T> p) {
    return A * p.x + B * p.y + C * p.z + D;
}

template <typename T>
RadiusVector<T> ConvexHull3DGiftWrapping<T>::FindP_2(const std::vector<RadiusVector<T>>& points, RadiusVector<T> p_0, RadiusVector<T> p_1) {
    for (auto p_2: points) {
        if (p_2 == p_0 || p_2 == p_1) {
            continue;
        }
        int A = (p_1 - p_0).y * (p_2 - p_0).z - (p_2 - p_0).y * (p_1 - p_0).z;
        int B = -1 * ((p_1 - p_0).x * (p_2 - p_0).z - (p_2 - p_0).x * (p_1 - p_0).z);
        int C = (p_1 - p_0).x * (p_2 - p_0).y - (p_2 - p_0).x * (p_1 - p_0).y;
        int D = -1 * (p_0.x * ((p_1 - p_0).y * (p_2 - p_0).z - (p_2 - p_0).y * (p_1 - p_0).z)
                      - p_0.y * ((p_1 - p_0).x * (p_2 - p_0).z - (p_2 - p_0).x * (p_1 - p_0).z)
                      + p_0.z * ((p_1 - p_0).x * (p_2 - p_0).y - (p_2 - p_0).x * (p_1 - p_0).y));
        bool scalar_sign = false;
        for (auto curr : points) {
            if (curr == p_0 || curr == p_1 || curr == p_2) {
                continue;
            }
            scalar_sign = PlaneEquation(A, B, C, D, curr) > 0;
            break;
        }
        bool fail = false;
        for (auto curr : points) {
            if (curr == p_0 || curr == p_1 || curr == p_2) {
                continue;
            }
            if (PlaneEquation(A, B, C, D, curr) > 0 != scalar_sign) {
                fail = true;
                break;
            }
        }
        if (!fail) {
            return p_2;
        }
    }
    return RadiusVector<T>(0,0,0);
}

template <typename T>
double ConvexHull3DGiftWrapping<T>::cos_angle(RadiusVector<T> first, RadiusVector<T> second) {
    return scalar(first, second) / (first.Length() * second.Length());
}

template <typename T>
bool ConvexHull3DGiftWrapping<T>::IsUsed(const std::vector<Triangle<T>>& visited_faces, Triangle<T> face) {
    for (auto curr : visited_faces) {
        if (curr == face) {
            return true;
        }
    }
    return false;
}

template <typename T>
void ConvexHull3DGiftWrapping<T>::FaceProcessing(const std::vector<RadiusVector<T>>& points,
                        std::vector<Triangle<T>>& to_do,
                        std::vector<Triangle<T>>& answer,
                        RadiusVector<T> a, RadiusVector<T> b, RadiusVector<T> c) {
    auto basis_normal = (a - c) * (b - c);
    RadiusVector<T> current_normal{};
    double min_angle = -2;
    RadiusVector<T> min_point_ab{};
    //Обработка ребра ab
    for (auto curr : points) {
        if (curr == a || curr == b || curr == c) {
            continue;
        }
        current_normal = (a - curr) * (curr - b);
        double normal_angle = cos_angle(basis_normal, current_normal);
        if (normal_angle > min_angle) {
            min_angle = normal_angle;
            min_point_ab = curr;
        }
    }

    Triangle<T> abx{a, b, min_point_ab};
    if (!IsUsed(answer, abx)) {
        to_do.push_back(abx);
        answer.push_back(abx);
    }

    min_angle = -2;
    basis_normal = (a - b) * (a - c);
    RadiusVector<T> min_point_bc{};
    //Обработка ребра ab
    for (auto curr : points) {
        if (curr == a || curr == b || curr == c) {
            continue;
        }
        current_normal = (b - curr) * (curr - c);
        double normal_angle = cos_angle(basis_normal, current_normal);
        if (normal_angle > min_angle) {
            min_angle = normal_angle;
            min_point_bc = curr;
        }
    }

    Triangle<T> bcx{b, c, min_point_bc};
    if (!IsUsed(answer, bcx)) {
        to_do.push_back(bcx);
        answer.push_back(bcx);
    }

    min_angle = -2;
    basis_normal = (b - a) * (b - c);
    RadiusVector<T> min_point_ac{};

    //Обработка ребра ab
    for (auto curr : points) {
        if (curr == a || curr == b || curr == c) {
            continue;
        }
        current_normal =  (a - curr) * (curr - c);
        double normal_angle = cos_angle(basis_normal, current_normal);
        if (normal_angle > min_angle) {
            min_angle = normal_angle;
            min_point_ac = curr;
        }
    }

    Triangle<T> acx{a, c, min_point_ac};
    if (!IsUsed(answer, acx)) {
        to_do.push_back(acx);
        answer.push_back(acx);
    }
}

template <typename T>
Triangle<T> ConvexHull3DGiftWrapping<T>::FindFirstFace(const std::vector<RadiusVector<T>>& points) {
    auto p_0 = FindP_0(points);
    auto p_1 = FindP_1(points, p_0);
    auto p_2 = FindP_2(points, p_0, p_1);
    return Triangle<T>{p_0, p_1, p_2};
}

template <typename T>
std::vector<std::vector<size_t>> ConvexHull3DGiftWrapping<T>::GetAnswer(std::vector<RadiusVector<T>>& points) {
    std::vector<Triangle<T>> answer;
    std::vector<Triangle<T>> to_do;
    auto first_face = FindFirstFace(points);
    answer.push_back(first_face);
    to_do.push_back(first_face);
    while (!to_do.empty()) {
        auto current = to_do[to_do.size() - 1];
        to_do.pop_back();
        FaceProcessing(points, to_do, answer, current.a, current.b, current.c);
    }

    //Ориентация каждой грани
    for (auto& face : answer) {
        auto u = face.b - face.a;
        auto v = face.c - face.a;
        auto normal = u * v;
        bool fail = false;
        for (auto curr : points) {
            if (curr == face.a || curr == face.b || curr == face.c) {
                continue;
            }
            auto p = curr - face.a;
            if (scalar(normal, p) > 0) {
                fail = true;
                break;
            }
        }
        if (fail) {
            std::swap(face.b, face.c);
        }
    }

    std::vector<std::vector<size_t>> number_answer(answer.size());
    std::vector<size_t> x(3);
    size_t i = 0;
    for (auto face: answer) {
        x[0] = --face.a.number;
        x[1] = --face.b.number;
        x[2] = --face.c.number;
        size_t min_number = x[0];
        size_t iter = 0;
        for (size_t j(0); j < 3; ++j) {
            if (x[j] < min_number) {
                min_number = x[j];
                iter = j;
            }
        }
        if (iter == 1) {
            size_t tmp = x[0];
            x[0] = x[1];
            x[1] = x[2];
            x[2] = tmp;
        }
        if (iter == 2) {
            size_t tmp = x[2];
            x[2] = x[1];
            x[1] = x[0];
            x[0] = tmp;
        }
        number_answer[i++] = x;
    }
    std::sort(number_answer.begin(), number_answer.end());
    return number_answer;
}

signed main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    size_t m;
    std::cin >> m;
    size_t n;
    std::vector<RadiusVector<long long>> points;
    for (size_t i(0); i < m; ++i) {
        std::cin >> n;
        points.resize(n);
        for (size_t j(0); j < n; ++j) {
            std::cin >> points[j].x >> points[j].y >> points[j].z;
            points[j].number = j + 1;
        }
        ConvexHull3DGiftWrapping<long long> solution;
        auto convex_hull = solution.GetAnswer(points);
        std::cout << convex_hull.size() << std::endl;
        for (auto iter : convex_hull) {
            std::cout << "3 ";
            for (auto j : iter) {
                std::cout << j << ' ';
            }
            std::cout << "\n";
        }
    }
    return 0;
}