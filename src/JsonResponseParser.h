#pragma once

#include "CommonTypes.h"

#include <nlohmann/json.hpp>

namespace BuySellRepeat_NS
{

class JsonResponseParser
{
    using json = nlohmann::json;
    private:
        std::string rawString;
        json receivedJson;
    public:
        JsonResponseParser(const std::string& jsonString):
        rawString(jsonString)
        {
            using namespace nlohmann::literals;
            receivedJson = json::parse(jsonString);
        }
        int getResponseStatus() const;
        std::string getUserData() const;
        std::pair<std::string, double> getPriceFromTicker() const;
        time_t getServerTime() const;
        int getSellResult() const;
        int getBuyResult() const;
        ResponseType getResponceType() const;

};
}