#pragma once

namespace cpptest {

template <typename T>
class Singleton {
private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

    struct Holder {
        T value;

        explicit Holder(Holder** holder) :value() {
            *holder = this;
        }
    };

protected:
    Singleton() {}
    ~Singleton() {}

public:
    static T& Instance() {
        // thread-safe since c++11
        static Holder holder(&s_holder);
        return holder.value;
    }

private:
    static Holder* s_holder;
};

template <typename T>
typename Singleton<T>::Holder *Singleton<T>::s_holder;

}