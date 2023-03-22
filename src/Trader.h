#pragma once

#include "CommonTypes.h"
#include "Display.h"
#include "WebIO.h"

#include <argumentum/argparse.h>

namespace BuySellRepeat_NS
{

    class TradingParameters : public argumentum::Options
{
    public:
        std::string tradingCurrency;
        std::string myCurrency;
        double tradingQty;
        double lossPercentToSell;
        double profitPercentToSell;
        int idleTimeToSellSeconds;
    protected:
        void add_parameters(argumentum::ParameterConfig& params) override
        {
            params.add_parameter(tradingCurrency, "--trading_currency", "-T").nargs(1).help("Currency to buy or sell");
            params.add_parameter(myCurrency, "--my_currency", "-M").nargs(1).help("Your currency");
            params.add_parameter(tradingQty, "--trading_qty", "-Q").nargs(1).help("Amount of currency you want to trade");
            params.add_parameter(lossPercentToSell, "--loss_to_sell", "-L").nargs(1).help("Currencly value loss percentage after which currency will be sold. This value should be positive");
            params.add_parameter(profitPercentToSell, "--profit_to_sell", "-P").nargs(1).help("Currencly value gain percentage after which currency will be sold");
            params.add_parameter(idleTimeToSellSeconds, "--idle_time_seconds", "-I").nargs(1).help("Time period without significant changes after which currency will be sold");
        }

};

class Trader
{
    private:
        // price drop at which we're selling
        double lossPercentToSell;
        // price rise at which we're buying
        double profitPercentToSell;
        // time without significant price changes to sell
        unsigned int idleTimeToSell;

        double currentPrice = 0;
        double previousTickPrice;
        // "resolution"
        int tickMilliseconds = 1000;
        double currencyToBuyOrSellQuantity = 0;
        double cycleStartPrice = 0;

        const std::string myCurrencySymbol;
        const std::string tradingCurrencySymbol;
        std::string tradingPair;

        Vallets portfolio;

        WebIO& webIO;
        Display& display;

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
        Trader(const TradingParameters& params, WebIO& wio, Display& display) :
            lossPercentToSell(params.lossPercentToSell), profitPercentToSell(params.profitPercentToSell), currencyToBuyOrSellQuantity(params.tradingQty), idleTimeToSell(params.idleTimeToSellSeconds), webIO(wio), display(display),
            tradingCurrencySymbol(params.tradingCurrency), myCurrencySymbol(params.myCurrency)
            {
                tradingPair = tradingCurrencySymbol + myCurrencySymbol;
                display.SetCurrencySymbols(myCurrencySymbol, tradingCurrencySymbol);
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