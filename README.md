
## Prerequisite

* cmake >= 3.14
* g++/clang++需要支持至少c++11
* 网络通畅（因为使用了第三方库gtest）

## 编译运行

```bash
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

也可以直接`make`。

## 目录结构

```tree
.
├── CMakeLists.txt
├── README.md
├── makefile
├── problems
│   ├── CMakeLists.txt
│   ├── q1
│   ├── q2
│   ├── q3
│   └── q4
└── tests
    ├── CMakeLists.txt
    ├── q1.cpp
    ├── q2.cpp
    ├── q3.cpp
    └── q4.cpp
```
`problems/*`下为四道题目的代码，`tests/*`下为四个题目的测试样例。

## 1. 不同券商行情处理

因为要求不论接入多少券商，printMidPrice不能修改，有两种办法：

1. 多态，有一个公共的基类`IMarketDataAccessor`，分别有两种继承实现，这时可以采用第三方依赖注入的库或者初始化时直接用`std::unordered_map`来保存数据类型和accessor的映射关系，运行时拿到对应的对象去处理。

2. 使用模板做编译期静态多态。

因为题目要求“性能越高越好”，这里考虑方法2。

## 2. 风控设计

不同的产品有不同的风控策略，但底层的基本需求可能类似（比如有个blocklist阻止某些股票交易，etc.），不同产品的风控策略可以认为是一些最基础的风控策略的串并联。同时题目提到有快速应用某些策略的需求，于是考虑到把代码实现和风控逻辑解耦，如果每次更新策略要重新编译就太慢了。

`IRisk`为风控接口，实现诸如blocklist，检测撤单率，股票市值占比等逻辑。配置文件里用一个DAG来把这些基础的风控逻辑做组合，更新策略如果不是最底层的单元，就只需要改配置文件就好了。

## 3. 回报分发

有两种思路：

1. 发布者订阅者模式。`Api::onRtnOrder`作为发布者，下游有多个订阅者`ModuleA`，`ModuleC`，`ModuleB`等，这样完全解耦。代价是需要维护一个消息列队，三个订阅者去订阅这个队列。我们可以采用`zmq`来做这种嵌入式的消息队列。zmq支持`inproc`，`ipc`，`tcp`等多种协议，`inproc`采用内存通信，代价最小。同时如果要切换协议支持更多的模式，也几乎不需要修改代码。

2. 使用代理模式，把多个Module的的回调用统一的接口包装之后注册到`Api`类里作为回调函数（实现见代码），好处是完全不需要维护队列。

这里我们选择方法2，因为题目中提到“有的模块需要收回报，然后通过tcp连接发给策略”，说明代码本身是想让回报分发尽可能的轻量级，而如果采用方法1，那么模块本身通过zmq接收再通过tcp发送数据出去就显得多此一举了，不如直接订阅`Api`类的zmq socket。

方法1也有实现（`problems/q3/api_zmq_backup.hpp`和`tests/q3_zmq_backup.cpp`），因为做完之后觉得和题目描述有点冲突就重新用方法2做了一遍。

## 4. 回报触发

题目等价于如何在`SendOrderInternal`里调用`OnRtnOrder`，但由于在`OnMarketData`和`OnRtnOrder`里都有锁，这里不能直接调用。一般来说办法有以下几种，我觉得都可以
```cpp
// 内部报单
void Strategy::SendOrderInternal(const Order& order) {
    // 报到内部程序
    // ....

    /// 认为订单已成交，请完成回报触发逻辑。
    m_lock.unlock();
    OnRtnOrder(rtn);
    m_lock.lock();
}
```
或者使用线程池，把`OnRtnOrder`逻辑交给线程池去处理，这样类似于`SendOrderExternal`的执行逻辑了，只是这里是直接移交给线程池而不是回调。

以上两种方法其实从执行逻辑上讲应该等价，因为方法2里，移交给线程池之后，`SendOrderInternal`返回，于是锁释放掉了，而线程池里的线程再重新尝试加锁，处理`OnRtnOrder`里的逻辑，跟方法1的`unlock`再尝试`lock`一致，唯一的区别就在于如果`OnRtnOrder`比较慢，用方法2不会block `SendOrderInternal`的执行。

因为题目提到“可以借助标准库或任何第三方库”，应该是想让采用方法2，用线程池的思路、

这里还有第三种思路，因为`OnRtnOrder`里有锁，所以即使线程池也不能达到并行处理的目的，于是我们可以简单用一个独立的线程来按顺序处理，那么我们可以简单有一个队列，`SendOrderInternal`时候把`OrderRtn`对象push进队列里，一个线程去轮询pop队列，拿到对象处理。**在代码里采用这种方法完成**。

### 更多思考

因为题目要求“不能修改`onMarketData`和`onRtnOrder`函数”，如果可以修改可以这样改，可能会更加简单高效：
1. `std::mutex` 换成 `std::shared_mutex`，使锁可重入
2. 增加`OnRtnOrderUnsafe`函数

```cpp
class Strategy {
    void OnRtnOrderUnsafe(const OrderRtn& rtn) {
        std::unique_lock<std::mutex> lock(m_lock);
        // 仓位更新，log打印，策略状态维护等
        // ....
    }
public:
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
        OnRtnOrderUnsafe(rtn);
    }

    void SendOrderExternal(const Order& order) {
        // 报到外部程序
        // ....
    }

    void OnRtnOrder(const OrderRtn& rtn) {
        std::unique_lock<std::mutex> lock(m_lock);
        // 仓位更新，log打印，策略状态维护等
        OnRtnOrderUnsafe(rtn)
    }
};
```


