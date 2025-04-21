add_library(
    Server STATIC
    Startup.cpp
    SessionManager.cpp
    Session.cpp
    Dispatcher.cpp
    controllers/FileController.cpp
    services/FileStorage.cpp
    services/WorkersPool.cpp
)

target_link_libraries(
    Server PRIVATE
    Common
    Boost::system
    Boost::asio
)

target_include_directories(
    Server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
