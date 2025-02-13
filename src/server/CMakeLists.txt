add_library(
    Server STATIC
    Startup.cpp
    Session.cpp
    MessageReader.cpp
    FileHandler.cpp
)

target_link_libraries(Server PRIVATE Boost::asio)

target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
