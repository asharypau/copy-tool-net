add_library(
    Server STATIC
    Startup.cpp
    SessionManager.cpp
    Dispatcher.cpp
    controllers/FileController.cpp
    services/FileStorage.cpp
)

target_link_libraries(
    Server PRIVATE
    Boost::system
    Boost::asio
    Boost::beast
)

target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
