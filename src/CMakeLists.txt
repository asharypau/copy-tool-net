add_subdirectory(server)
add_subdirectory(client)

add_executable(
    CopyToolNet
    main.cpp
    infrastructure/ApplicationBuilder.cpp
    common/services/FileService.cpp
    utils/CmdArgsParser.cpp
)

target_link_libraries(
    CopyToolNet PRIVATE
    Boost::system
    Boost::asio
    Boost::beast
    Server
    Client
)

target_include_directories(CopyToolNet PRIVATE Server Client)
