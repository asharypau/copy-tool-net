add_library(
    Common STATIC
    services/FileService.cpp
    network/tcp/Socket.cpp
    network/tcp/Acceptor.cpp
    network/tcp/Connector.cpp
    encryption/Rsa.cpp
    encryption/Aes.cpp
)

target_link_libraries(
    Common PUBLIC
    Boost::system
    Boost::asio
    Boost::beast
    OpenSSL::Crypto
    OpenSSL::SSL
)

target_include_directories(
    Common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)
