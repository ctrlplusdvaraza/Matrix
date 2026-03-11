#pragma once

#include <gtest/gtest.h>

#include "signature.hpp"
#include "types_for_test.hpp"


std::size_t throws_at = 0;

struct AllocException : public std::bad_alloc {};

inline void ThrowAt(std::size_t count) {
    throws_at = count;
}

inline void ThrowNext() {
    throws_at = 1;
}

inline void Nothrow() {
    throws_at = 0;
}

inline void ThrowRandom() {
    const int kLimit = 75;
    if (GenerateRandomInt(0, 100) < kLimit) { ThrowNext(); }
}


void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    return malloc(size);
}

void* operator new(std::size_t size) {
    if (throws_at == 0) { return malloc(size); }
    --throws_at;
    if (throws_at == 0) { throw AllocException{}; }
    return malloc(size);
}

void* operator new[](std::size_t size) {
    if (throws_at == 0) { return malloc(size); }
    --throws_at;
    if (throws_at == 0) { throw AllocException{}; }
    return malloc(size);
}

void operator delete(void* data) noexcept {
    free(data);
}

void operator delete(void* data, std::size_t) noexcept {
    free(data);
}

void operator delete[](void* data) noexcept {
    free(data);
}

void operator delete[](void* data, std::size_t) noexcept {
    free(data);
}


template <typename T>
using ArrayT = signature::ArrayT<T>;

template <typename T>
class ExceptionSafety : public ::testing::Test {};

TYPED_TEST_SUITE(ExceptionSafety, TestTypes);

TYPED_TEST(ExceptionSafety, ConstructionCount) {
    std::size_t size = 10;
    ThrowNext();
    EXPECT_THROW(ArrayT<TypeParam> arr(size), AllocException);
}

TYPED_TEST(ExceptionSafety, ConstructionCopy) {
    std::size_t size = 10;
    ArrayT<TypeParam> arr(size);
    ThrowNext();
    EXPECT_THROW(ArrayT<TypeParam> copy = arr, AllocException);
}

TYPED_TEST(ExceptionSafety, AssignmentCopy) {
    std::size_t size = 10;

    ArrayT<TypeParam> arr(size);
    ArrayT<TypeParam> other;

    ThrowNext();
    EXPECT_THROW(other = arr, AllocException);

    EXPECT_EQ(other, ArrayT<TypeParam>());
}
