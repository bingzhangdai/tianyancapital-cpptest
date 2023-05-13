#include <gtest/gtest.h>
#include <cstdlib>

#include "q2/order.hpp"
#include "q2/risk.hpp"

namespace cpptest {

TEST(TestRisk, OrderSuccess) {
    auto o = Order();
    o.accountId = 1;
    EXPECT_TRUE(Risk::Check(o));
}

TEST(TestRisk, OrderFail) {
    auto o = Order();
    o.accountId = 2;
    EXPECT_FALSE(Risk::Check(o));
}

TEST(TestRisk, OrderInBlocklist) {
    auto o = Order();
    o.accountId = 1;
    strcpy(o.instrumentId, "123456.SH");
    EXPECT_FALSE(Risk::Check(o));
}

}
