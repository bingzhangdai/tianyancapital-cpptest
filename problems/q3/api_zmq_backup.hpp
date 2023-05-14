#pragma once

#include <zmq.hpp>
#include <optional>
#include <string>

#include "q3/order_rtn.hpp"

namespace cpptest {

const std::string conncetion_string = "inproc://cpptest";

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
        zmq::message_t msg(&rtn, sizeof(OrderRtn));
        GetInstance().sock.send(std::move(msg), zmq::send_flags::none);
    }
};

class IProxy {
    virtual OnRtnOrder(const OrderRtn& rtn) = 0;
};




class ModuleA {
private:
    zmq::context_t ctx;
    zmq::socket_t sock;
public:
    ModuleA(): sock(ctx, zmq::socket_type::sub) {
        sock.connect(conncetion_string);
        sock.set(zmq::sockopt::subscribe, "");
    }

    std::optional<OrderRtn> GetMessage() {
        zmq::message_t msg;
        auto res = sock.recv(msg, zmq::recv_flags::none);
        if (!res) {
            // log error
            return {};
        }
        return *msg.data<OrderRtn>();
    }
};

class ModuleB {
private:
    zmq::context_t ctx;
    zmq::socket_t sock;
public:
    ModuleB(): sock(ctx, zmq::socket_type::sub) {
        sock.connect(conncetion_string);
        sock.set(zmq::sockopt::subscribe, "");
    }

    std::optional<OrderRtn> GetMessage() {
        zmq::message_t msg;
        auto res = sock.recv(msg, zmq::recv_flags::none);
        if (!res) {
            // log error
            return {};
        }
        return *msg.data<OrderRtn>();
    }
};

}
