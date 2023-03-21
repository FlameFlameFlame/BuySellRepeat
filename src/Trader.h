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
                std::copy(tradingPair.begin(), tradingPair.begin() + 3, std::back_inserter(myCurrencySymbol));
                std::copy(tradingPair.begin() + 3, tradingPair.end(), std::back_inserter(tradingCurrencySymbol));
            };
        ~Trader() = default;

        void UpdatePrice();
        void GetValletDataFromServer();

        double GetPrice()
        {
            return currentPrice;
        }

        double GetTradingCurrencyQuantity() const
        {
            return portfolio.at(tradingCurrencySymbol);
        }

        double GetMyCurrencyQuantity() const
        {
            return portfolio.at(myCurrencySymbol);
        }
        
        std::string SellCurrency(const double& quantity);
        std::string BuyCurrency(const double& quantity);

        std::string SellAllCurrency();
        std::string BuyCurrenctyForAll();

        static std::time_t GetCurrentTimestamp();
};

}