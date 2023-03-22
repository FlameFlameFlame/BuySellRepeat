#pragma once

#include <ostream>

#include "CommonTypes.h"

namespace BuySellRepeat_NS
{

class Accountant
{
private:
    double allProfits = 0;
    double allLosses = 0;
    double net = 0;

    std::string myCurrencySymbol;
    std::string tradingCurrencySymbol;

    std::ostream& outStream;
public:
    Accountant(std::ostream& os, const std::string& tradingPair): outStream(os) 
    {
        std::copy(tradingPair.begin(), tradingPair.begin() + 3, std::back_inserter(tradingCurrencySymbol));
        std::copy(tradingPair.begin() + 3, tradingPair.end(), std::back_inserter(myCurrencySymbol));
    };
    ~Accountant() = default;


    void ReportTradingStart(const std::string& _tradingPair, const double& currencyToBuyQuantity, const double& _lossPercentToSell, const double& _profitPercentToBuy, const unsigned int& idleTimeToSellSeconds);
    void ReportBuyOperation(const std::time_t& timestamp, const double& price, const double& currencyQty, const double& spentQty);
    void ReportSellOperation(const std::time_t& timestamp, const double& price, const double& currncyQty, const double& recvQty);
    void ReportBalance(const std::time_t& timestamp, const double& myCurrencyBalance, const double& tradingCurrencyBalance);
    void ReportPriceAndDiff(const double& price, const double& diff);
    void AccountNetChange(const double& netChange);
    void ReportWaitingEnd(const int& duration)
    {
        outStream << "Order is fullfilled in " << duration << "ms" << std::endl;
    }
};
}
