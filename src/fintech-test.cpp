#include "CommonTypes.h"
#include "Trader.h"

#include <argumentum/argparse.h>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <nlohmann/json.hpp>

#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>

int main(int argc, char** argv)
{
    using namespace BuySellRepeat_NS;
    using namespace argumentum;

    std::string apiKey;
    std::string secretKey;
    TradingParameters tradingParameters;

    auto parser = argument_parser{};
    parser.config().program(argv[0]);
    auto programParams = parser.params();
    parser.config().program(argv[0]).description("Buy, sell, repeat");
    programParams.add_parameter(apiKey, "--api_key").nargs(1).help("Your API key");
    programParams.add_parameter(secretKey, "--secret_key").nargs(1).help("Your secret key");
    auto pOptions = std::make_shared<TradingParameters>();
    programParams.add_parameters(pOptions);
    if (!parser.parse_args(argc, argv, 1))
        throw std::logic_error("Incorrect arguements set");
    
    auto currentTime = Trader::GetCurrentTimestampSeconds();
    std::stringstream ss;
    ss << "prices " << tradingParameters.tradingCurrency << tradingParameters.myCurrency << std::put_time(std::localtime(&currentTime), "%F %T") << ".csv";
    std::fstream csvPricesFile{ss.str(), csvPricesFile.trunc | csvPricesFile.out};

    WebIO wio("wss://testnet.binance.vision/ws-api/v3", apiKey, secretKey);
    Display disp(std::cout, csvPricesFile);
    Trader tr(*pOptions, wio, disp);
    tr.StartTrading();

    return 0;
}