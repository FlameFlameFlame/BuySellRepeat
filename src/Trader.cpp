#include "Trader.h"

namespace BuySellRepeat_NS
{

void Trader::UpdatePrice()
{
    previousTickPrice = currentPrice;
    currentPrice = webIO.GetPrice(tradingPair);
}

}
