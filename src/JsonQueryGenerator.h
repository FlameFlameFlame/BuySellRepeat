#pragma once

#include "CommonTypes.h"
#include <nlohmann/json.hpp>

#include <any>


namespace BuySellRepeat_NS
{

class JsonQueryGenerator
{
    enum class BuyOrSell
    {
        BUY,
        SELL
    };

    private:
        int id = 0;
        std::vector<std::any> arguements;
        QueryType qt;
        nlohmann::json resultJson;
        std::string apiKey;
        std::string secretKey;

        // manually construct the 'HTTP-like' request: https://developers.binance.com/docs/binance-trading-api/websocket_api#signed-request-example-hmac
        std::string CalculateSignature(const std::string& paramsString) const;
        static std::string GetParamsStringToSign(const nlohmann::json& paramsSection);
        void GenerateJson();

        // those requests are similar so I've separated them into function
        void GenerateBuyOrSellRequest(const BuyOrSell& action);
    public:
        JsonQueryGenerator(const int& id, const QueryType& queryType, const std::vector<std::any>&& args):
        id(id), qt(queryType), arguements(std::move(args))
        {
            GenerateJson();
        };
        JsonQueryGenerator(const int& id, const QueryType& queryType, const std::string& apiKey, const std::string& secretKey, const std::vector<std::any>&& args):
        id(id), qt(queryType), apiKey(apiKey), secretKey(secretKey), arguements(std::move(args))
        {
            GenerateJson();
        };
        ~JsonQueryGenerator() = default;

        nlohmann::json GetJson() const
        {
            return resultJson;
        }
};
}