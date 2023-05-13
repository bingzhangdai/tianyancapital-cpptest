#pragma once

#include <cstdint>

namespace cpptest {

struct Order {
    char instrumentId[20];  //股票代码，如: 111111.SH, 222222.SH
    uint64_t accountId; //报单账户（对应某个产品）
    double price;       //报单价格
    int32_t qty;        //报单数量
    // 其他字段...
};

}
