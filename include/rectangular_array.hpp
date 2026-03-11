#pragma once

#include "array.hpp"

namespace mtx::details {

template <typename T>
class RectangularArray {
  public:
    RectangularArray() = default;

    RectangularArray(std::size_t n_rows, std::size_t n_cols) : data_(n_rows, Array<T>(n_cols)) {}

    RectangularArray(std::size_t n_rows, std::size_t n_cols, const T& elem)
        : data_(n_rows, Array<T>(n_cols, elem)) {}

    RectangularArray(std::initializer_list<std::initializer_list<T>> ilist) {
        validate_rectangular(ilist);
        data_ = Array<Array<T>>(ilist.begin(), ilist.end());
    }

    template <std::forward_iterator Iter>
    RectangularArray(std::size_t n_rows, std::size_t n_cols, Iter elems_begin, Iter elems_end) {
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
    Array<T>& operator[](std::size_t idx) noexcept { return data_[idx]; }
    const Array<T>& operator[](std::size_t idx) const noexcept { return data_[idx]; }

    Array<T>* begin() noexcept { return data_.begin(); }
    const Array<T>* begin() const noexcept { return data_.begin(); }

    Array<T>* end() noexcept { return data_.end(); }
    const Array<T>* end() const noexcept { return data_.end(); }

  public:
    bool empty() const noexcept { return data_.empty(); }

    std::size_t n_rows() const noexcept { return data_.size(); }

    std::size_t n_cols() const noexcept {
        return empty() ? 0 : data_[0].size();
    }

  public:
    void swap_rows(std::size_t first_idx, std::size_t second_idx) noexcept {
        std::swap(data_[first_idx], data_[second_idx]);
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

  private:
    Array<Array<T>> data_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const RectangularArray<T>& array) {
    if (array.empty()) { return ostream; }

    auto last_it = array.end();
    --last_it;

    std::copy(array.begin(), last_it, std::ostream_iterator<Array<T>>(ostream, "\n"));
    ostream << *last_it;
    
    return ostream;
}

}; // namespace mtx::details
