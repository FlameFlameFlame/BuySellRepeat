#include "Trader.h"

#include <chrono>

namespace BuySellRepeat_NS
{
    std::string Trader::SellCurrency(const double &quantity)
    {
        using namespace std::chrono;
        const auto curTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return webIO.SendSellRequest(tradingPair, quantity, currentPrice, curTime);
    }

    std::string Trader::BuyCurrency(const double &quantity)
    {
        using namespace std::chrono;
        const auto curTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return webIO.SendBuyRequest(tradingPair, quantity, currentPrice, curTime);
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

void Trader::UpdatePrice()
{
    previousTickPrice = currentPrice;
    currentPrice = webIO.GetPrice(tradingPair);
}

}
