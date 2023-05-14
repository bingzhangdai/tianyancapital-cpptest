#pragma once

#include "q3/order_rtn.hpp"
#include "q3/singleton.hpp"

#include <memory>
#include <vector>

namespace cpptest {

class ModuleA {
public:
    void OnRtnOrderA(const OrderRtn& rtn) {
    }
};

class ModuleB {
public:
    void OnRtnOrderB(const OrderRtn& rtn) {
    }
};

class ModuleC {
public:
    void OnRtnOrderC(const OrderRtn& rtn) {
    }
};

class IModuleProxy {
public:
    virtual void OnRtnOrder(const OrderRtn& rtn) = 0;
};

class ModuleAProxy: public IModuleProxy {
    std::shared_ptr<ModuleA> m_module;
public:
    ModuleAProxy(std::shared_ptr<ModuleA> module): m_module(module) {}
    void OnRtnOrder(const OrderRtn& rtn) {
        m_module->OnRtnOrderA(rtn);
    }
};

class ModuleBProxy: public IModuleProxy {
    std::shared_ptr<ModuleB> m_module;
public:
    ModuleBProxy(std::shared_ptr<ModuleB> module): m_module(module) {}
    void OnRtnOrder(const OrderRtn& rtn) {
        m_module->OnRtnOrderB(rtn);
    }
};

class ModuleCProxy: public IModuleProxy {
    std::shared_ptr<ModuleC> m_module;
public:
    ModuleCProxy(std::shared_ptr<ModuleC> module): m_module(module) {}
    void OnRtnOrder(const OrderRtn& rtn) {
        m_module->OnRtnOrderC(rtn);
    }
};

class Publisher: public Singleton<Publisher> {
private:
    std::vector<std::shared_ptr<IModuleProxy>> m_callbacks;
public:
    void Register(std::shared_ptr<IModuleProxy> callback) {
        m_callbacks.push_back(callback);
    }

    void OnRtnOrder(const OrderRtn& rtn) {
        for (const auto& callback : m_callbacks) {
            callback->OnRtnOrder(rtn);
        }
    }

};

class Api {
public:
    static void Register(std::shared_ptr<IModuleProxy> callback) {
        Publisher::Instance().Register(callback);
    }

    static void OnRtnOrder(const OrderRtn& rtn) {
        Publisher::Instance().OnRtnOrder(rtn);
    }

};

}

