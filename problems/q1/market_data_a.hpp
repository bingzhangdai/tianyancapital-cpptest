#pragma once

#include <cstdint>

namespace cpptest {

struct MarketDataA {
    char instrumentId[20]; // 股票代码
    double askPrice1;      // 申卖价一
    uint64_t askVolume1;   // 申卖量一
    double bidPrice1;      // 申买价一
    uint64_t bidVolume1;   // 申买量一
    double askPrice2;      // 申卖价二
    uint64_t askVolume2;   // 申卖量二
    double bidPrice2;      // 申买价二
    uint64_t bidVolume2;   // 申买量二
    double askPrice3;      // 申卖价三
    uint64_t askVolume3;   // 申卖量三
    double bidPrice3;      // 申买价三
    uint64_t bidVolume3;   // 申买量三
    double askPrice4;      // 申卖价四
    uint64_t askVolume4;   // 申卖量四
    double bidPrice4;      // 申买价四
    uint64_t bidVolume4;   // 申买量四
    double askPrice5;      // 申卖价五
    uint64_t askVolume5;   // 申卖量五
    double bidPrice5;      // 申买价五
    uint64_t bidVolume5;   // 申买量五
    // 其余字段
    // .......
};
}
