#include <iostream>
#include "biginteger.h"

int main() {
    BigInteger a, b;
    std::cin >> a >> b;
    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
    std::cout << a * b << std::endl;
    std::cout << a / b << std::endl;
    return 0;
}

