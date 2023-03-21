#include "JsonResponseParser.h"

#include <string>

namespace BuySellRepeat_NS
{

int JsonResponseParser::getResponseStatus() const
{
    if (receivedJson.contains("status"))
        return receivedJson.at("status");
    else
        throw std::logic_error("Received malformed JSON: no status field");
}

std::string JsonResponseParser::getUserData() const
{
    return receivedJson.dump();
}

std::pair<std::string, double> JsonResponseParser::getPriceFromTicker() const
{
    if (receivedJson.contains("result"))
    {
        const auto symbols = receivedJson.at("result").at("symbol");
        const auto price = std::stod(std::string(receivedJson.at("result").at("price")));
        return {symbols, price};
    }
    else
        throw std::logic_error("Received malformed JSON: no result field");
}
time_t JsonResponseParser::getServerTime() const
{
    if (receivedJson.contains("result"))
    {
        return std::stol(receivedJson["result"]["serverTime"].dump());
    }
    else
        throw std::logic_error("Received malformed JSON: no result field");
}

std::string JsonResponseParser::getSellRequestResult() const
{
    return receivedJson.dump();
}

std::string JsonResponseParser::getBuyRequestResult() const
{
    return receivedJson.dump();
}
}
