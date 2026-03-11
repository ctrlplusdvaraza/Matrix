#pragma once

#include <gtest/gtest.h>

#include <stdexcept>

#include "common.hpp"
#include "signature.hpp"

struct TestException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct ThrowingInt {
    ThrowingInt() { ThrowIfNeed(); }
    ThrowingInt(int value) : value_(value) { ThrowIfNeed(); }

    ThrowingInt(const ThrowingInt& other) : value_(other.value_) { ThrowIfNeed(); }
    ThrowingInt& operator=(const ThrowingInt& other) {
        if (this != &other) {
            ThrowIfNeed();
            value_ = other.value_;
        }
        return *this;
    }

    bool operator==(const ThrowingInt& other) const = default;
    auto operator<=>(const ThrowingInt& other) const = default;

    static void ThrowAt(std::size_t count) { throws_at = count; }

    static void ThrowNext() { throws_at = 1; }

    static void Nothrow() { throws_at = 0; }

    static void ThrowRandom() {
        const int kLimit = 75;
        if (GenerateRandomInt(0, 100) < kLimit) { ThrowNext(); }
    }

    static bool WillNextThrow() { return throws_at < 1; }

  private:
    static void ThrowIfNeed() {
        if (throws_at == 0) { return; }
        --throws_at;
        if (throws_at == 0) { Throw(); }
    }

    static void Throw() { throw TestException("Throwing int exception"); }

    inline static std::size_t throws_at = 0;

  private:
    int value_ = 0;
};

using Array = signature::ArrayT<ThrowingInt>;

TEST(ExceptionInType, Construction_CountElement_AtFirst) {
    std::size_t size = 10;
    ThrowingInt value(10);

    ThrowingInt::ThrowNext();

    EXPECT_THROW(Array vec(size, value), TestException);

    ThrowingInt::Nothrow();
}

TEST(ExceptionInType, Construction_CountElement_InMiddle) {
    std::size_t size = 10;
    ThrowingInt value(10);

    ThrowingInt::ThrowAt(size / 2);

    EXPECT_THROW(Array vec(size, value), TestException);

    ThrowingInt::Nothrow();
}

TEST(ExceptionInType, Construction_CountElement_AtLast) {
    std::size_t size = 10;
    ThrowingInt value(10);

    ThrowingInt::ThrowAt(size);

    EXPECT_THROW(Array vec(size, value), TestException);

    ThrowingInt::Nothrow();
}

TEST(ExceptionInType, Construction_Copy_AtFirst) {
    std::size_t size = 10;
    ThrowingInt value(10);

    Array vec(size, value);

    ThrowingInt::ThrowNext();

    EXPECT_THROW(Array copy = vec, TestException);

    ThrowingInt::Nothrow();
}

TEST(ExceptionInType, Construction_Copy_InMiddle) {
    std::size_t size = 10;
    ThrowingInt value(10);

    Array vec(size, value);

    ThrowingInt::ThrowAt(size / 2);

    EXPECT_THROW(Array copy = vec, TestException);

    ThrowingInt::Nothrow();
}

TEST(ExceptionInType, Construction_Copy_AtLast) {
    std::size_t size = 10;
    ThrowingInt value(10);

    Array vec(size, value);

    ThrowingInt::ThrowAt(size);

    EXPECT_THROW(Array copy = vec, TestException);

    ThrowingInt::Nothrow();
}

TEST(ExceptionInType, Assignment_SameSize) {
    std::size_t size = 10;
    ThrowingInt value(1);
    ThrowingInt other_value(2);

    Array vec(size, value);
    Array vec_other(size, other_value);

    ThrowingInt::ThrowAt(size / 2);

    EXPECT_THROW(vec = vec_other, TestException);

    ThrowingInt::Nothrow();

    EXPECT_EQ(vec, Array(size, value));
}

TEST(ExceptionInType, Assignment_Bigger) {
    std::size_t size = 10;
    ThrowingInt value(10);
    ThrowingInt other_value(20);

    Array vec(size, value);
    Array vec_other(size * size, other_value);

    ThrowingInt::ThrowAt(size * 4);

    EXPECT_THROW(vec = vec_other, TestException);

    ThrowingInt::Nothrow();

    EXPECT_EQ(vec, Array(size, value));
}

TEST(ExceptionInType, Assignment_UnreacheableThrow) {
    std::size_t size = 10;
    ThrowingInt value(10);
    ThrowingInt other_value(20);

    Array vec(size, value);
    Array vec_other(size * 2, other_value);

    ThrowingInt::ThrowAt(4 * size + 1);

    EXPECT_NO_THROW(vec = vec_other);

    ThrowingInt::Nothrow();

    EXPECT_EQ(vec, vec_other);
}
