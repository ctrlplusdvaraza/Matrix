#pragma once

#include <algorithm>
#include <utility>
#include <iostream>
#include <iterator>

namespace mtx {

template<typename Iter, typename T>
concept IteratorOf = std::same_as<std::iter_value_t<Iter>, T>;

template<typename T>
class Array {
  public:
    Array() = default;

    Array(std::initializer_list<T> init_list) : size_(init_list.size()), data_(allocate(size())) {
        std::copy(init_list.begin(), init_list.end(), data_);
    }

    template <typename Iter>
    requires IteratorOf<Iter, T>
    Array(std::size_t size, Iter elems_begin, Iter elems_end) : size_(size), data_(allocate(size)) {
        std::copy(elems_begin, elems_end, data_);
    }

    Array(std::size_t size, const T& value = T{}) {
        reallocate_and_fill(size, value);
    }

  public:
    Array(const Array& other) : size_(other.size()), data_(allocate(size())) {
        std::copy(other.begin(), other.end(), data_);
    }

    Array(Array&& other) {
        swap(other);
    }

    Array& operator=(const Array& other) {
        if(this == &other) {
            return *this;
        }

        Array temp(other);
        swap(temp);

        return *this;
    }

    Array& operator=(Array&& other) {
        if(this == &other) {
            return *this;
        }

        swap(other);

        return *this;
    }

    ~Array() { deallocate(data_); }

  public:
    T& operator[](std::size_t idx) { return data_[idx]; }
    const T& operator[](std::size_t idx) const { return data_[idx]; }

    T* begin() { return data_; } 
    const T* begin() const { return data_; } 

    T* end() { return begin() + size(); } 
    const T* end() const { return begin() + size(); } 

  public:
    std::size_t size() const { return size_; }

    bool empty() const { return size() == 0; }

  public:
    const Array& operator+=(const Array& other) {
        std::transform(begin(), end(), other.begin(), begin(), std::plus<T>());
        return *this;
    }

    Array operator+(const Array& other) const {
        Array temp(*this);
        temp += other;
        return temp;
    }

    const Array& operator-=(const Array& other) {
        std::transform(begin(), end(), other.begin(), begin(), std::minus<T>());
        return *this;
    }

    Array operator-(const Array& other) const {
        Array temp(*this);
        temp -= other;
        return temp;
    }

    const Array& operator*=(const T& scalar) {
        for (std::size_t i = 0; i < size(); ++i) {
            (*this)[i] *= scalar;
        }
        return *this;
    }

    Array operator*(const T& scalar) const {
        Array temp(*this);
        temp *= scalar;
        return temp;
    }

  public:
    void resize(std::size_t new_size, const T& value = T{}) {
        reallocate_and_fill(new_size, value);
    }

    void fill(const T& value) {
        std::fill(begin(), end(), value);
    }

  private:
    void swap(Array& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    static T* allocate(std::size_t capacity) {
        return new T[capacity];
    }
    static void deallocate(T* data) {
        delete[] data;
    }

    void reallocate(std::size_t new_size) {
        if (new_size == size()) {
            return;
        }

        T* new_buffer = allocate(new_size);

        if (begin() != nullptr) {
            std::size_t n_to_copy = std::min(size_, new_size);
            std::copy(begin(), begin() + n_to_copy, new_buffer);
            deallocate(data_);
        }

        data_ = new_buffer;
        size_ = new_size;
    }

    void reallocate_and_fill(std::size_t new_size, const T& value = T{}) {
        std::size_t old_size = size();

        reallocate(new_size);

        T* first = begin() + old_size;
        T* last  = begin() + new_size;

        if (last > first) {
            std::fill(first, last, value);
        }
    }

  private:
    std::size_t size_ = 0;
    T* data_ = nullptr;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const Array<T>& array) {
    std::copy(array.begin(), array.end() - 1, std::ostream_iterator<T>(ostream, ", "));
    ostream << *(array.end() - 1);
    return ostream;
}

template <typename T>
class JaggedArray {
  public:
    JaggedArray() = default;

    JaggedArray(std::initializer_list<std::initializer_list<T>> init_lists) 
        : data_(init_lists.size()) 
    {
        std::size_t row_idx = 0;
        for (const auto& row_list : init_lists) {
            data_[row_idx] = Array<T>(row_list);
            ++row_idx;
        }
    }

