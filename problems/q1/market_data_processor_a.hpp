#pragma once

#include "q1/market_data_a.hpp"
#include "q1/market_data_processor_base.hpp"

namespace cpptest {

class MarketDataProcessorA: public MarketDataProcessorBase {
public:
    double GetMidPrice(const void *data) override {
        const MarketDataA *val = reinterpret_cast<const MarketDataA *>(data);
        return (val->askPrice1 + val->bidPrice1) / 2;
    }
    const char *GetSymbol(const void *data) override {
        const MarketDataA *val = reinterpret_cast<const MarketDataA *>(data);
        return val->instrumentId;
    }
};


} // namespace cpptest 
