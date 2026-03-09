#pragma once

#include "array.hpp"

namespace mtx {

template <typename T>
class RectangularArray {
  public:
    RectangularArray() = default;

    RectangularArray(std::initializer_list<std::initializer_list<T>> ilist) : data_(ilist.size()) {
        for (std::size_t i = 0; i < ilist.size(); ++i) {
            data_[i] = Array<T>(ilist[i]);
        }
    }

    RectangularArray(std::size_t n_rows, std::size_t n_cols, const T& elem = T{}) : data_(n_rows) {
        for (std::size_t row_idx = 0; row_idx < n_rows; ++row_idx) {
            data_[row_idx].resize(n_cols, elem);
        }
    }

    template <typename Iter>
        requires IteratorOf<Iter, T>
    RectangularArray(std::size_t n_rows, std::size_t n_cols, Iter elems_begin, Iter elems_end)
        : RectangularArray(n_rows, n_cols) {
        fill_from_iter(elems_begin, elems_end);
    }

  public:
    Array<T>& operator[](std::size_t idx) { return data_[idx]; }
    const Array<T>& operator[](std::size_t idx) const { return data_[idx]; }

    Array<T>* begin() { return data_.begin(); }
    const Array<T>* begin() const { return data_.begin(); }

    Array<T>* end() { return data_.end(); }
    const Array<T>* end() const { return data_.end(); }

  public:
    bool empty() const { return data_.empty(); }

    std::size_t n_rows() const { return data_.size(); }

    std::size_t n_cols() const {
        if (this->empty()) { return 0; }

        return data_[0].size();
    }

  public:
    void swap_rows(std::size_t first_idx, std::size_t second_idx) {
        std::swap(data_[first_idx], data_[second_idx]);
    }

    void resize(std::size_t new_size, const T& value = T{}) {
        std::size_t old_size = n_rows();

        data_.resize(new_size);
        for (std::size_t i = old_size; i < new_size; ++i) {
            data_[i] = Array<T>(n_cols(), value);
        }
    }

    void resize_rows(std::size_t new_size, const T& value = T{}) {
        for (std::size_t i = 0; i < n_rows(); ++i) {
            data_[i].resize(new_size, value);
        }
    }

  private:
    template <typename Iter>
    void fill_from_iter(Iter begin, Iter end) {
        auto elem_it = begin;

        for (std::size_t row_idx = 0; row_idx < data_.size(); ++row_idx) {
            for (std::size_t col_idx = 0; col_idx < data_[row_idx].size(); ++col_idx, ++elem_it) {
                data_[row_idx][col_idx] = *elem_it;
            }
        }
    }

  private:
    Array<Array<T>> data_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const RectangularArray<T>& array) {
    std::copy(array.begin(), array.end() - 1, std::ostream_iterator<Array<T>>(ostream, "\n"));
    ostream << *(array.end() - 1);
    return ostream;
}

}; // namespace mtx
