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

}; // namespace mtx
