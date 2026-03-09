#include <iostream>

#include <Eigen/Dense>

int main () {
    std::size_t size = 0;
    std::cin >> size;

    Eigen::MatrixXd matrix(size, size);
    for (std::size_t i = 0; i < size; i++) {
        for (std::size_t j = 0; j < size; j++) {
            std::cin >> matrix(i, j);
        }
    }

    std::cout << matrix.determinant();
}
