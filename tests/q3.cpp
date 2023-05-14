#include <gtest/gtest.h>

#include "q3/api.hpp"
#include "q3/order_rtn.hpp"

namespace cpptest {

TEST(TestApi, SimpleTest) {
    Api::Register(std::make_shared<ModuleAProxy>(std::make_shared<ModuleA>()));
    Api::Register(std::make_shared<ModuleBProxy>(std::make_shared<ModuleB>()));
    Api::Register(std::make_shared<ModuleCProxy>(std::make_shared<ModuleC>()));
    Api::OnRtnOrder(OrderRtn());
}

}