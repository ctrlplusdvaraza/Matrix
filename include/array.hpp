#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <iterator>

namespace mtx {

template <typename T>
class ArrayBuffer {
  protected:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

  protected:
    ArrayBuffer() = default;

    ArrayBuffer(std::size_t capacity) : data_(allocate(capacity)), capacity_(capacity) {}

    ArrayBuffer(const ArrayBuffer&) = delete;

    ArrayBuffer& operator=(const ArrayBuffer&) = delete;

    ArrayBuffer(ArrayBuffer&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    };

    ArrayBuffer& operator=(ArrayBuffer&& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        return *this;
    }

    ~ArrayBuffer() {
        destroy(data_, data_ + size_);
        ::operator delete(data_);
    }

  protected:
    void construct(T* ptr, const T& value) { new (ptr) T(value); }

    void construct(T* ptr, T&& value) { new (ptr) T(std::move(value)); }

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
};

template <typename T>
class Array : private ArrayBuffer<T>{
  private:
    using ArrayBuffer<T>::data_;
    using ArrayBuffer<T>::size_;
    using ArrayBuffer<T>::capacity_;
    using ArrayBuffer<T>::construct;

  public:
    Array() = default;
    
    Array(std::initializer_list<T> init_list) : ArrayBuffer(init_list.size()) {
        for (const auto& item : init_list) {
            construct(data_ + size_, item);
            ++size_;
        }
    }

    template <std::forward_iterator Iter>
    Array(Iter elems_begin, Iter elems_end) : ArrayBuffer(std::distance(elems_begin, elems_end)) {
        for (auto iter = elems_begin; iter != elems_end; ++iter) {
            construct(data_ + size_, *iter);
            ++size_;
        }
    }

    Array(std::size_t size, const T& value = T{}) : ArrayBuffer(size) { 
        while (size_ < capacity_) {
            construct(data_ + size_, value);
            ++size_;
        }        
    }

    Array(const Array& other) : ArrayBuffer(other.size_) {
        while (size_ < other.size_) {
            construct(data_ + size_, other.data_[size_]);
            ++size_;
        }
    }

    Array& operator=(const Array& other) {
        Array tmp(other);
        std::swap(*this, tmp);
        return *this;
    }

    Array(Array&& other) = default;

    Array& operator=(Array&& other) = default;

    ~Array() = default;

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

    void swap(Array& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

}; // namespace mtx
