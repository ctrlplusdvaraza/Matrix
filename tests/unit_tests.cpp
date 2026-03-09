#include <gtest/gtest.h>
#include <vector>
#include <list>
#include <string>

#include "jagged_array.hpp"

using namespace mtx;

int main(int argc, char **argv)
{
    testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS ();
}

// -----------------------------------------------------------------------------
// ----------------------------- Array constructors ----------------------------
// -----------------------------------------------------------------------------

TEST(Array, default_constructor)
{
    Array<double> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0);
}

TEST(Array, initializer_list_constructor)
{
    Array<int> arr{1, 2, 3, 4, 5};
    EXPECT_FALSE(arr.empty());
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[4], 5);
}

TEST(Array, size_value_constructor)
{
    Array<std::string> arr(3, "hello");
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "hello");
    EXPECT_EQ(arr[1], "hello");
    EXPECT_EQ(arr[2], "hello");
}

TEST(Array, size_default_constructor)
{
    Array<int> arr(4);
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[3], 0);
}

TEST(Array, iterator_constructor)
{
    std::vector<int> vec{10, 20, 30};
    Array<int> arr(3, vec.begin(), vec.end());
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

// -----------------------------------------------------------------------------
// ----------------------------- Array methods ---------------------------------
// -----------------------------------------------------------------------------

TEST(Array, operator_brackets)
{
    Array<int> arr{1, 2, 3};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    
    // Test modification
    arr[1] = 100;
    EXPECT_EQ(arr[1], 100);
}

TEST(Array, begin_end)
{
    Array<int> arr{1, 2, 3};
    auto it = arr.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    
    auto rit = arr.end();
    --rit;
    EXPECT_EQ(*rit, 3);
}

TEST(Array, resize)
{
    Array<int> arr{1, 2, 3};
    arr.resize(5, 10);
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 10);
    EXPECT_EQ(arr[4], 10);
    
    arr.resize(2);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(Array, fill)
{
    Array<int> arr(3);
    arr.fill(42);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], 42);
    EXPECT_EQ(arr[2], 42);
}

// -----------------------------------------------------------------------------
// -------------------------- JaggedArray constructors -------------------------
// -----------------------------------------------------------------------------

TEST(JaggedArray, initializer_list_constructor)
{
    JaggedArray<int> jarr{{1, 2}, {3, 4, 5}, {6}};
    EXPECT_EQ(jarr.n_rows(), 3);
    EXPECT_EQ(jarr[0][0], 1);
    EXPECT_EQ(jarr[0][1], 2);
    EXPECT_EQ(jarr[1][2], 5);
    EXPECT_EQ(jarr[2][0], 6);
}

TEST(JaggedArray, row_sizes_constructor)
{
    std::vector<size_t> sizes{2, 3, 1};
    JaggedArray<int> jarr(3, sizes.begin(), sizes.end(), 99);
    EXPECT_EQ(jarr.n_rows(), 3);
    EXPECT_EQ(jarr[0].size(), 2);
    EXPECT_EQ(jarr[1].size(), 3);
    EXPECT_EQ(jarr[2].size(), 1);
    EXPECT_EQ(jarr[0][0], 99);
    EXPECT_EQ(jarr[1][2], 99);
}

TEST(JaggedArray, row_sizes_elements_constructor)
{
    std::vector<size_t> sizes{2, 1};
    std::vector<int> elems{10, 20, 30};
    JaggedArray<int> jarr(2, sizes.begin(), sizes.end(), elems.begin(), elems.end());
    EXPECT_EQ(jarr.n_rows(), 2);
    EXPECT_EQ(jarr[0].size(), 2);
    EXPECT_EQ(jarr[1].size(), 1);
    EXPECT_EQ(jarr[0][0], 10);
    EXPECT_EQ(jarr[0][1], 20);
    EXPECT_EQ(jarr[1][0], 30);
}

TEST(JaggedArray, uniform_size_constructor)
{
    JaggedArray<int> jarr(3, 2, 7);
    EXPECT_EQ(jarr.n_rows(), 3);
    EXPECT_EQ(jarr[0].size(), 2);
    EXPECT_EQ(jarr[1].size(), 2);
    EXPECT_EQ(jarr[2].size(), 2);
    EXPECT_EQ(jarr[0][0], 7);
    EXPECT_EQ(jarr[2][1], 7);
}


