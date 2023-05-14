#include <gtest/gtest.h>
#include <thread>

#include "q4/strategy.hpp"
#include "q3/order_rtn.hpp"

namespace cpptest {

using namespace std::chrono_literals;

TEST(TestStrategy, SendOrderInternal) {
    Strategy s(true);
    std::this_thread::sleep_for(100ms);
    s.OnMarketData(MarketData());
    std::this_thread::sleep_for(100ms);
}

}

