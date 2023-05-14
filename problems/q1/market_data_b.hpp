#pragma once

#include <cstdint>
#include <string>

namespace cpptest {

struct MarketDataB {
    std::string code;      // 股票代码
    double askPrice[5];    // 申卖价
    uint64_t askVolume[5]; // 申卖量
    double bidPrice[5];    // 申买价
    uint64_t bidVolume[5]; // 申买量
    // 其余字段
    // .......
};

template <typename T>
struct MarketDataTraits;

template <>
struct MarketDataTraits<MarketDataB> {
    static const char *GetSymbol(const MarketDataB& data) {
        return data.code.c_str();
    }
    static double GetBidPrice1(const MarketDataB& data) {
        return data.bidPrice[0];
    }
    static double GetAskPrice1(const MarketDataB& data) {
        return data.askPrice[0];
    }
};

}
