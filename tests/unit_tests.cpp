#include <gtest/gtest.h>

#include "test_simple.hpp"
#include "test_throwing_allocation.hpp"
#include "test_throwing_type.hpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
