add_library(
    Server STATIC
    Startup.cpp
    Session.cpp
    Dispatcher.cpp
    controllers/FileController.cpp
)

target_link_libraries(
    Server PRIVATE
    Boost::system
    Boost::asio
    Boost::beast
)

target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
