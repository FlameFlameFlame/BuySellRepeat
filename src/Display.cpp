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
    outStream << std::put_time(std::localtime(&timestamp), "%F %T\t") <<
    "Buy  " << currencyQty << " " << tradingCurrencySymbol << 
    " for " << spentQty << " " << myCurrencySymbol << ". ";
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportSellOperation(const long& timestamp, const double &price, const double &currencyQty, const double &recvQty)
{
    outStream << std::put_time(std::localtime(&timestamp), "%F %T\t") <<
    "Sell " << currencyQty << " " << tradingCurrencySymbol << 
    " for " << recvQty << " " << myCurrencySymbol << ". ";
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportBalance(const long& timestamp, const double& myCurrencyBalance, const double& tradingCurrencyBalance, const TradeResults& results) 
{
    outStream << std::put_time(std::localtime(&timestamp), "%F %T\t") << myCurrencySymbol << ": " << std::to_string(myCurrencyBalance) << "\t" <<
    tradingCurrencySymbol << ": " << std::to_string(tradingCurrencyBalance) <<
    "\tprofits: " << std::to_string(results.allProfits) << " " << myCurrencySymbol << "\tlosses:" << std::to_string(results.allLosses) << " " <<myCurrencySymbol << 
    "\tnet: " << std::to_string(results.net) << " " << myCurrencySymbol << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportPriceAndDiff(const long& timestamp, const double &price, const double &diff)
{
    outStream << std::put_time(std::localtime(&timestamp), "%F %T\t") << tradingCurrencySymbol + myCurrencySymbol << ": " <<
    std::to_string(price) << "\t\tdiff: " << std::to_string(diff) + "%" << std::endl;
    pricesFile << std::put_time(std::localtime(&timestamp), "%F %T") << "," << std::to_string(price) << std::endl;
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportWaitingEnd(const unsigned int &duration)
{
    std::strstream ss;
    ss  << "Order is fullfilled in " << duration << "ms" << std::endl;
    outStream << ss.str();
    outStream.flush();
}

void BuySellRepeat_NS::Display::ReportOrderWaiting(const unsigned int &durationSeconds)
{
    std::thread t1 ([&]{
    // remove 's'
    outStream << "\b ";
    // remove left symbols
    for (int i = 0; i < durationSeconds%10 + 1; ++i)
    {
        outStream << '\b';
    }
    outStream << std::to_string(durationSeconds);});
    t1.detach();
}

void BuySellRepeat_NS::Display::ReportOrderWaitingStart()
{
    std::thread t1([&]{
    outStream << waitingSting << "0s";});
    t1.detach();
}
