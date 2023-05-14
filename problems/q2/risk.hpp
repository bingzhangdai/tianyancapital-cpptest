#pragma once

#include "q2/order.hpp"

#include <string>
#include <mutex>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace cpptest {

class IRisk {
public:
    virtual bool Check(const Order&) = 0;
};

class BlocklistRisk: public IRisk {
    std::unordered_set<std::string> m_blocklist;
public:
    BlocklistRisk(const std::unordered_set<std::string>& blocklist) {
        m_blocklist = blocklist;
    }
    bool Check(const Order& order) override {
        return m_blocklist.find(order.instrumentId) == m_blocklist.end();
    }
};

class CancelRateRisk: public IRisk {
    std::mutex mtx;
public:
    bool Check(const Order& order) override {
        // just to illustrate we can use lock if we need.
        std::unique_lock<std::mutex> lk(mtx);
        return true;
    }
};

class SequenceRisk: public IRisk {
    std::vector<std::shared_ptr<IRisk>> m_risks;
public:
    SequenceRisk(const std::vector<std::shared_ptr<IRisk>>& risks) {
        m_risks = risks;
    }
    bool Check(const Order& order) override {
        for (const auto& risk : m_risks) {
            if (!risk->Check(order)) return false;
        }
        return true;
    }
};

class ByIdRisk: public IRisk {
    std::unordered_map<uint64_t, std::shared_ptr<IRisk>> m_risks;
public:
    ByIdRisk(std::unordered_map<uint64_t, std::shared_ptr<IRisk>> risks) {
        m_risks = risks;
    }
    bool Check(const Order& order) override {
        if (!m_risks.count(order.accountId)) {
            // log error
            return false;
        }
        return m_risks[order.accountId]->Check(order);
    }
};

// example
class Risk {
    struct Holder {
        std::shared_ptr<IRisk> m_risk;
        explicit Holder(Holder **holder, std::shared_ptr<IRisk> risk): m_risk(risk) {
            *holder = this;
        }
    };
    static Holder *s_holder;
public:
    static void Init(const char *config_file_path) {
        // the follow codes logic should lies in config file
        // here is only used for illustration purpose
        auto risk = std::unordered_map<uint64_t, std::shared_ptr<IRisk>>{
            { 1, std::make_shared<CancelRateRisk>() }
        };
        static Holder holder(&s_holder, std::make_shared<ByIdRisk>(risk));
    }

    static bool Check(const Order& order) {
        if (!s_holder) {
            // log error
            return false;
        }
        return s_holder->m_risk->Check(order);
    }
};

typename Risk::Holder *Risk::s_holder;

}
