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
        resultJson["params"]["signature"] = signature.value();
        resultJson["params"]["timestamp"] = std::any_cast<std::time_t>(arguements[0]);
    }
    else if (qt == QueryType::SERVER_TIME)
    {
        resultJson["method"] = "time";
    }

    // implement other methods later
}
void JsonQueryGenerator::CalculateSignature()
{
    using json = nlohmann::json;
    std::string paramsString {};
    if (qt == QueryType::BUY)
    {

    }
    else if (qt == QueryType::SELL)
    {

    }
    else if (qt == QueryType::USER_DATA)
    {
        paramsString += "apiKey="+apiKey+"&";
        paramsString += "timestamp="+std::to_string(std::any_cast<std::time_t>(arguements[0]));
    }
    std::vector<uint8_t> signatureVectorChars(32);
    hmac_sha256(secretKey.data(), secretKey.size(), paramsString.data(), paramsString.size(), signatureVectorChars.data(), signatureVectorChars.size());
    std::stringstream ss;
    for (uint8_t x : signatureVectorChars) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)x;
    }
    signature = std::optional(ss.str());
}
}