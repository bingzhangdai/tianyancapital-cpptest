#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "q1/market_data_processor_a.hpp"
#include "q1/market_data_processor_b.hpp"
#include "q1/market_data_processor_base.hpp"

namespace cpptest {

enum class Securities {
  A,
  B,
};

static std::unordered_map<Securities, MarketDataProcessorBase*> processors{
    {Securities::A, new MarketDataProcessorA()},
    {Securities::B, new MarketDataProcessorB()}};

class MarketDataProcessor {
public:
    static void PrintMidPrice(const void *data, Securities type) {
        MarketDataProcessorBase * p = processors[type];
        std::cout << "symbol: " << p->GetSymbol(data)
                  << ",mid price: " << p->GetMidPrice(data) 
                  << std::endl;
    }
};

void onQuoteA(const MarketDataA& quote) {
    MarketDataProcessor::PrintMidPrice(&quote, Securities::A);
}

void onQuoteB(const MarketDataB& quote) {
    MarketDataProcessor::PrintMidPrice(&quote, Securities::B);
}

} // namespace cpptest

