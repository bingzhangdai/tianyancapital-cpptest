#pragma once

#include <zmq.hpp>
#include <optional>

#include "q3/order_rtn.hpp"

namespace cpptest {

const char *conncetion_string = "ipc://cpptest";

class Api {
    zmq::context_t ctx;
    zmq::socket_t sock;
private:
    Api(): sock(ctx, zmq::socket_type::pub) {
        sock.bind(conncetion_string);
    }

    static Api& GetInstance() {
        static Api instance;
        return instance;
    }

public:
    Api(const Api&) = delete;
    Api& operator=(const Api&) = delete;

    static void OnRtnOrder(const OrderRtn& rtn) {
        auto buf = zmq::buffer(&rtn, sizeof(OrderRtn));
        GetInstance().sock.send(buf, zmq::send_flags::dontwait);
    }
};

class ModuleA {
private:
    zmq::context_t ctx;
    zmq::socket_t sock;
public:
    ModuleA(): sock(ctx, zmq::socket_type::sub) {
        sock.connect(conncetion_string);
    }

    std::optional<OrderRtn> GetMessage() {
        char buf[sizeof(OrderRtn)];
        auto res = sock.recv(zmq::buffer(buf));
        if (!res) {
            // log error
            return {};
        }
        return *reinterpret_cast<OrderRtn *>(buf);
    }
};

class ModuleB {
private:
    zmq::context_t ctx;
    zmq::socket_t sock;
public:
    ModuleB(): sock(ctx, zmq::socket_type::sub) {
        sock.connect(conncetion_string);
    }

    std::optional<OrderRtn> GetMessage() {
        char buf[sizeof(OrderRtn)];
        auto res = sock.recv(zmq::buffer(buf));
        if (!res) {
            // log error
            return {};
        }
        return *reinterpret_cast<OrderRtn *>(buf);
    }
};

}