TEST(JaggedArray, uniform_size_iterator_constructor)
{
    std::vector<size_t> row_sizes{2, 3, 1};
    std::vector<size_t> elements{10, 20, 30, 40, 50, 60};
    
    JaggedArray<size_t> jarr(3, row_sizes.begin(), row_sizes.end(), elements.begin(), elements.end());
    EXPECT_EQ(jarr.n_rows(), 3);
    EXPECT_EQ(jarr[0].size(), 2);
    EXPECT_EQ(jarr[1].size(), 3);
    EXPECT_EQ(jarr[2].size(), 1);
}

// -----------------------------------------------------------------------------
// ---------------------------- JaggedArray methods ----------------------------
// -----------------------------------------------------------------------------

TEST(JaggedArray, operator_brackets)
{
    JaggedArray<int> jarr{{1, 2}, {3, 4, 5}};
    EXPECT_EQ(jarr[0][0], 1);
    EXPECT_EQ(jarr[1][2], 5);
    
    jarr[0][1] = 100;
    EXPECT_EQ(jarr[0][1], 100);
}

TEST(JaggedArray, begin_end)
{
    JaggedArray<int> jarr{{1}, {2, 3}};
    auto it = jarr.begin();
    EXPECT_EQ((*it)[0], 1);
    ++it;
    EXPECT_EQ((*it).size(), 2);
    EXPECT_EQ((*it)[1], 3);
}

TEST(JaggedArray, empty)
{
    JaggedArray<int> jarr;
    EXPECT_TRUE(jarr.empty());
    
    JaggedArray<int> jarr2{{1, 2}};
    EXPECT_FALSE(jarr2.empty());
}

TEST(JaggedArray, resize)
{
    JaggedArray<int> jarr{{1, 2}, {3, 4, 5}};
    jarr.resize(4);
    EXPECT_EQ(jarr.n_rows(), 4);
    EXPECT_TRUE(jarr[2].empty());
    EXPECT_TRUE(jarr[3].empty());
}

TEST(JaggedArray, resize_row)
{
    JaggedArray<int> jarr{{1, 2}, {3, 4, 5}};
    jarr.resize_row(0, 4);
    EXPECT_EQ(jarr[0].size(), 4);
    EXPECT_EQ(jarr[0][0], 1); // Existing elements preserved
    EXPECT_EQ(jarr[0][1], 2);
    
    jarr.resize_row(1, 1);
    EXPECT_EQ(jarr[1].size(), 1);
    EXPECT_EQ(jarr[1][0], 3);
}

TEST(JaggedArray, swap_rows)
{
    JaggedArray<int> jarr{{1, 2}, {3, 4, 5}, {6}};
    jarr.swap_rows(0, 2);
    EXPECT_EQ(jarr[0].size(), 1);
    EXPECT_EQ(jarr[0][0], 6);
    EXPECT_EQ(jarr[2].size(), 2);
    EXPECT_EQ(jarr[2][0], 1);
    EXPECT_EQ(jarr[2][1], 2);
}

// -----------------------------------------------------------------------------
// ------------------------ RectangularArray constructors ----------------------
// -----------------------------------------------------------------------------

