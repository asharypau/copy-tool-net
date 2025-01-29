add_executable(
    CopyToolNet
    CopyToolNet.cpp
    utils/ApplicationBuilder.cpp
    utils/CmdArgsParser.cpp
    server/ServerApplication.cpp
    client/ClientApplication.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio)
