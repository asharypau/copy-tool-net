add_library(
    Client STATIC
    Startup.cpp
    MessagesPrompt.cpp
    MessagesQueueHandler.cpp
    FileClient.cpp
)

target_link_libraries(
    Client PRIVATE
    Boost::system
    Boost::asio
    Boost::beast
    OpenSSL::Crypto
    OpenSSL::SSL
)

target_include_directories(Client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
