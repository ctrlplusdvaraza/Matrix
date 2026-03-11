#include <iostream>

#include "array.hpp"
#include "common.hpp"
#include "matrix.hpp"

int main() {
    std::size_t size = 0;
    if (!scan_until_next_line(std::cin, size)) {
        std::cerr << "failed to scan size\n";
        return 1;
    }

    mtx::Matrix<double> matrix(size);
    for (std::size_t i = 0; i < size; i++) {
        for (std::size_t j = 0; j < size; j++) {
            if (!scan_until_next_line(std::cin, matrix[i][j])) {
                std::cerr << "failed to scan matrix[" << i << "][" << j << "]" << std::endl;
                return 1;
            }
        }
    }

    std::cout << matrix.determinant() << std::endl;
}
