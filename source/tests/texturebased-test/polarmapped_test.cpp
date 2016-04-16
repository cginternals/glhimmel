
#include <gmock/gmock.h>


#include <texturebased/polarmapped.h>

class polarmapped_test: public testing::Test
{
public:
};

TEST_F(polarmapped_test, CheckSomeResults)
{
    EXPECT_EQ(1 + 1, 2);
}
