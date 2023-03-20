#include "WebIO.h"

#include "JsonQueryGenerator.h"
#include "JsonResponseParser.h"

#include <iostream>

namespace BuySellRepeat_NS

{
int WebIO::Init()
{
    ix::initNetSystem();
    ws.setUrl(url);
    ws.setOnMessageCallback(webSocketMsgCallback);
    ws.start();

    return 0;
}

void WebIO::HandleMessage(const ix::WebSocketMessagePtr &msg)
{
    std::cout << "Incoming message";
    std::unique_lock<std::mutex> lock(resultProtecter);
    JsonResponseParser jrp(msg->str);
    if (jrp.getResponseStatus() != 200) // TODO: add server responces enum
        throw std::logic_error("Incorrect responce from server: " + std::to_string(jrp.getResponseStatus()));

    lock.unlock();
    if (expectedResponse == ResponseType::TICKER)
        result = jrp.getPriceFromTicker();
    
    resultCv.notify_one();
    lock.lock();
}

double WebIO::getPrice(const std::string &symbols)
{
    std::unique_lock<std::mutex> lock(resultProtecter);
    expectedResponse = ResponseType::TICKER;

    JsonQueryGenerator jqg(requestId, QueryType::TICKER);
    jqg.SetArguements({symbols});
    const auto requestJson = jqg.GetJson();
    const auto str = requestJson.dump();
    std::cout << "sending " << str;
    ws.send(requestJson.dump());
    resultCv.wait(lock);
    const auto resultPair = std::any_cast<std::pair<std::string, double>>(result);
    if (resultPair.first == symbols)
        return resultPair.second;
    else
        throw std::logic_error("Incorrect symbols returned in getPrice");
}
}
