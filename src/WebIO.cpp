#include "WebIO.h"

#include "JsonQueryGenerator.h"
#include "JsonResponseParser.h"

namespace BuySellRepeat_NS

{

void WebIO::HandleMessage(const ix::WebSocketMessagePtr &msg)
{
    JsonResponseParser jrp(msg->str);
    // if (jrp.getResponseStatus() != 200) // TODO: add server responces enum
    //     throw std::logic_error("Incorrect responce from server: " + std::to_string(jrp.getResponseStatus()));

    if (expectedResponse == ResponseType::TICKER)
        result = jrp.getPriceFromTicker();
    else if (expectedResponse == ResponseType::USER_DATA)
        result = jrp.getUserData();
    else if (expectedResponse == ResponseType::SERVER_TIME)
        result = jrp.getServerTime();
    else if (expectedResponse == ResponseType::BUY_REQUEST_ACK)
        result = jrp.getBuyRequestResult();

    resultCv.notify_one();

}

void WebIO::SendRequestAwaitResponse(const ResponseType& r, const std::string& requestStr)
{
    using namespace std::chrono_literals;
    std::unique_lock<std::mutex> lock(resultProtecter);
    ws.send(requestStr);
    while (resultCv.wait_for(lock, 50ms) == std::cv_status::timeout); // TODO: parametrise waiting time
    ++requestId;
}

std::string WebIO::GeneratePriceRequest(const std::string &symbols) const
{
    JsonQueryGenerator jqg(requestId, QueryType::TICKER, {symbols});
    const auto requestJson = jqg.GetJson();
    return requestJson.dump();
}

std::string WebIO::GenerateUserDataRequest(const time_t &timestamp) const
{
    JsonQueryGenerator jqg(requestId, QueryType::USER_DATA, apiKey, secretKey, {timestamp});
    const auto reqstr = jqg.GetJson().dump();
    return reqstr;
}

std::string WebIO::GenerateBuyRequest(const std::string &symbols, const double &qty, const double &price, const std::time_t timestamp)
{
    JsonQueryGenerator jqg(requestId, QueryType::BUY_REQUEST, apiKey, secretKey, {symbols, qty, price, timestamp});
    const auto reqstr = jqg.GetJson().dump();
    return reqstr;
}

std::string WebIO::GenerateServerTimeRequest() const
{
    JsonQueryGenerator jqg(requestId, QueryType::SERVER_TIME, {});
    const auto reqstr = jqg.GetJson().dump();
    return reqstr;
}

double WebIO::GetPrice(const std::string &symbols)
{
    expectedResponse = ResponseType::TICKER;
    SendRequestAwaitResponse(ResponseType::TICKER, GeneratePriceRequest(symbols));
    const auto resultPair = std::any_cast<std::pair<std::string, double>>(result);
    result.reset();
    if (resultPair.first == symbols)
        return resultPair.second;
    else
        throw std::logic_error("Incorrect symbols returned in GetPrice");
}

std::string WebIO::GetUserData(const std::time_t &timestamp)
{
    expectedResponse = ResponseType::USER_DATA;
    SendRequestAwaitResponse(ResponseType::USER_DATA, GenerateUserDataRequest(timestamp));
    const auto retVal = std::any_cast<std::string>(result);
    result.reset();
    return retVal;
}

std::time_t WebIO::GetServerTime()
{
    expectedResponse = ResponseType::SERVER_TIME;
    SendRequestAwaitResponse(ResponseType::SERVER_TIME, GenerateServerTimeRequest());
    const auto retVal = std::any_cast<std::time_t>(result);
    result.reset();
    return retVal;
}

std::string WebIO::SendBuyRequest(const std::string &symbols, const double &qty, const double &price, const std::time_t timestamp)
{
    expectedResponse = ResponseType::BUY_REQUEST_ACK;
    SendRequestAwaitResponse(ResponseType::SERVER_TIME, GenerateBuyRequest(symbols, qty, price, timestamp));
    const auto retVal = std::any_cast<std::string>(result);
    result.reset();
    return retVal;
}

}
