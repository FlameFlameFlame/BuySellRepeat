#include "JsonResponseParser.h"

namespace BuySellRepeat_NS
{

int JsonResponseParser::getResponseStatus() const
{
    if (receivedJson.contains("status"))
        return receivedJson.at("status");
    else
        throw std::logic_error("Received malformed JSON: no status field");
}

std::pair<std::string, double> JsonResponseParser::getPriceFromTicker() const
{
    if (receivedJson.contains("result"))
        return receivedJson.at("result");
    else
        throw std::logic_error("Received malformed JSON: no result field");
}
}
