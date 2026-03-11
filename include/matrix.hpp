#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cassert>

#include "common.hpp"
#include "rectangular_array.hpp"

namespace mtx {

template <std::floating_point T>
class Matrix {
  public: // constructors
    explicit Matrix(std::size_t size) : data_(size, size) {}

    Matrix(std::size_t n_rows, std::size_t n_cols) : data_(n_rows, n_cols) {}

    Matrix(std::size_t n_rows, std::size_t n_cols, const T& elem) : data_(n_rows, n_cols, elem) {}

    Matrix(std::initializer_list<std::initializer_list<T>> init_lists) : data_(init_lists) {}

    template <std::forward_iterator Iter>
    Matrix(std::size_t n_rows, std::size_t n_cols, Iter elems_begin, Iter elems_end)
        : data_(n_rows, n_cols, elems_begin, elems_end) {}

  public:
    static Matrix<T> diag(const std::size_t size, const T value) {
        Matrix<T> diag_matrix(size);
        for (std::size_t i = 0; i < size; i++) {
            diag_matrix[i][i] = value;
        }

        return diag_matrix;
    }

    static Matrix<T> identity(const std::size_t size) { return diag(size, T(1.0)); }

  public: // element access
    details::Array<T>& operator[](const std::size_t idx) { return data_[idx]; }

    const details::Array<T>& operator[](const std::size_t idx) const { return data_[idx]; }

  public: // capacity
    std::size_t n_rows() const { return data_.n_rows(); }
    std::size_t n_cols() const { return data_.n_cols(); }

  public: // math
    Matrix<T>& negate() {
        for (std::size_t i = 0; i < n_rows(); i++) {
            for (std::size_t j = 0; j < n_cols(); j++) {
                data_[i][j] *= T(-1.0);
            }
        }

        return *this;
    }

    Matrix<T> transpose() const {
        Matrix<T> res_matrix(n_cols(), n_rows());
        for (std::size_t i = 0; i < n_rows(); i++) {
            for (std::size_t j = 0; j < n_cols(); j++) {
                res_matrix[j][i] = data_[i][j];
            }
        }

        return res_matrix;
    };

    void swap_rows(const std::size_t fst_idx, const std::size_t snd_idx) {
        data_.swap_rows(fst_idx, snd_idx);
    }

    T determinant() const {
        bool flag_sign = 0;
        Matrix cur_matrix(*this);
        for (std::size_t row_idx = 0; row_idx < n_rows() - 1; ++row_idx) {
            std::size_t row_idx_max_value = cur_matrix.get_row_max_value(row_idx, row_idx);
            if (FloatingPointE<T>(cur_matrix.data_[row_idx_max_value][row_idx], T(0))) { return 0; }
            if (row_idx_max_value != row_idx) {
                flag_sign = !flag_sign;
                cur_matrix.swap_rows(row_idx_max_value, row_idx);
            }
            for (std::size_t row_idx_2 = row_idx + 1; row_idx_2 < n_rows(); ++row_idx_2) {
                cur_matrix.add_row_to_row(row_idx_2, row_idx,
                                          -cur_matrix.data_[row_idx_2][row_idx] /
                                              cur_matrix.data_[row_idx][row_idx]);
            }
        }
        T res = 1;
        for (std::size_t index = 0; index < n_rows(); ++index) {
            res *= cur_matrix.data_[index][index];
        }

        if (flag_sign && !FloatingPointE<T>(res, 0)) { return -res; }
        return res;
    }

  private:
    // find row with max abs elem in column
    std::size_t get_row_max_value(const std::size_t column, const std::size_t start_row) const {
        assert(column < n_cols());

        std::size_t result_row_idx = start_row;
        T result_value = std::fabs(data_[start_row][column]);

        for (std::size_t row_idx = start_row + 1; row_idx < n_rows(); ++row_idx) {
            T cur_val = std::fabs(data_[row_idx][column]);

            if (cur_val > result_value) {
                result_value = cur_val;
                result_row_idx = row_idx;
            }
        }

        return result_row_idx;
    }

    void add_row_to_row(const std::size_t dst_row_idx, const std::size_t src_row_idx,
                        const T& mul = T(1.0)) {
        assert(dst_row_idx < n_rows());
        assert(src_row_idx < n_rows());

        data_[dst_row_idx] += data_[src_row_idx] * mul;
    }

  private:
    details::RectangularArray<T> data_{};
};

template <FloatingPoint T>
std::ostream& operator<<(std::ostream& ostream, const Matrix<T>& matrix) {
    ostream << matrix.data();

    return ostream;
}

} // namespace mtx
