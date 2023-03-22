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

long long JsonResponseParser::getSellRequestResult() const
{
    if (receivedJson.contains("result"))
    {
        return std::stol(receivedJson["result"]["orderId"].dump());
    }
    else
        throw std::logic_error("Received malformed JSON: no result field");
}

long long JsonResponseParser::getBuyRequestResult() const
{
    if (receivedJson.contains("result"))
    {
        return std::stol(receivedJson["result"]["orderId"].dump());
    }
    else
        throw std::logic_error("Received malformed JSON: no result field");
}

std::optional<double> JsonResponseParser::getOrderQueryResult() const
{
    if (receivedJson.contains("result"))
    {
        const auto statusStr = receivedJson["result"]["status"].get<std::string>();
        if (statusStr == "FILLED")
        {
            return std::optional(std::stod(receivedJson["result"]["executedQty"].get<std::string>()));
        }
        else if (statusStr == "NEW" || statusStr == "PARTIALLY_FILLED")
        {
            return std::nullopt;
        }
        else 
        {
            throw std::logic_error("Issue with an order, order status is " + receivedJson["result"]["status"].dump());
        }
    }
    else
        throw std::logic_error("Received malformed JSON: no result field");
}
}
