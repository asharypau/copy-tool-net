add_executable(
    CopyToolNet
    main.cpp
    common/ApplicationBuilder.cpp
    common/TcpClient.cpp
    utils/CmdArgsParser.cpp
    server/Server.cpp
    server/Session.cpp
    server/FileWriter.cpp
    client/Client.cpp
    client/FileReader.cpp
    client/FilePathPrompt.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio)
