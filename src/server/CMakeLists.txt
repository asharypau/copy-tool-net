add_library(
    Server STATIC
    Startup.cpp
    SessionManager.cpp
    Session.cpp
    Dispatcher.cpp
    controllers/FileController.cpp
    services/FileStorage.cpp
)

target_link_libraries(
    Server PRIVATE
    Boost::system
    Boost::asio
    Boost::beast
    OpenSSL::Crypto
    OpenSSL::SSL
)

target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
