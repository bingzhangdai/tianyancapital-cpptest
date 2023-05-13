#pragma once

#include "q1/market_data_b.hpp"
#include "q1/market_data_processor_base.hpp"

namespace cpptest {

class MarketDataProcessorB : public MarketDataProcessorBase {
public:
    double GetMidPrice(const void *data) override {
        const MarketDataB *val = reinterpret_cast<const MarketDataB *>(data);
        return (val->askPrice[0] + val->bidPrice[0]) / 2;
    }
    const char *GetSymbol(const void *data) override {
        const MarketDataB *val = reinterpret_cast<const MarketDataB *>(data);
        return val->code.c_str();
    }
};

}
