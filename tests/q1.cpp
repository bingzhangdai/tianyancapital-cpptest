#include <gtest/gtest.h>
#include <cstdlib>

#include "q1/market_data_processor_a.hpp"
#include "q1/market_data_processor_b.hpp"
#include "q1/market_data_processor.hpp"

namespace cpptest {

TEST(TestMarketDataProcessor, MarketDataProcessorA) {
    auto data = MarketDataA();
    strcpy(data.instrumentId, "MarketDataA");
    data.askPrice1 = 2.33;
    data.bidPrice1 = 2.22;

    MarketDataProcessorBase *p = new MarketDataProcessorA();

    EXPECT_EQ(data.instrumentId, p->GetSymbol(&data));
    EXPECT_DOUBLE_EQ((data.askPrice1 + data.bidPrice1) / 2, p->GetMidPrice(&data));
}

TEST(TestMarketDataProcessor, MarketDataProcessorB) {
    auto data = MarketDataB();
    data.code = "MarketDataA";
    data.askPrice[0] = 2.33;
    data.bidPrice[0] = 2.22;

    MarketDataProcessorBase *p = new MarketDataProcessorB();

    EXPECT_EQ(data.code, p->GetSymbol(&data));
    EXPECT_DOUBLE_EQ((data.askPrice[0] + data.bidPrice[0]) / 2, p->GetMidPrice(&data));
}

TEST(TestMarketDataProcessor, onQuote) {
    auto data = MarketDataA();
    strcpy(data.instrumentId, "MarketDataA");
    data.askPrice1 = 2.33;
    data.bidPrice1 = 2.22;

    onQuoteA(data);

    auto data2 = MarketDataB();
    data2.code = "MarketDataA";
    data2.askPrice[0] = 2.33;
    data2.bidPrice[0] = 2.22;

    onQuoteB(data2);
}

} // namespace cpptest

