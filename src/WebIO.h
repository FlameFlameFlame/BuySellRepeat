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
    int Init();
    ix::WebSocket ws;
    bool isConnected = false;
    int requestId = 0;

    std::condition_variable resultCv;    
    std::mutex resultProtecter;
    std::any result;
    std::mutex responseTypeProtecter;
    ResponseType expectedResponse;

    void ConnectedCallback()
    {
        isConnected = true;
    }
    void DisconnectedCallback()
    {
        isConnected = false;
    }
    std::function<void(const ix::WebSocketMessagePtr&)> webSocketMsgCallback = [this](const ix::WebSocketMessagePtr& msg)
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

public:
    WebIO(const std::string& endpoint):
    url(endpoint)
    {
        Init();
    };
    ~WebIO() = default;
    double getPrice(const std::string& symbols); 
};
}