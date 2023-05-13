#pragma once

#include "q2/order.hpp"

#include <mutex>
#include <lock>

namespace cpptest {

struct MarketData {
};

class Stategy {
    std::mutex m_lock;
    bool m_orderInternal;

public:
    strategy(bool orderInternal): m_orderInternal(orderInternal) {

    }

    void OnMarketData(const MarketData& md) {
        std::unique_lock<std::mutex> lock(m_lock);
        Order order;
        // 获取策略内部数据，并结合行情做一些计算
        // .......
        if (m_orderInternal) {
            sendOrderInternal(order);
        }
        else {
            sendOrderExternal(order);
        }

    }

    void SendOrderInternal(const Order& order) {
        // 报到内部程序
        // ....

        /// 认为订单已成交，请完成回报触发逻辑。
        m_lock.unlock()
        onRtnOrder(order);
    }

    void SendOrderExternal(const Order& order) {
        // 报到外部程序
        // ....
    }

    void onRtnOrder(const OrderRtn& rtn) {
        std::unique_lock<std::mutex> lock(m_lock);
        // 仓位更新，log打印，策略状态维护等
        // ....
    }


}

}