    template<typename Iter>
    requires IteratorOf<Iter, std::size_t>
    JaggedArray(std::size_t n_rows, Iter rows_sizes_begin, Iter rows_sizes_end, const T& elem = T{}) : data_(n_rows) {
        auto it = rows_sizes_begin;
        for (std::size_t row_idx = 0; row_idx < n_rows; ++row_idx, ++it) {
            data_[row_idx].resize(*it, elem);
        }
    }

    template<typename Iter1, typename Iter2>
    requires IteratorOf<Iter1, std::size_t> && IteratorOf<Iter2, T>
    JaggedArray(std::size_t n_rows, Iter1 rows_sizes_begin, Iter1 rows_sizes_end, Iter2 elems_begin, Iter2 elems_end) 
        : JaggedArray(n_rows, rows_sizes_begin, rows_sizes_end) 
    {    
        fill_from_iter(elems_begin, elems_end);
    }

    JaggedArray(std::size_t n_rows, std::size_t row_size, const T& elem = T{}) : data_(n_rows) {
        for (std::size_t row_idx = 0; row_idx < n_rows; ++row_idx) {
            data_[row_idx].resize(row_size, elem);
        }
    }

    template<typename Iter>
    requires IteratorOf<Iter, T>
    JaggedArray(std::size_t n_rows, std::size_t rows_size, Iter elems_begin, Iter elems_end) 
        : JaggedArray(n_rows, rows_size)
    {
        fill_from_iter(elems_begin, elems_end);
    }

  public:
    Array<T>& operator[](std::size_t idx) {
        return data_[idx];
    }

    const Array<T>& operator[](std::size_t idx) const {
        return data_[idx];
    }

    Array<T>* begin() { return data_.begin(); } 
    const Array<T>* begin() const { return data_.begin(); } 

    Array<T>* end() { return data_.end(); } 
    const Array<T>* end() const { return data_.end(); } 

  public:
    bool empty() const { return data_.empty(); }

    std::size_t n_rows() const {
        return data_.size();
    }

  public:
    void resize(std::size_t new_size) {
        data_.resize(new_size);
    }

    void resize_row(std::size_t row_idx, std::size_t new_size) {
        data_[row_idx].resize(new_size);
    }

    void swap_rows(std::size_t first_idx, std::size_t second_idx) {
        std::swap(data_[first_idx], data_[second_idx]);
    }

  private:
    template<typename Iter>
    void fill_from_iter(Iter begin, Iter end) {
        auto elem_it = begin;
        
        for (std::size_t row_idx = 0; row_idx < data_.size(); ++row_idx) {
            for (std::size_t col_idx = 0; col_idx < data_[row_idx].size(); ++col_idx, ++elem_it) {
                data_[row_idx][col_idx] = *elem_it;
            }
        }
    }

  protected:
    Array<Array<T>> data_{};
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const JaggedArray<T>& array) {
    std::copy(array.begin(), array.end() - 1, std::ostream_iterator<Array<T>>(ostream, "\n"));
    ostream << *(array.end() - 1);
    return ostream;
}

template <typename T>
class RectangularArray : private JaggedArray<T> {
  public:
    RectangularArray() = default;

    RectangularArray(std::initializer_list<std::initializer_list<T>> init_lists)
        : JaggedArray<T>(init_lists) {}

    RectangularArray(std::size_t n_rows, std::size_t n_cols, const T& elem = T{}) 
        : JaggedArray<T>(n_rows, n_cols, elem) {}

    template<typename Iter>
    requires IteratorOf<Iter, T>
    RectangularArray(std::size_t n_rows, std::size_t n_cols, Iter elems_begin, Iter elems_end) 
        : JaggedArray<T>(n_rows, n_cols, elems_begin, elems_end) {}

  public:
    using JaggedArray<T>::operator[];
    using JaggedArray<T>::begin;
    using JaggedArray<T>::end;
    
  public:
    using JaggedArray<T>::empty;
    using JaggedArray<T>::n_rows;

    std::size_t n_cols() const {
        if (this->empty()) {
            return 0;
        }
        
        return data_[0].size();
    }

  public:  
    using JaggedArray<T>::swap_rows;

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
    using JaggedArray<T>::data_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& ostream, const RectangularArray<T>& array) {
    std::copy(array.begin(), array.end() - 1, std::ostream_iterator<Array<T>>(ostream, "\n"));
    ostream << *(array.end() - 1);
    return ostream;
}

}; // namespace mtx
