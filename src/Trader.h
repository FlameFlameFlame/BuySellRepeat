#pragma once

#include "CommonTypes.h"
#include "Display.h"
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
        int tickMilliseconds = 1000;
        double currencyToBuyOrSellQuantity = 0;
        double cycleStartPrice = 0;

        std::string myCurrencySymbol;
        std::string tradingCurrencySymbol;
        Vallets portfolio;

        WebIO& webIO;
        Display& acc;

        bool doTrade;

        // updated every sell
        TradeResults results;
        // updated every buy
        double spentToBuy = 0.0;

    private:
        void TradingLoop();
        void TradingCycle();
        // returns true if sold or bought in this tick
        bool Tick();
        double CalculatePercentageDiff();
        double AwaitForOrderFullfillment(const long long& orderId) const;
        void AccountTradeResults(const double& diff);

    public:
        Trader(const std::string& _tradingPair, const double& currencyToBuyQuantity, const double& _lossPercentToSell, const double& _profitPercentToBuy, const unsigned int& idleTimeToSellSeconds,  WebIO& wio, Display& acc) :
            lossPercentToSell(_lossPercentToSell), profitPercentToBuy(_profitPercentToBuy), tradingPair(_tradingPair), currencyToBuyOrSellQuantity(currencyToBuyQuantity), idleTimeToSell(idleTimeToSellSeconds), webIO(wio), acc(acc)
            {
                std::copy(tradingPair.begin() + 3, tradingPair.end(), std::back_inserter(myCurrencySymbol));
                std::copy(tradingPair.begin(), tradingPair.begin() + 3, std::back_inserter(tradingCurrencySymbol));
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
        
        double SellCurrency(const double& quantity);
        double BuyCurrency(const double& quantity);

        double SellAllCurrency();
        double BuyCurrenctyForAll();

        static std::time_t GetCurrentTimestamp();
        static std::time_t GetCurrentTimestampSeconds()
        {
            return GetCurrentTimestamp() / 1000;
        };

        void StartTrading();
        void StopTrading()
        {
            doTrade = false;
        }

        TradeResults GetTradeResults() const
        {
            return results;
        }
};

}