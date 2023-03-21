#include "Trader.h"

#include <chrono>

namespace BuySellRepeat_NS
{

std::string Trader::SellCurrency(const double &quantity)
{
    return webIO.SendSellRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
}

std::string Trader::BuyCurrency(const double &quantity)
{
    return webIO.SendBuyRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
}

std::string Trader::SellAllCurrency()
{
    const auto retStr = SellCurrency(portfolio.at(tradingCurrencySymbol));
    portfolio[tradingCurrencySymbol] = 0;
    return retStr;
}

std::string Trader::BuyCurrenctyForAll()
{
    const auto retStr = BuyCurrency(portfolio.at(myCurrencySymbol));
    portfolio[myCurrencySymbol] = 0;
    return retStr;
}

std::time_t Trader::GetCurrentTimestamp()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Trader::StartTrading()
{
    doTrade = true;
    TradingLoop();
}

void Trader::TradingLoop()
{    
    while (doTrade)
    {
        std::thread tradingThread(&Trader::TradingCycle, this);
        tradingThread.join();
    }
}

void Trader::TradingCycle()
{
    using namespace std::chrono;

    const auto cycleStartTime = system_clock::now();
    bool didOperation = false;
    while (!didOperation && duration_cast<seconds>(cycleStartTime - system_clock::now()).count() < idleTimeToSell)
    {
        didOperation = Tick();
        std::this_thread::sleep_for(seconds(tickSeconds));
    }
    if (!didOperation)
        SellCurrency(currencyToBuyOrSellQuantity);
}

bool Trader::Tick()
{
    UpdatePrice();
    const double diff = CalculatePercentageDiff();
    if (diff > 0 && diff > profitPercentToBuy)
    {
        BuyCurrency(currencyToBuyOrSellQuantity);
        return true;
    }
    if (diff < 0 && diff < -lossPercentToSell)
    {
        SellCurrency(currencyToBuyOrSellQuantity);
        return true;
    }
    return false;
}

double Trader::CalculatePercentageDiff()
{
    return (currentPrice - previousTickPrice) / currentPrice;
}

void Trader::UpdatePrice()
{
    previousTickPrice = currentPrice;
    currentPrice = webIO.GetPrice(tradingPair);
}

void Trader::GetValletDataFromServer()
{
    const auto vallet = webIO.GetUserData(GetCurrentTimestamp());
    portfolio[myCurrencySymbol] = vallet.at(myCurrencySymbol);
    portfolio[tradingCurrencySymbol] = vallet.at(tradingCurrencySymbol);
}
}
