add_executable(
    CopyToolNet
    main.cpp
    common/ApplicationBuilder.cpp
    utils/CmdArgsParser.cpp
    server/Server.cpp
    server/Session.cpp
    server/FileHandler.cpp
    client/Client.cpp
    client/TcpClient.cpp
    client/FileReader.cpp
    client/FilePathPrompt.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio)
