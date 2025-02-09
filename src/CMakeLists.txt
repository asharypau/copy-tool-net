add_executable(
    CopyToolNet
    main.cpp
    common/ApplicationBuilder.cpp
    common/Tcp.cpp
    utils/CmdArgsParser.cpp
    server/Server.cpp
    server/Session.cpp
    server/FileHandler.cpp
    client/Client.cpp
    client/FileReader.cpp
    client/MessagesPrompt.cpp
    client/MessageQueueHandler.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio)
