#pragma once

#include "CommonTypes.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <time.h>

namespace BuySellRepeat_NS
{

class Display
{
private:
    std::string myCurrencySymbol;
    std::string tradingCurrencySymbol;
    std::fstream& pricesFile;
    std::ostream& outStream;
    const std::string waitingSting = "Waiting for order fullfillment for ";
public:
    Display(std::ostream& os, std::fstream& csvFile): outStream(os), pricesFile(csvFile)
    {
    };
    ~Display()
    {
        pricesFile.flush();
    }

    void ReportTradingStart(const std::string& _tradingPair, const double& currencyToBuyQuantity, const double& _lossPercentToSell, const double& _profitPercentToBuy, const unsigned int& idleTimeToSellSeconds);
    void ReportBuyOperation(const long& timestamp, const double& price, const double& currencyQty, const double& spentQty);
    void ReportSellOperation(const long& timestamp, const double& price, const double& currncyQty, const double& recvQty);
    void ReportBalance(const long& timestamp, const double& myCurrencyBalance, const double& tradingCurrencyBalance, const TradeResults& results);
    void ReportPriceAndDiff(const long& timestamp, const double& price, const double& diff);
    void ReportWaitingEnd(const long& timestamp, const unsigned int& duration);
    void ReportOrderWaiting(const long& timestamp, const unsigned int& durationSeconds, const double& fullfilledQty);
    void ReportOrderWaitingStart(const long& timestamp);
    void SetCurrencySymbols(const std::string& _myCurrencySymbol, const std::string& _tradingCurrencySymbol)
    {
        myCurrencySymbol = _myCurrencySymbol;
        tradingCurrencySymbol = _tradingCurrencySymbol;
    }
    static void AddTimeAndDataStamp(const long& timestamp, std::ostream& s)
    {
        s << std::put_time(std::localtime(&timestamp), "%F %T\t");
    }
};
}
