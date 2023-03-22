#include "Trader.h"

#include <chrono>

namespace BuySellRepeat_NS
{

double Trader::SellCurrency(const double &quantity)
{
    const auto orderId = webIO.SendSellRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
    acc.ReportSellOperation(GetCurrentTimestampSeconds(), currentPrice, quantity, quantity * currentPrice);
    const auto soldQty = AwaitForOrderFullfillment(orderId);
    AccountTradeResults(soldQty * currentPrice - spentToBuy);
    return soldQty;
}

double Trader::BuyCurrency(const double &quantity)
{
    spentToBuy = quantity * currentPrice;
    const auto orderId = webIO.SendBuyRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
    acc.ReportBuyOperation(GetCurrentTimestampSeconds(), currentPrice, quantity, quantity * currentPrice);
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
    acc.ReportTradingStart(tradingPair, currencyToBuyOrSellQuantity, lossPercentToSell, profitPercentToBuy, idleTimeToSell);
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
    acc.ReportBalance(GetCurrentTimestampSeconds(), portfolio[myCurrencySymbol], portfolio[tradingCurrencySymbol], results);

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
    acc.ReportPriceAndDiff(currentPrice, CalculatePercentageDiff());
    const double diffPercent = CalculatePercentageDiff();
    if (diffPercent > profitPercentToBuy || diffPercent < -lossPercentToSell)
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
    // acc.ReportOrderWaitingStart();
    do
    {
        fullfillmentQty = webIO.SendOrderQuery(tradingPair, orderId, GetCurrentTimestamp());
        // acc.ReportOrderWaiting(duration_cast<seconds>(system_clock::now() - requestTime).count());
        std::this_thread::sleep_for(milliseconds(500));
    } 
    while (!fullfillmentQty);
    acc.ReportWaitingEnd(duration_cast<milliseconds>(system_clock::now() - requestTime).count());
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
    currentPrice = webIO.GetPrice(tradingPair);
}

void Trader::GetValletDataFromServer()
{
    const auto vallet = webIO.GetUserData(GetCurrentTimestamp());
    portfolio[myCurrencySymbol] = vallet.at(myCurrencySymbol);
    portfolio[tradingCurrencySymbol] = vallet.at(tradingCurrencySymbol);
}
}
