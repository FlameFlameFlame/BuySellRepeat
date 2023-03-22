#include "Trader.h"

#include <chrono>

namespace BuySellRepeat_NS
{

std::string Trader::SellCurrency(const double &quantity)
{
    using namespace std::chrono;
    const auto orderId = webIO.SendSellRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
    const auto requestTime = system_clock::now();
    std::optional<double> fullfillmentQty;
    acc.ReportSellOperation(GetCurrentTimestampSeconds(), currentPrice, quantity, quantity * currentPrice);
    do
    {
        fullfillmentQty = webIO.SendOrderQuery(tradingPair, orderId, GetCurrentTimestamp());
    } while (!fullfillmentQty);
    acc.ReportWaitingEnd(duration_cast<milliseconds>(system_clock::now() - requestTime).count());

    if (fullfillmentQty)
    {
        acc.AccountNetChange(fullfillmentQty.value() * currentPrice);
    }
    return {};
}

std::string Trader::BuyCurrency(const double &quantity)
{
    using namespace std::chrono;

    const auto orderId = webIO.SendBuyRequest(tradingPair, quantity, currentPrice, GetCurrentTimestamp());
    const auto requestTime = system_clock::now();
    std::optional<double> fullfillmentQty;
    acc.ReportBuyOperation(GetCurrentTimestampSeconds(), currentPrice, quantity, quantity * currentPrice);
    do
    {
        fullfillmentQty = webIO.SendOrderQuery(tradingPair, orderId, GetCurrentTimestamp());
    } while (!fullfillmentQty);
    acc.ReportWaitingEnd(duration_cast<milliseconds>(system_clock::now() - requestTime).count());

    if (fullfillmentQty)
    {
        acc.AccountNetChange(-fullfillmentQty.value() * currentPrice);
    }
    return {};
}

std::string Trader::SellAllCurrency()
{
    const auto retStr = SellCurrency(portfolio.at(tradingCurrencySymbol));
    return retStr;
}

std::string Trader::BuyCurrenctyForAll()
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
    acc.ReportBalance(GetCurrentTimestampSeconds(), portfolio[myCurrencySymbol], portfolio[tradingCurrencySymbol]);

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
        const auto timeLeft = milliseconds((int) tickSeconds * 1000) - timeSpendMs;
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
    const double diff = CalculatePercentageDiff();
    if (diff > 0 && diff > profitPercentToBuy)
    {
        BuyCurrency(currencyToBuyOrSellQuantity);
        tickCv.notify_one();
        return true;
    }
    if (diff < 0 && diff < -lossPercentToSell)
    {
        SellCurrency(currencyToBuyOrSellQuantity);
        tickCv.notify_one();
        return true;
    }
    return false;
}

double Trader::CalculatePercentageDiff()
{
    return (100 * (currentPrice - cycleStartPrice)) / (currentPrice);
}

void Trader::UpdatePrice()
{
    previousTickPrice = currentPrice;
    currentPrice = webIO.GetPrice(tradingPair);
    acc.ReportPriceAndDiff(currentPrice, CalculatePercentageDiff());
}

void Trader::GetValletDataFromServer()
{
    const auto vallet = webIO.GetUserData(GetCurrentTimestamp());
    portfolio[myCurrencySymbol] = vallet.at(myCurrencySymbol);
    portfolio[tradingCurrencySymbol] = vallet.at(tradingCurrencySymbol);
}
}
