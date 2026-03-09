#pragma once

#include "array.hpp"

namespace mtx::details {

template <typename T>
class RectangularArray {
  public:
    RectangularArray() = default;

    RectangularArray(std::initializer_list<std::initializer_list<T>> ilist)
        : data_(ilist.begin(), ilist.end()) {}

    RectangularArray(std::size_t n_rows, std::size_t n_cols) : data_(n_rows, Array<T>(n_cols)) {}

    RectangularArray(std::size_t n_rows, std::size_t n_cols, const T& elem)
        : data_(n_rows, Array<T>(n_cols, elem)) {}

    template <std::forward_iterator Iter>
    RectangularArray(std::size_t n_rows, std::size_t n_cols, Iter elems_begin, Iter elems_end)
        : data_(n_rows) {
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
        if (this->empty()) { return 0; }

        return data_[0].size();
    }

  public:
    void swap_rows(std::size_t first_idx, std::size_t second_idx) noexcept {
        std::swap(data_[first_idx], data_[second_idx]);
    }

  private:
    Array<Array<T>> data_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const RectangularArray<T>& array) {
    if (array.empty()) { return ostream; }

    std::copy(array.begin(), --(array.end()), std::ostream_iterator<Array<T>>(ostream, "\n"));
    ostream << *(--array.end());
    return ostream;
}

}; // namespace mtx::details
