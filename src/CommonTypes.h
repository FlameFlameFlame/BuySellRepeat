#pragma once

#include <map>
#include <string>
#include <utility>

namespace BuySellRepeat_NS
{

struct TradeResults
{
    double allProfits;
    double allLosses;
    double net;
};

using Vallets = std::map<std::string, double>;

enum class QueryType
{
    PING = 0,
    SERVER_TIME,
    TICKER,
    SELL_REQUEST,
    BUY_REQUEST,
    USER_DATA,
    ORDER_QUERY
};

enum class ResponseType
{
    SERVER_TIME,
    TICKER,
    USER_DATA,
    BUY_REQUEST_ACK,
    SELL_REQUEST_ACK,
    ORDER_QUERY
};
}