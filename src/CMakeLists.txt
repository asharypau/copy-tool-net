add_executable(
    CopyToolNet
    main.cpp
    common/ApplicationBuilder.cpp
    common/TcpClient.cpp
    utils/CmdArgsParser.cpp
    server/Server.cpp
    server/ServerSession.cpp
    server/FileWriter.cpp
    client/Client.cpp
    client/FileReader.cpp
    client/FilePathPrompt.cpp
    client/ClientSession.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio)
