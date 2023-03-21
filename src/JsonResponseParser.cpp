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

std::map<std::string, double> JsonResponseParser::getUserData() const
{
    std::map<std::string, double> retVal;
    if (receivedJson["result"].contains("balances"))
    {
        // const auto str = receivedJson["result"]["balances"].dump();
        for (const auto& asset: receivedJson["result"]["balances"])
        {
            retVal[asset.at("asset")] = std::stod(asset.at("free").get<std::string>());
        }
        return retVal;
    }
    else
        throw std::logic_error("Received malformed JSON: no balances field");
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
