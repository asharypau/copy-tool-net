add_library(
    Client STATIC
    Startup.cpp
    MessagesPrompt.cpp
    MessagesQueueHandler.cpp
    FileClient.cpp
)

target_link_libraries(Client PRIVATE Boost::asio)

target_include_directories(Client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
