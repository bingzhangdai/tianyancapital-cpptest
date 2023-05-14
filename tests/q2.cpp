#include <gtest/gtest.h>
#include <cstdlib>

#include "q2/order.hpp"
#include "q2/risk.hpp"

namespace cpptest {

std::shared_ptr<IRisk> crate_risk_plan() {
    auto risk = std::unordered_map<uint64_t, std::shared_ptr<IRisk>>{
        { 1, std::make_shared<CancelRateRisk>() },
        { 2, std::make_shared<SequenceRisk>(
            std::vector<std::shared_ptr<IRisk>>{
                std::make_shared<CancelRateRisk>(),
                std::make_shared<BlocklistRisk>(std::unordered_set<std::string>{"123456.SH"})
            }
        )}
    };

    return std::make_shared<ByIdRisk>(risk);
}

TEST(TestRisk, SimpleTest) {
    Risk::Init("/path/to/config_file");
    auto o = Order();
    o.accountId = 1;
    strcpy(o.instrumentId, "123456.SH");
    EXPECT_TRUE(Risk::Check(o));
}

TEST(TestRisk, OrderSuccess) {
    auto o = Order();
    o.accountId = 1;
    auto risk = crate_risk_plan();
    EXPECT_TRUE(risk->Check(o));
}

TEST(TestRisk, OrderInBlocklist) {
    auto o = Order();
    o.accountId = 2;
    strcpy(o.instrumentId, "123456.SH");
    auto risk = crate_risk_plan();
    EXPECT_FALSE(risk->Check(o));
    strcpy(o.instrumentId, "123456.SZ");
    EXPECT_TRUE(risk->Check(o));
}

}
