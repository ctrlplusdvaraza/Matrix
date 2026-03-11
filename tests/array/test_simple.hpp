#pragma once

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "signature.hpp"
#include "types_for_test.hpp"

template <typename T>
using ArrayT = signature::ArrayT<T>;

template <typename T>
class SizeCorrect : public ::testing::Test {};

TYPED_TEST_SUITE(SizeCorrect, TestTypes);

TYPED_TEST(SizeCorrect, Empty) {
    ArrayT<TypeParam> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0);
}

TYPED_TEST(SizeCorrect, NotEmpty)
{
    ArrayT<TypeParam> arr(5);
    EXPECT_FALSE(arr.empty());
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr[0], TypeParam{});
    EXPECT_EQ(arr[1], TypeParam{});
    EXPECT_EQ(arr[2], TypeParam{});
    EXPECT_EQ(arr[3], TypeParam{});
    EXPECT_EQ(arr[4], TypeParam{});
}

TEST(Constructor, ValueConstructor)
{
    ArrayT<std::string> arr(3, "hello");
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "hello");
    EXPECT_EQ(arr[1], "hello");
    EXPECT_EQ(arr[2], "hello");
}

TEST(Constructor, IteratorConstructor)
{
    std::vector<int> vec{10, 20, 30};
    ArrayT<int> arr(vec.begin(), vec.end());
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(Constructor, InitializerListConstructor) 
{
    ArrayT<double> arr = {-8.9, 5.6, 0.4, -1.2};
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], -8.9);
    EXPECT_EQ(arr[1], 5.6);
    EXPECT_EQ(arr[2], 0.4);
    EXPECT_EQ(arr[3], -1.2);
}

TEST(Constructor, CopyConstructor) 
{
    ArrayT<std::string> arr1 = {"one", "two", "three", "four", "five"};
    ArrayT<std::string> arr2 = arr1;
    EXPECT_EQ(arr2.size(), 5);
    EXPECT_EQ(arr2[0], "one");
    EXPECT_EQ(arr2[1], "two");
    EXPECT_EQ(arr2[2], "three");
    EXPECT_EQ(arr2[3], "four");
    EXPECT_EQ(arr2[4], "five");
}

TEST(Constructor, MoveConstructor) 
{
    ArrayT<std::string> arr1 = {"one", "two", "three", "four", "five"};
    ArrayT<std::string> arr2 = std::move(arr1);
    EXPECT_EQ(arr2.size(), 5);
    EXPECT_EQ(arr2[0], "one");
    EXPECT_EQ(arr2[1], "two");
    EXPECT_EQ(arr2[2], "three");
    EXPECT_EQ(arr2[3], "four");
    EXPECT_EQ(arr2[4], "five");
}
