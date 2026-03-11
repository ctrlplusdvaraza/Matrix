#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "array.hpp"
#include "common.hpp"

namespace mtx {

template <std::floating_point T>
class Matrix {
  public:
    Matrix() = default;

    explicit Matrix(std::size_t size) : data_(size, Array<T>(size)) {}

    Matrix(std::size_t n_rows, std::size_t n_cols) : data_(n_rows, Array<T>(n_cols)) {}

    Matrix(std::size_t n_rows, std::size_t n_cols, const T& elem)
        : data_(n_rows, Array<T>(n_cols, elem)) {}

    Matrix(std::initializer_list<std::initializer_list<T>> ilist) {
        validate_rectangular(ilist);
        data_ = Array<Array<T>>(ilist.begin(), ilist.end());
    }

    template <std::forward_iterator Iter>
    Matrix(std::size_t n_rows, std::size_t n_cols, Iter elems_begin, Iter elems_end) {
        validate_elements_count(n_rows, n_cols, elems_begin, elems_end);
        data_ = Array<Array<T>>(n_rows, n_cols);
        auto it = elems_begin;
        for (std::size_t row_idx = 0; row_idx < n_rows; ++row_idx) {
            auto next_it = std::next(it, n_cols);
            data_[row_idx] = Array<T>(it, next_it);
            it = next_it;
        }
    }

  public:
    static Matrix<T> diag(std::size_t size, const T& value) {
        Matrix<T> result(size);

        for (std::size_t i = 0; i < size; i++) {
            result[i][i] = value;
        }

        return result;
    }

    static Matrix<T> identity(std::size_t size) { return diag(size, T(1.0)); }

  public:
    Array<T>& operator[](std::size_t idx) noexcept { return data_[idx]; }
    const Array<T>& operator[](std::size_t idx) const noexcept { return data_[idx]; }

    Array<T>* begin() noexcept { return data_.begin(); }
    const Array<T>* begin() const noexcept { return data_.begin(); }

    Array<T>* end() noexcept { return data_.end(); }
    const Array<T>* end() const noexcept { return data_.end(); }

  public:
    bool empty() const noexcept { return data_.empty(); }

    std::size_t n_rows() const noexcept { return data_.size(); }

    std::size_t n_cols() const noexcept { return empty() ? 0 : data_[0].size(); }

  public:
    void negate() {
        for (std::size_t i = 0; i < n_rows(); i++) {
            for (std::size_t j = 0; j < n_cols(); j++) {
                data_[i][j] *= T(-1.0);
            }
        }
    }

    void swap_rows(std::size_t first_idx, std::size_t second_idx) noexcept {
        std::swap(data_[first_idx], data_[second_idx]);
    }

  public:
    Matrix<T> transpose() const {
        Matrix<T> result(n_cols(), n_rows());

        for (std::size_t i = 0; i < n_rows(); i++) {
            for (std::size_t j = 0; j < n_cols(); j++) {
                result[j][i] = data_[i][j];
            }
        }

        return result;
    };

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
    static void validate_rectangular(std::initializer_list<std::initializer_list<T>> ilist) {
        if (ilist.empty()) { return; }

        std::size_t row_size = ilist.begin()->size();

        for (auto iter = ilist.begin() + 1; iter != ilist.end(); ++iter) {
            if (iter->size() != row_size) {
                throw std::invalid_argument("All rows must have the same length.");
            }
        }
    }

    template <std::forward_iterator Iter>
    static void validate_elements_count(std::size_t n_rows, std::size_t n_cols, Iter begin,
                                        Iter end) {
        std::size_t expected_elements = n_rows * n_cols;

        if (static_cast<std::size_t>(std::distance(begin, end)) != expected_elements) {
            throw std::invalid_argument("The number of elements does not match n_rows * n_cols.");
        }
    }

    // find row with max abs elem in column
    std::size_t get_row_max_value(std::size_t column, std::size_t start_row) const {
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

    void add_row_to_row(std::size_t dst_row_idx, std::size_t src_row_idx, const T& mul = T(1.0)) {
        data_[dst_row_idx] += data_[src_row_idx] * mul;
    }

  private:
    Array<Array<T>> data_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const Matrix<T>& matrix) {
    if (matrix.empty()) { return ostream; }

    auto last_it = matrix.end() - 1;

    std::copy(matrix.begin(), last_it, std::ostream_iterator<Array<T>>(ostream, "\n"));
    ostream << *last_it;

    return ostream;
}

} // namespace mtx
