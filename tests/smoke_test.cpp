

#include <gtest/gtest.h>

TEST(Toolchain, UsesCpp17)
{
    EXPECT_EQ(__cplusplus, 201703L);
}

