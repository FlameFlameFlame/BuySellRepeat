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
    int requestId = 0;

    std::condition_variable resultCv;    
    std::mutex resultProtecter;
    std::any result;
    bool hasNewMessage = false;
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


public:
    WebIO(const std::string& endpoint):
    url(endpoint)
    {
        ix::initNetSystem();
        ws.setUrl(url);
        ws.setOnMessageCallback(webSocketMsgCallback);
        ws.start();
    };
    ~WebIO() = default;
    double getPrice(const std::string& symbols); 
};
}