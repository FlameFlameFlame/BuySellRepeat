#include "JsonQueryGenerator.h"
#include <hmac_sha256.h>

#include <sstream>

namespace BuySellRepeat_NS
{

void JsonQueryGenerator::GenerateJson()
{        
    resultJson["id"] = id;
    if (qt == QueryType::PING)
    {
        return;
    }
    else if (qt == QueryType::TICKER)
    {
        resultJson["method"] = "ticker.price";
        resultJson["params"]["symbol"] = std::any_cast<std::string>(arguements[0]);
    }
    else if (qt == QueryType::USER_DATA)
    {
        resultJson["method"] = "account.status";
        resultJson["params"]["timestamp"] = std::any_cast<std::time_t>(arguements[0]);
        resultJson["params"]["apiKey"] = apiKey;
        

        const auto paramsString = GetParamsStringToSign(resultJson["params"]);
        resultJson["params"]["signature"] = CalculateSignature(paramsString);
    }
    else if (qt == QueryType::SERVER_TIME)
    {
        resultJson["method"] = "time";
    }
    else if (qt == QueryType::BUY_REQUEST)
    {
        GenerateBuyOrSellRequest(BuyOrSell::BUY);
    }
    else if (qt == QueryType::SELL_REQUEST)
    {
        GenerateBuyOrSellRequest(BuyOrSell::SELL);
    }
    else if (qt == QueryType::ORDER_QUERY)
    {
        resultJson["method"] = "order.status";
        resultJson["params"]["symbol"] = std::any_cast<std::string>(arguements[0]);
        resultJson["params"]["timestamp"] = std::any_cast<time_t>(arguements[2]);
        resultJson["params"]["orderId"] = std::to_string(std::any_cast<long long>(arguements[1]));
        resultJson["params"]["apiKey"] = apiKey;

        const auto paramsString = GetParamsStringToSign(resultJson["params"]);
        resultJson["params"]["signature"] = CalculateSignature(paramsString);;
    }
}

void JsonQueryGenerator::GenerateBuyOrSellRequest(const BuyOrSell& action)
{
        resultJson["method"] = "order.place";
        resultJson["params"]["apiKey"] = apiKey;
        resultJson["params"]["symbol"] = std::any_cast<std::string>(arguements[0]);
        resultJson["params"]["side"] = (action == BuyOrSell::BUY) ? "BUY" : "SELL";
        resultJson["params"]["type"] = "LIMIT";
        resultJson["params"]["timeInForce"] = "GTC";
        resultJson["params"]["price"] = std::to_string(std::any_cast<double>(arguements[2]));
        resultJson["params"]["quantity"] = std::to_string(std::any_cast<double>(arguements[1]));
        resultJson["params"]["timestamp"] = std::any_cast<time_t>(arguements[3]);

        const auto paramsString = GetParamsStringToSign(resultJson["params"]);
        resultJson["params"]["signature"] = CalculateSignature(paramsString);
}

std::string JsonQueryGenerator::CalculateSignature(const std::string& paramsString) const
{
    std::vector<uint8_t> signatureVectorChars(32);
    hmac_sha256(secretKey.data(), secretKey.size(), paramsString.data(), paramsString.size(), signatureVectorChars.data(), signatureVectorChars.size());
    std::stringstream ss;
    for (uint8_t x : signatureVectorChars) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)x;
    }
    return ss.str();
}

std::string JsonQueryGenerator::GetParamsStringToSign(const nlohmann::json& paramsSection)
{
    std::map<std::string, std::string> resMap;
    for (const auto& [param, value] : paramsSection.items())
    {
        std::string valueStr;
        if (value.is_string())
            value.get_to(valueStr);
        else if (value.is_number_integer()) 
        {
            long long tempNumber;
            value.get_to(tempNumber);
            valueStr = std::to_string(tempNumber);
        }
        else if (value.is_number_float())
        {
            double tempNumber;
            value.get_to(tempNumber);
            valueStr = std::to_string(tempNumber);
        } 
        resMap[param] = valueStr;
    }
    std::string paramsString {};
        for (const auto& [param, value] : resMap)
        {   
            
            paramsString += param + "=" + value + "&";
        }
        paramsString.pop_back(); // erase trailing '&'
        return paramsString;
}
}