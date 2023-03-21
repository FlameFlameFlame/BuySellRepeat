#pragma once

#include "CommonTypes.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <any>
#include <mutex>
#include <optional>

namespace BuySellRepeat_NS
{

class WebIO
{
private:
    std::string url;
    ix::WebSocket ws;
    bool isConnected = false;
    unsigned long long requestId = 0;
    const std::string apiKey;
    const std::string secretKey;

    std::condition_variable resultCv;    
    std::mutex resultProtecter;
    std::any result;
    ResponseType expectedResponse;

    std::function<void(const ix::WebSocketMessagePtr&)> webSocketMsgCallback = [&](const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Open)
        {
            isConnected = true;
            return;
        }
        if (msg->type == ix::WebSocketMessageType::Close)
        {
            isConnected = false;
            return;
        }
        if (msg->type == ix::WebSocketMessageType::Message)
        {
            HandleMessage(msg);
        }
        if (msg->type == ix::WebSocketMessageType::Error)
        {
            throw std::logic_error("Network error " + msg->errorInfo.reason);
        }
    };
    void HandleMessage(const ix::WebSocketMessagePtr& msg);

    std::string GeneratePriceRequest(const std::string &symbols) const;
    std::string GenerateUserDataRequest(const time_t& timestamp) const;

    void SendRequestAwaitResponse(const ResponseType& r, const std::string& requestStr);
public:
    WebIO(const std::string& endpoint, const std::string& apiKey, const std::string& secretKey):
    url(endpoint), apiKey(apiKey), secretKey(secretKey)
    {
        ix::initNetSystem();
        ws.setUrl(url);
        ws.setOnMessageCallback(webSocketMsgCallback);
        ws.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // TODO: investigate. sleeping so next messages won't get lost
    };
    ~WebIO() = default;
    double GetPrice(const std::string& symbols); 
    std::string GetUserData(const std::time_t& timestamp);
};
}