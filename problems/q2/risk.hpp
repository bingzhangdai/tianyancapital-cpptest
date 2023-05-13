#pragma once

#include "q2/order.hpp"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <mutex>

namespace cpptest {

class IRisk {
    std::mutex m_mutex;
public:
    virtual bool Check(const Order&) = 0;
};

class RiskA: public IRisk {
public:
    RiskA(const char *file_path) {
        // do initialization
    }
    bool Check(const Order& order) override {
        std::unique_lock<std::mutex> lk(m_mutex);
        return true;
    }
};

class RiskB: public IRisk {
public:
    RiskB(const char *file_path) {
        // do initialization
    }
    bool Check(const Order& order) override {
        std::unique_lock<std::mutex> lk(m_mutex);
        return false;
    }
};

class RiskC: public IRisk {
public:
    RiskC(const char *file_path) {
        // do initialization
    }
    bool Check(const Order& order) override {
        std::unique_lock<std::mutex> lk(m_mutex);
        return true;
    }
};

class Risk {
private:
    static Risk& GetInstance() {
        // thread-safe since c++11
        static Risk instance;
        return instance;
    }
    std::unordered_map<uint64_t, IRisk *> m_trategies;
    std::unordered_set<std::string> m_blocklist;
    Risk() {
        // load the main risk strategy from file
        m_blocklist.insert("123456.SH");
        // then, load the risk stategy for each account
        // the config structure can be like,
        //  accountIdA, file_pathA
        //  accountIdB, file_pathB
        //  ..., ...
        // for each line, load the strategy by calling `Risk(const char *)`
        // and initialize m_trategies container
        m_trategies[1] = new RiskA("path/to/config/a");
        m_trategies[2] = new RiskB("path/to/config/b");
        m_trategies[3] = new RiskC("path/to/config/c");
    }
    bool CommonCheck(const Order& order) {
        if (m_blocklist.count(order.instrumentId)) {
            return false;
        }
        return true;
    }
public:
    static bool Check(const Order& order) {
        // this is the common check
        if (!GetInstance().CommonCheck(order)) {
            return false;
        }
        if (!GetInstance().m_trategies.count(order.accountId)) {
            // log error
            return false;
        }
        return GetInstance().m_trategies[order.accountId]->Check(order);
    }

    Risk(const Risk&) = delete;
    Risk& operator=(const Risk&) = delete;
};

}
