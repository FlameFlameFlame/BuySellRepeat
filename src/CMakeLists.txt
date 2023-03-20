add_subdirectory(thirdparty)

add_executable(fintech-test fintech-test.cpp)
add_library(Accountant Accountant.cpp)
add_library(Trader Trader.cpp)
add_library(WebIO WebIO.cpp)
add_library(JsonResponseParser JsonResponseParser.cpp)
add_library(JsonQueryGenerator JsonQueryGenerator.cpp)


set_property(TARGET fintech-test Accountant Trader WebIO JsonResponseParser JsonQueryGenerator PROPERTY CXX_STANDARD 17)


include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(JsonQueryGenerator nlohmann_json::nlohmann_json)
target_link_libraries(JsonResponseParser nlohmann_json::nlohmann_json)
target_link_libraries(Trader ixwebsocket JsonResponseParser)
target_link_libraries(WebIO ixwebsocket JsonResponseParser)
target_link_libraries(fintech-test PRIVATE nlohmann_json::nlohmann_json ixwebsocket Trader Accountant WebIO JsonQueryGenerator JsonResponseParser ixwebsocket)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
