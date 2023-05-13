#pragma once

namespace cpptest {

class MarketDataProcessorBase {
public:
    virtual double GetMidPrice(const void *data) = 0;
    virtual const char *GetSymbol(const void *data) = 0;
};

}
