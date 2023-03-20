#pragma once

#include "CommonTypes.h"
#include <nlohmann/json.hpp>

#include <any>


namespace BuySellRepeat_NS
{

class JsonQueryGenerator
{
    using json = nlohmann::json;
    private:
        int id = 0;
        std::vector<std::any> arguements;
        QueryType qt;
        json resultJson;
        void GenerateJson();
    public:
        JsonQueryGenerator(const int& id, const QueryType& queryType):
        id(id), qt(queryType)
        {
        };
        ~JsonQueryGenerator() = default;

        void SetArguements(const std::vector<std::any>&& args)
        {
            arguements = std::move(args);
            GenerateJson();
        }
        json GetJson() const
        {
            return resultJson;
        }
        
        
};
}