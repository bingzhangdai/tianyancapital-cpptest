#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include "q3/api.hpp"
#include "q3/order_rtn.hpp"

namespace cpptest {

using namespace std::chrono_literals;

TEST(TestRisk, ReceiveTest) {
    std::thread sub([]() {
        ModuleA a;
        EXPECT_EQ(1, a.GetMessage().value().status);
        EXPECT_EQ(2, a.GetMessage().value().status);
        EXPECT_EQ(3, a.GetMessage().value().status);
    });
    std::thread sub2([]() {
        ModuleB b;
        EXPECT_EQ(1, b.GetMessage().value().status);
        EXPECT_EQ(2, b.GetMessage().value().status);
        EXPECT_EQ(3, b.GetMessage().value().status);
    });
    std::thread pub([]() {
        OrderRtn i{1}, j{2}, k{3};
        Api::OnRtnOrder(i);
        Api::OnRtnOrder(j);
        std::this_thread::sleep_for(100ms);
    });
}

}
