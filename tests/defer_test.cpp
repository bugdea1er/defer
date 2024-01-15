#include <defer/defer.hpp>

#include <gtest/gtest.h>

TEST(DeferTest, ExecutedAtBlockLeaving) {
    bool flag = false;
    {
        EXPECT_FALSE(flag);
        defer {
            flag = true;
        };
    }

    EXPECT_TRUE(flag);
}

TEST(DeferTest, ExecutedAtReturn) {
    bool flag = false;
    auto func = [&] {
        EXPECT_FALSE(flag);
        defer {
            flag = true;
        };

        return 42;
    };

    EXPECT_FALSE(flag);
    EXPECT_EQ(func(), 42);
    EXPECT_TRUE(flag);
}

TEST(DeferTest, ExecutedAtThrow) {
    bool flag = false;
    auto func = [&] {
        EXPECT_FALSE(flag);
        defer {
            flag = true;
        };

        throw 42;
    };

    EXPECT_FALSE(flag);
    EXPECT_THROW(func(), int);
    EXPECT_TRUE(flag);
}
