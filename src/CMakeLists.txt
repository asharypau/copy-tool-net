add_executable(
    CopyToolNet
    CopyToolNet.cpp
    common/ApplicationBuilder.cpp
    common/TcpClient.cpp
    utils/CmdArgsParser.cpp
    server/Server.cpp
    server/Session.cpp
    server/FileHandler.cpp
    client/ClientApplication.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio)
