#pragma once

#include "CommonTypes.h"
#include "WebIO.h"

namespace BuySellRepeat_NS
{

class Trader
{
    private:
        // price drop at which we're selling
        double lossPercentToSell;
        // price rise at which we're buying
        double profitPercentToBuy;
        std::string tradingPair;
        // time without significant price changes to sell
        unsigned int idleTimeToSell;

        double currentPrice = 0;
        double previousTickPrice;
        // "resolution"
        int tickSeconds = 1;
        double currencyToBuyOrSellQuantity = 0;

        std::string myCurrencySymbol;
        std::string tradingCurrencySymbol;
        Vallets portfolio;

        WebIO& webIO;

        bool doTrade;

        private:
        void TradingLoop();
        void TradingCycle();
        // returns true if sold or bought in this tick
        bool Tick();
        double CalculatePercentageDiff();

    public:
        Trader(const std::string& _tradingPair, const double& currencyToBuyQuantity, const double& _lossPercentToSell, const double& _profitPercentToBuy, const unsigned int& idleTimeToSellSeconds,  WebIO& wio) :
            lossPercentToSell(_lossPercentToSell), profitPercentToBuy(_profitPercentToBuy), tradingPair(_tradingPair), currencyToBuyOrSellQuantity(currencyToBuyQuantity), idleTimeToSell(idleTimeToSellSeconds), webIO(wio) 
            {
                std::copy(tradingPair.begin(), tradingPair.begin() + 3, std::back_inserter(myCurrencySymbol));
                std::copy(tradingPair.begin() + 3, tradingPair.end(), std::back_inserter(tradingCurrencySymbol));
                GetValletDataFromServer();
                UpdatePrice();
                if (currencyToBuyOrSellQuantity / currentPrice > GetMyCurrencyQuantity())
                    throw std::logic_error("Trying to buy " + std::to_string(currencyToBuyOrSellQuantity) + " " + tradingCurrencySymbol + " for " + std::to_string(currencyToBuyOrSellQuantity / currentPrice) + " " + myCurrencySymbol + ", but have only " + std::to_string(GetMyCurrencyQuantity()) + " " + myCurrencySymbol);
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

        void StartTrading();
        void StopTrading()
        {
            doTrade = false;
        }
};

}