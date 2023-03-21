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
        resultJson["params"]["apiKey"] = apiKey;
        resultJson["params"]["timestamp"] = std::any_cast<std::time_t>(arguements[0]);

        const auto paramsString = GetParamsStringToSign(resultJson["params"]);
        resultJson["params"]["signature"] = CalculateSignature(paramsString);

    }
    else if (qt == QueryType::SERVER_TIME)
    {
        resultJson["method"] = "time";
    }

    // implement other methods later
}

std::string JsonQueryGenerator::CalculateSignature(const std::string& paramsString)
{
    std::vector<uint8_t> signatureVectorChars(32);
    hmac_sha256(secretKey.data(), secretKey.size(), paramsString.data(), paramsString.size(), signatureVectorChars.data(), signatureVectorChars.size());
    std::stringstream ss;
    for (uint8_t x : signatureVectorChars) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)x;
    }
    return ss.str();
}

std::string JsonQueryGenerator::GetParamsStringToSign(const nlohmann::json &paramsSection)
{
    std::string paramsString {};
        for (const auto& [param, value] : resultJson["params"].items())
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
            paramsString += param + "=" + valueStr + "&";
        }
        paramsString.pop_back(); // erase trailing '&'
        return paramsString;
}
}