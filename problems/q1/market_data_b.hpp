#pragma once

#include <cstdint>
#include <string>

struct MarketDataB {
    std::string code;      // 股票代码
    double askPrice[5];    // 申卖价
    uint64_t askVolume[5]; // 申卖量
    double bidPrice[5];    // 申买价
    uint64_t bidVolume[5]; // 申买量
    // 其余字段
    // .......
};
