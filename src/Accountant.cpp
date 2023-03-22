#include "Accountant.h"

void BuySellRepeat_NS::Accountant::ReportTradingStart(const std::string &tradingPair, const double &currencyToBuyQuantity, const double &lossPercentToSell, const double &profitPercentToBuy, const unsigned int &idleTimeToSellSeconds)
{
    outStream << "Starting trade with parameters:" << std::endl <<
    "Trading pair " << tradingPair << std::endl <<
    "Initial buy qty " <<  std::to_string(currencyToBuyQuantity) << std::endl <<
    "Sell if loss higher then " << std::to_string(lossPercentToSell) << "%" << std::endl << 
    "Buy if profit higher then " << std::to_string(profitPercentToBuy) << "%" << std::endl <<
    "Sell if no signinificant changes after " << std::to_string(idleTimeToSellSeconds) << "s" << std::endl;
}

void BuySellRepeat_NS::Accountant::ReportBuyOperation(const std::time_t &timestamp, const double &price, const double &currencyQty, const double &spentQty)
{
    outStream << std::endl << std::ctime(&timestamp) << ": " <<
    "Buy  " << currencyQty << " " << tradingCurrencySymbol << 
    " for " << spentQty << " " << myCurrencySymbol << std::endl << std::endl;
}

void BuySellRepeat_NS::Accountant::ReportSellOperation(const std::time_t &timestamp, const double &price, const double &currencyQty, const double &recvQty)
{
    outStream << std::endl << std::ctime(&timestamp) << ": " <<
    "Sell " << currencyQty << " " << tradingCurrencySymbol << 
    " for " << recvQty << " " << myCurrencySymbol << std::endl << std::endl;
}

void BuySellRepeat_NS::Accountant::ReportBalance(const std::time_t &timestamp, const double &myCurrencyBalance, const double &tradingCurrencyBalance) 
{
    outStream << std::ctime(&timestamp) << myCurrencySymbol << ": " << std::to_string(myCurrencyBalance) << "\t" <<
    tradingCurrencySymbol << ": " << std::to_string(tradingCurrencyBalance) << std::endl <<
    "tprofits: " << std::to_string(allProfits) << " " << myCurrencySymbol << "\tlosses:" << std::to_string(allLosses) << " " <<myCurrencySymbol << 
    "\tnet: " << std::to_string(net) << std::endl;
}

void BuySellRepeat_NS::Accountant::ReportPriceAndDiff(const double &price, const double &diff)
{
    outStream <<  tradingCurrencySymbol + myCurrencySymbol << ": " <<
    std::to_string(price) << " diff " << std::to_string(diff) + "%" << std::endl;
}

void BuySellRepeat_NS::Accountant::AccountNetChange(const double &netChange)
{
    if (netChange > 0)
        allProfits += netChange;
    if (netChange < 0)
        allLosses += netChange;
    net += netChange;
}
