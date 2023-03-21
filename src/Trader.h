#pragma once

#include "CommonTypes.h"
#include "WebIO.h"

namespace BuySellRepeat_NS
{

class Trader
{
    private:
        double lossPercentToSell;
        double profitPercentToBuy;
        std::string tradingPair;

        double currentPrice = 0;
        double previousTickPrice;
        int tickSeconds = 1;
        double currencyQuantity = 0;

        std::string myCurrencySymbol;
        std::string tradingCurrencySymbol;
        Vallets portfolio;

        WebIO& webIO;
    public:
        Trader(const std::string& _tradingPair, const double& currencyToBuyQuantity, const double& _lossPercentToSell, const double& _profitPercentToBuy, WebIO& wio) :
            lossPercentToSell(_lossPercentToSell), profitPercentToBuy(_profitPercentToBuy), tradingPair(_tradingPair), currencyQuantity(currencyToBuyQuantity), webIO(wio) 
            {
                std::copy(tradingPair.begin(), tradingPair.begin() + 3, myCurrencySymbol.begin());
                std::copy(tradingPair.begin(), tradingPair.begin() + 3, tradingCurrencySymbol.begin());
            };
        ~Trader() = default;
        double GetPrice()
        {
            return currentPrice;
        }
        double GetCurrencyQuantity() const
        {
            return portfolio.at(tradingCurrencySymbol);
        }
        void UpdatePrice();


        std::string SellCurrency(const double& quantity);
        std::string BuyCurrency(const double& quantity);

        std::string SellAllCurrency();
        std::string BuyCurrenctyForAll();
};

}