#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "q1/market_data_a.hpp"
#include "q1/market_data_b.hpp"

namespace cpptest {

template<typename T>
void PrintMidPrice(const T& data) {
    using Trait = MarketDataTraits<T>;
    std::cout << "symbol: " << Trait::GetSymbol(data)
        << ", mid price: " << (Trait::GetBidPrice1(data) + Trait::GetBidPrice1(data)) / 2
        << std::endl;
}

void onQuoteA(const MarketDataA& quote) {
    PrintMidPrice(quote);
}

void onQuoteB(const MarketDataB& quote) {
    PrintMidPrice(quote);
}

} // namespace cpptest

