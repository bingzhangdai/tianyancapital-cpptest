#include <gtest/gtest.h>
#include <cstdlib>

#include "q1/market_data_a.hpp"
#include "q1/market_data_b.hpp"
#include "q1/market_data_processor.hpp"

namespace cpptest {

TEST(TestMarketDataProcessor, MarketDataA) {
    auto data = MarketDataA();
    strcpy(data.instrumentId, "MarketDataA");
    data.askPrice1 = 2.33;
    data.bidPrice1 = 2.22;

    EXPECT_EQ(data.instrumentId, MarketDataTraits<MarketDataA>::GetSymbol(data));
    EXPECT_EQ(data.askPrice1, MarketDataTraits<MarketDataA>::GetAskPrice1(data));
    EXPECT_EQ(data.bidPrice1, MarketDataTraits<MarketDataA>::GetBidPrice1(data));
}

TEST(TestMarketDataProcessor, MarketDataB) {
    auto data = MarketDataB();
    data.code = "MarketDataB";
    data.askPrice[0] = 2.33;
    data.bidPrice[0] = 2.22;

    EXPECT_EQ(data.code, MarketDataTraits<MarketDataB>::GetSymbol(data));
    EXPECT_EQ(data.askPrice[0], MarketDataTraits<MarketDataB>::GetAskPrice1(data));
    EXPECT_EQ(data.bidPrice[0], MarketDataTraits<MarketDataB>::GetBidPrice1(data));
}

TEST(TestMarketDataProcessor, onQuote) {
    auto data = MarketDataA();
    strcpy(data.instrumentId, "MarketDataA");
    data.askPrice1 = 2.33;
    data.bidPrice1 = 2.22;

    onQuoteA(data);

    auto data2 = MarketDataB();
    data2.code = "MarketDataB";
    data2.askPrice[0] = 2.33;
    data2.bidPrice[0] = 2.22;

    onQuoteB(data2);
}

} // namespace cpptest
