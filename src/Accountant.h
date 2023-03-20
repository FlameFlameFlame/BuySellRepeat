#pragma once

#include "CommonTypes.h"

namespace BuySellRepeat_NS
{

class Accountant
{
private:
    TradeParams params;
public:
    Accountant(const std::string& tradingPair, const double& lossPercentToSell, const double& profitPercentToBuy, const int& idleTime) :
    params({tradingPair, lossPercentToSell, profitPercentToBuy, idleTime})
    {};
    ~Accountant() = default;
};
}
