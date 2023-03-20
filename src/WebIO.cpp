#include "WebIO.h"

#include "JsonQueryGenerator.h"
#include "JsonResponseParser.h"

#include <iostream>

namespace BuySellRepeat_NS

{

void WebIO::HandleMessage(const ix::WebSocketMessagePtr &msg)
{
    hasNewMessage = true;
    JsonResponseParser jrp(msg->str);
    if (jrp.getResponseStatus() != 200) // TODO: add server responces enum
        throw std::logic_error("Incorrect responce from server: " + std::to_string(jrp.getResponseStatus()));

    if (expectedResponse == ResponseType::TICKER)
        result = jrp.getPriceFromTicker();
    resultCv.notify_one();
}

std::string WebIO::GeneratePriceRequest(const std::string &symbols) const
{
    JsonQueryGenerator jqg(requestId, QueryType::TICKER);
    jqg.SetArguements({symbols});
    const auto requestJson = jqg.GetJson();
    return requestJson.dump();
}

double WebIO::getPrice(const std::string &symbols)
{
    using namespace std::chrono_literals;

    std::unique_lock<std::mutex> lock(resultProtecter);
    expectedResponse = ResponseType::TICKER;

    const std::string requestStr = GeneratePriceRequest(symbols);
    while (resultCv.wait_for(lock, 300ms) == std::cv_status::timeout && !hasNewMessage)
    {
        ws.send(requestStr);
    }
    hasNewMessage = false;
    const auto resultPair = std::any_cast<std::pair<std::string, double>>(result);
    result.reset();
    if (resultPair.first == symbols)
        return resultPair.second;
    else
        throw std::logic_error("Incorrect symbols returned in getPrice");
}
}
