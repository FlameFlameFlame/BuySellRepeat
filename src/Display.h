#pragma once

#include "CommonTypes.h"

#include <ostream>
#include <time.h>

namespace BuySellRepeat_NS
{

class Display
{
private:
    std::string myCurrencySymbol;
    std::string tradingCurrencySymbol;

    std::ostream& outStream;
    const std::string waitingSting = "Waiting for order fullfillment for ";
public:
    Display(std::ostream& os): outStream(os) 
    {
    };
    ~Display() = default;

    void ReportTradingStart(const std::string& _tradingPair, const double& currencyToBuyQuantity, const double& _lossPercentToSell, const double& _profitPercentToBuy, const unsigned int& idleTimeToSellSeconds);
    void ReportBuyOperation(const std::time_t& timestamp, const double& price, const double& currencyQty, const double& spentQty);
    void ReportSellOperation(const std::time_t& timestamp, const double& price, const double& currncyQty, const double& recvQty);
    void ReportBalance(const std::time_t& timestamp, const double& myCurrencyBalance, const double& tradingCurrencyBalance, const TradeResults& results);
    void ReportPriceAndDiff(const double& price, const double& diff);
    void ReportWaitingEnd(const unsigned int& duration);
    void ReportOrderWaiting(const unsigned int& durationSeconds);
    void ReportOrderWaitingStart();
    void SetCurrencySymbols(const std::string& _myCurrencySymbol, const std::string& _tradingCurrencySymbol)
    {
        myCurrencySymbol = _myCurrencySymbol;
        tradingCurrencySymbol = _tradingCurrencySymbol;
    }
};
}
