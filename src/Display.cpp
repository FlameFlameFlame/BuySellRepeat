#include "Display.h"

#include <thread>
#include <iomanip>
#include <strstream>

void BuySellRepeat_NS::Display::ReportTradingStart(const std::string &tradingPair, const double &currencyToBuyQuantity, const double &lossPercentToSell, const double &profitPercentToSell, const unsigned int &idleTimeToSellSeconds)
{
    outStream << "Starting trade with parameters:" << std::endl <<
    "Trading pair " << tradingPair << std::endl <<
    "Initial buy qty " <<  std::to_string(currencyToBuyQuantity) << std::endl <<
    "Sell if loss higher then " << std::to_string(lossPercentToSell) << "%" << std::endl << 
    "Buy if profit higher then " << std::to_string(profitPercentToSell) << "%" << std::endl <<
    "Sell if no signinificant changes after " << std::to_string(idleTimeToSellSeconds) << "s" << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportBuyOperation(const long& timestamp, const double &price, const double &currencyQty, const double &spentQty)
{
    AddTimeAndDataStamp(timestamp, outStream);
    outStream << 
    "buy  " << currencyQty << " " << tradingCurrencySymbol << 
    " for " << spentQty << " " << myCurrencySymbol << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportSellOperation(const long& timestamp, const double &price, const double &currencyQty, const double &recvQty)
{
    AddTimeAndDataStamp(timestamp, outStream);
    outStream << 
    "sell " << currencyQty << " " << tradingCurrencySymbol << 
    " for " << recvQty << " " << myCurrencySymbol << std::endl;;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportBalance(const long& timestamp, const double& myCurrencyBalance, const double& tradingCurrencyBalance, const TradeResults& results) 
{
    AddTimeAndDataStamp(timestamp, outStream);
    outStream <<  myCurrencySymbol << ": " << std::to_string(myCurrencyBalance) << "\t" <<
    tradingCurrencySymbol << ": " << std::to_string(tradingCurrencyBalance) <<
    "\tprofits: " << std::to_string(results.allProfits) << " " << myCurrencySymbol << "\tlosses:" << std::to_string(results.allLosses) << " " <<myCurrencySymbol << 
    "\tnet: " << std::to_string(results.net) << " " << myCurrencySymbol << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportPriceAndDiff(const long& timestamp, const double &price, const double &diff)
{
    AddTimeAndDataStamp(timestamp, outStream);
    outStream << tradingCurrencySymbol + myCurrencySymbol << ": " <<
    std::to_string(price) << "\t\tdiff: " << std::to_string(diff) + "%" << std::endl;
    pricesFile << std::put_time(std::localtime(&timestamp), "%F %T") << "," << std::to_string(price) << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportWaitingEnd(const long& timestamp, const unsigned int &duration)
{
    AddTimeAndDataStamp(timestamp, outStream);
    outStream  << "order is fullfilled in " << duration << "ms" << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportOrderWaiting(const long& timestamp, const unsigned int &durationSeconds, const double& fullfilledQty)
{
    AddTimeAndDataStamp(timestamp, outStream);
    outStream << "waiting for order fullfillment for " << std::to_string(durationSeconds) << "s\tcurrently fullfilled " <<
    std::to_string(fullfilledQty) << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportOrderWaitingStart(const long& timestamp)
{
    std::thread t1([&]{
    outStream << waitingSting << "0s";});
    t1.detach();
}
