#include "Trader.h"

#include <chrono>

namespace BuySellRepeat_NS
{

double Trader::SellCurrency(const double &quantity)
{
    const auto orderId = webIO.SendSellRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
    display.ReportSellOperation(GetCurrentTimestampSeconds(), currentPrice, quantity, quantity * currentPrice);
    const auto soldQty = AwaitForOrderFullfillment(orderId);
    AccountTradeResults(soldQty * currentPrice - spentToBuy);
    return soldQty;
}

double Trader::BuyCurrency(const double &quantity)
{
    spentToBuy = quantity * currentPrice;
    const auto orderId = webIO.SendBuyRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
    display.ReportBuyOperation(GetCurrentTimestampSeconds(), currentPrice, quantity, quantity * currentPrice);
    return AwaitForOrderFullfillment(orderId);
}

double Trader::SellAllCurrency()
{
    const auto retStr = SellCurrency(portfolio.at(tradingCurrencySymbol));
    return retStr;
}

double Trader::BuyCurrenctyForAll()
{
    const auto retStr = BuyCurrency(portfolio.at(myCurrencySymbol));
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
    display.ReportTradingStart(tradingPair, currencyToBuyOrSellQuantity, lossPercentToSell, profitPercentToSell, idleTimeToSell);
    TradingLoop();
}

void Trader::TradingLoop()
{    
    while (doTrade)
    {
        TradingCycle();
    }
}

void Trader::TradingCycle()
{
    using namespace std::chrono;
    using namespace std::chrono_literals;

    GetValletDataFromServer();
    display.ReportBalance(GetCurrentTimestampSeconds(), portfolio[myCurrencySymbol], portfolio[tradingCurrencySymbol], results);

    if (currencyToBuyOrSellQuantity * currentPrice > GetMyCurrencyQuantity())
        throw std::logic_error("Trying to buy " + std::to_string(currencyToBuyOrSellQuantity) + " " + tradingCurrencySymbol + " for " + std::to_string(currencyToBuyOrSellQuantity * currentPrice) + " " + myCurrencySymbol + ", but have only " + std::to_string(GetMyCurrencyQuantity()) + " " + myCurrencySymbol);

    UpdatePrice();
    BuyCurrency(currencyToBuyOrSellQuantity);
    cycleStartPrice = currentPrice;
    const auto cycleStartTime = system_clock::now();
    bool didOperation = false;
    while (!didOperation && duration_cast<seconds>(system_clock::now() - cycleStartTime).count() < idleTimeToSell)
    {
        const auto tickStartTime = system_clock::now();
        didOperation = Tick();

        const auto timeSpendMs = duration_cast<milliseconds>(system_clock::now() - tickStartTime);
        const auto timeLeft = milliseconds(tickMilliseconds) - timeSpendMs;
        if (timeLeft.count() > 0)
            std::this_thread::sleep_for(timeLeft);
    }
    if (!didOperation)
    {
        SellCurrency(currencyToBuyOrSellQuantity);
    }
}

bool Trader::Tick()
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    UpdatePrice();
    display.ReportPriceAndDiff(GetCurrentTimestampSeconds(), currentPrice, CalculatePercentageDiff());
    const double diffPercent = CalculatePercentageDiff();
    if (diffPercent > profitPercentToSell || diffPercent < -lossPercentToSell)
    {
        SellCurrency(currencyToBuyOrSellQuantity);
        return true;
    }
    return false;
}

double Trader::CalculatePercentageDiff()
{
    return (100 * (currentPrice - cycleStartPrice)) / (currentPrice);
}

double Trader::AwaitForOrderFullfillment(const long long& orderId) const
{    
    using namespace std::chrono;
    using namespace std::chrono_literals;

    const auto requestTime = system_clock::now();
    std::optional<double> fullfillmentQty;
    auto queryTimeStart = system_clock::now();
    do
    {
        if (duration_cast<milliseconds>(system_clock::now() - queryTimeStart).count() > tickMilliseconds)
        {
            fullfillmentQty = webIO.SendOrderQuery(tradingPair, orderId, GetCurrentTimestamp());
            queryTimeStart = system_clock::now();
        }
    } 
    while (!fullfillmentQty);
    display.ReportWaitingEnd(duration_cast<milliseconds>(system_clock::now() - requestTime).count());
    return fullfillmentQty.value();
}

void Trader::AccountTradeResults(const double& diff)
{
    if (diff < 0)
        results.allLosses += diff;
    else 
        results.allProfits += diff;
    results.net += diff;
}

void Trader::UpdatePrice()
{
    previousTickPrice = currentPrice;
    const auto getPriceResult = webIO.GetPrice(tradingPair);
    if (getPriceResult)
        currentPrice = getPriceResult.value(); 
}

void Trader::GetValletDataFromServer()
{
    const auto vallet = webIO.GetUserData(GetCurrentTimestamp());
    if (vallet.size() == 0)
        return;
    try 
    {
        portfolio[myCurrencySymbol] = vallet.at(myCurrencySymbol);
    }
    catch (std::out_of_range& e)
    {
        throw std::logic_error (myCurrencySymbol + " is not in your wallet");
    }
    try 
    {
        portfolio[tradingCurrencySymbol] = vallet.at(tradingCurrencySymbol);
    }
    catch (std::out_of_range& e)
    {
        throw std::logic_error (tradingCurrencySymbol + " is not in your wallet");
    }
}
}
