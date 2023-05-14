#pragma once

#include "q2/order.hpp"
#include "q3/order_rtn.hpp"
#include "q4/concurrent_queue.hpp"

#include <mutex>
#include <thread>

namespace cpptest {

using namespace std::chrono_literals;

struct MarketData {
};

class Strategy {
    std::mutex m_lock;
    bool m_orderInternal;
    ConcurrentQueue<OrderRtn> m_queue;

    FRIEND_TEST(TestStrategy, SendOrderInternal);

public:

    Strategy(bool orderInternal): m_orderInternal(orderInternal) {
        if (orderInternal) {
            std::thread([this]() {
                while (true) {
                    auto rtn = m_queue.Dequeue();
                    OnRtnOrder(OrderRtn());
                }
            }).detach();
        }
    }

    void OnMarketData(const MarketData& md) {
        std::unique_lock<std::mutex> lock(m_lock);
        Order order;
        // 获取策略内部数据，并结合行情做一些计算
        // .......
        if (m_orderInternal) {
            SendOrderInternal(order);
        }
        else {
            SendOrderExternal(order);
        }

    }

    void SendOrderInternal(const Order& order) {
        // 报到内部程序
        // ....

        /// 认为订单已成交，请完成回报触发逻辑。
        // push to queue for background callback
        m_queue.Enqueue(OrderRtn());
    }

    void SendOrderExternal(const Order& order) {
        // 报到外部程序
        // ....
    }

    void OnRtnOrder(const OrderRtn& rtn) {
        std::unique_lock<std::mutex> lock(m_lock);
        // 仓位更新，log打印，策略状态维护等
        // ....
    }


};

}

