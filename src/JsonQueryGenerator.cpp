#include "JsonQueryGenerator.h"

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
    // implement other methods later
}
}