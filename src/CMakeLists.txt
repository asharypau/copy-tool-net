add_subdirectory(server)
add_subdirectory(client)

add_executable(
    CopyToolNet
    main.cpp
    infrastructure/ApplicationBuilder.cpp
    common/services/FileService.cpp
    common/network/tcp/Socket.cpp
    common/network/tcp/Acceptor.cpp
    common/network/tcp/Connector.cpp
    common/encryption/Rsa.cpp
    common/encryption/Aes.cpp
    utils/CmdArgsParser.cpp
)

target_link_libraries(
    CopyToolNet PRIVATE
    Boost::system
    Boost::asio
    Boost::beast
    OpenSSL::Crypto
    OpenSSL::SSL
    Server
    Client
)

target_include_directories(
    CopyToolNet PRIVATE
    ${Boost_INCLUDE_DIRS}
    Server
    Client
)
