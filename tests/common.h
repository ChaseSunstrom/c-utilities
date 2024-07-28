#ifndef CUTIL_TESTS_COMMON_H
#define CUTIL_TESTS_COMMON_H

#define EXPECT_TRUE(x) assert(x)
#define EXPECT_FALSE(x) assert(!(x))

#define EXPECT_EQ(x, y) assert((x) == (y))
#define EXPECT_NE(x, y) assert((x) != (y))

#define EXPECT_LT(x, y) assert((x) < (y))
#define EXPECT_LE(x, y) assert((x) <= (y))

#define EXPECT_GT(x, y) assert((x) > (y))
#define EXPECT_GE(x, y) assert((x) >= (y))

#define EXPECT_STREQ(x, y) assert(strcmp(x, y) == 0)
#define EXPECT_STRNE(x, y) assert(strcmp(x, y) != 0)

#define EXPECT_PTR_EQ(x, y) assert((x) == (y))
#define EXPECT_PTR_NE(x, y) assert((x) != (y))

#define EXPECT_NULL(x) assert((x) == NULL)
#define EXPECT_NOT_NULL(x) assert((x) != NULL)

#endif