TEST(RectangularArray, initializer_list_constructor)
{
    RectangularArray<int> rarr{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    EXPECT_EQ(rarr.n_rows(), 3);
    EXPECT_EQ(rarr.n_cols(), 3);
    EXPECT_EQ(rarr[0][0], 1);
    EXPECT_EQ(rarr[1][2], 6);
    EXPECT_EQ(rarr[2][1], 8);
}

TEST(RectangularArray, size_value_constructor)
{
    RectangularArray<std::string> rarr(2, 3, "test");
    EXPECT_EQ(rarr.n_rows(), 2);
    EXPECT_EQ(rarr.n_cols(), 3);
    EXPECT_EQ(rarr[0][0], "test");
    EXPECT_EQ(rarr[1][2], "test");
}

TEST(RectangularArray, size_iterator_constructor)
{
    std::vector<int> elems{1, 2, 3, 4, 5, 6};
    RectangularArray<int> rarr(2, 3, elems.begin(), elems.end());
    EXPECT_EQ(rarr.n_rows(), 2);
    EXPECT_EQ(rarr.n_cols(), 3);
    EXPECT_EQ(rarr[0][0], 1);
    EXPECT_EQ(rarr[0][2], 3);
    EXPECT_EQ(rarr[1][0], 4);
    EXPECT_EQ(rarr[1][2], 6);
}

// -----------------------------------------------------------------------------
// -------------------------- RectangularArray methods -------------------------
// -----------------------------------------------------------------------------

TEST(RectangularArray, operator_brackets)
{
    RectangularArray<int> rarr{{1, 2}, {3, 4}};
    EXPECT_EQ(rarr[0][0], 1);
    EXPECT_EQ(rarr[1][1], 4);
    
    rarr[0][1] = 100;
    EXPECT_EQ(rarr[0][1], 100);
}

TEST(RectangularArray, empty)
{
    RectangularArray<int> rarr;
    EXPECT_TRUE(rarr.empty());
    
    RectangularArray<int> rarr2(2, 3);
    EXPECT_FALSE(rarr2.empty());
}

TEST(RectangularArray, swap_rows)
{
    RectangularArray<int> rarr{{1, 2}, {3, 4}, {5, 6}};
    rarr.swap_rows(0, 2);
    EXPECT_EQ(rarr[0][0], 5);
    EXPECT_EQ(rarr[0][1], 6);
    EXPECT_EQ(rarr[2][0], 1);
    EXPECT_EQ(rarr[2][1], 2);
}

TEST(RectangularArray, resize)
{
    RectangularArray<int> rarr{{1, 2}, {3, 4}};
    rarr.resize(3);
    EXPECT_EQ(rarr.n_rows(), 3);
    EXPECT_EQ(rarr.n_cols(), 2);
}

TEST(RectangularArray, resize_rows)
{
    RectangularArray<int> rarr{{1, 2}, {3, 4}};
    rarr.resize_rows(3);
    EXPECT_EQ(rarr.n_rows(), 2);
    EXPECT_EQ(rarr.n_cols(), 3);
}

// -----------------------------------------------------------------------------
// --------------------------- Edge cases and errors ---------------------------
// -----------------------------------------------------------------------------

TEST(EdgeCases, empty_initializer_lists)
{
    Array<int> arr{};
    EXPECT_TRUE(arr.empty());
    
    JaggedArray<int> jarr{};
    EXPECT_TRUE(jarr.empty());
    
    RectangularArray<int> rarr{};
    EXPECT_TRUE(rarr.empty());
}

TEST(EdgeCases, zero_sizes)
{
    Array<int> arr(0);
    EXPECT_TRUE(arr.empty());
    
    JaggedArray<int> jarr(0, 0);
    EXPECT_TRUE(jarr.empty());
    
    RectangularArray<int> rarr(0, 0);
    EXPECT_TRUE(rarr.empty());
}

TEST(EdgeCases, string_types)
{
    Array<std::string> arr{"hello", "world"};
    EXPECT_EQ(arr[0], "hello");
    
    JaggedArray<std::string> jarr{{"a", "b"}, {"c"}};
    EXPECT_EQ(jarr[0][1], "b");
    
    RectangularArray<std::string> rarr{{"x", "y"}, {"z", "w"}};
    EXPECT_EQ(rarr[1][0], "z");
}

TEST(EdgeCases, double_types)
{
    Array<double> arr{1.1, 2.2, 3.3};
    EXPECT_DOUBLE_EQ(arr[1], 2.2);
    
    JaggedArray<double> jarr{{1.1}, {2.2, 3.3}};
    EXPECT_DOUBLE_EQ(jarr[1][1], 3.3);
    
    RectangularArray<double> rarr{{1.1, 2.2}, {3.3, 4.4}};
    EXPECT_DOUBLE_EQ(rarr[1][0], 3.3);
}

TEST(Performance, large_arrays)
{
    Array<int> large_arr(1000, 42);
    EXPECT_EQ(large_arr.size(), 1000);
    EXPECT_EQ(large_arr[999], 42);
    
    JaggedArray<int> large_jarr(100, 10, 7);
    EXPECT_EQ(large_jarr.n_rows(), 100);
    EXPECT_EQ(large_jarr[99].size(), 10);
    
    RectangularArray<int> large_rarr(50, 20, 8);
    EXPECT_EQ(large_rarr.n_rows(), 50);
    EXPECT_EQ(large_rarr.n_cols(), 20);
}
