#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace mtx {

namespace details {

template <typename T>
class ArrayBuffer {
  protected:
    ArrayBuffer() = default;

    ArrayBuffer(std::size_t capacity) : data_(allocate(capacity)), capacity_(capacity) {}

    ArrayBuffer(const ArrayBuffer&) = delete;

    ArrayBuffer& operator=(const ArrayBuffer&) = delete;

    ArrayBuffer(ArrayBuffer&& other) noexcept { swap(other); };

    ArrayBuffer& operator=(ArrayBuffer&& other) noexcept {
        swap(other);
        return *this;
    }

    ~ArrayBuffer() {
        destroy(data_, data_ + size_);
        ::operator delete(data_);
    }

  protected:
    void construct(T* ptr, const T& value) { new (ptr) T(value); }

    void construct(T* ptr, T&& value) { new (ptr) T(std::move(value)); }

    void swap(ArrayBuffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

  private:
    static T* allocate(std::size_t size) {
        if (size == 0) { return nullptr; }

        return static_cast<T*>(::operator new(sizeof(T) * size));
    }

    static void destroy(T* ptr) noexcept { ptr->~T(); }

    static void destroy(T* begin, T* end) noexcept {
        while (begin != end) {
            destroy(begin);
            ++begin;
        }
    }

  protected:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

} // namespace details

template <typename T>
class Array : private details::ArrayBuffer<T> {
  public:
    Array() = default;

    Array(std::initializer_list<T> init_list) : details::ArrayBuffer<T>(init_list.size()) {
        fill_data(init_list.begin(), init_list.end());
    }

    template <std::forward_iterator Iter>
    Array(Iter elems_begin, Iter elems_end)
        : details::ArrayBuffer<T>(std::distance(elems_begin, elems_end)) {
        fill_data(elems_begin, elems_end);
    }

    Array(std::size_t size, const T& value = T{}) : details::ArrayBuffer<T>(size) {
        fill_data(value);
    }

    Array(const Array& other) : details::ArrayBuffer<T>(other.size_) { fill_data(other.data_); }

    Array& operator=(const Array& other) {
        Array tmp(other);
        std::swap(*this, tmp);
        return *this;
    }

    Array(Array&& other) noexcept = default;

    Array& operator=(Array&& other) noexcept = default;

    ~Array() = default;

  public:
    T& operator[](std::size_t idx) noexcept { return data_[idx]; }
    const T& operator[](std::size_t idx) const noexcept { return data_[idx]; }

    T* begin() noexcept { return data_; }
    const T* begin() const noexcept { return data_; }

    T* end() noexcept { return begin() + size(); }
    const T* end() const noexcept { return begin() + size(); }

  public:
    std::size_t size() const noexcept { return size_; }

    bool empty() const noexcept { return size() == 0; }

  public:
    Array& operator+=(const Array& other) {
        if (size() != other.size()) { throw std::invalid_argument("Array sizes must match"); }

        std::transform(begin(), end(), other.begin(), begin(), std::plus<T>());
        return *this;
    }

    Array& operator-=(const Array& other) {
        if (size() != other.size()) { throw std::invalid_argument("Array sizes must match"); }

        std::transform(begin(), end(), other.begin(), begin(), std::minus<T>());
        return *this;
    }

    Array& operator*=(const T& scalar) {
        for (std::size_t i = 0; i < size(); ++i) {
            (*this)[i] *= scalar;
        }
        return *this;
    }

    void swap(Array& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

  private:
    using details::ArrayBuffer<T>::construct;
    using details::ArrayBuffer<T>::swap;

    template <typename Iter>
    void fill_data(Iter begin, Iter end) {
        for (auto iter = begin; iter != end; ++iter) {
            construct(data_ + size_, *iter);
            ++size_;
        }
    }

    void fill_data(const T& value) {
        while (size_ < capacity_) {
            construct(data_ + size_, value);
            ++size_;
        }
    }

    void fill_data(T* other_data) {
        while (size_ < capacity_) {
            construct(data_ + size_, other_data[size_]);
            ++size_;
        }
    }

  private:
    using details::ArrayBuffer<T>::data_;
    using details::ArrayBuffer<T>::size_;
    using details::ArrayBuffer<T>::capacity_;
};

template <typename T>
Array<T> operator+(Array<T> lhs, const Array<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
Array<T> operator-(Array<T> lhs, const Array<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T>
Array<T> operator*(Array<T> arr, const T& value) {
    arr *= value;
    return arr;
}

template <typename T>
Array<T> operator*(const T& value, Array<T> arr) {
    arr *= value;
    return arr;
}

}; // namespace mtx
