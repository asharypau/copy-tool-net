add_subdirectory(server)
add_subdirectory(client)

add_executable(
    CopyToolNet
    main.cpp
    common/ApplicationBuilder.cpp
    common/Tcp.cpp
    common/FileService.cpp
    utils/CmdArgsParser.cpp
)

target_link_libraries(CopyToolNet PRIVATE Boost::asio Server Client)

target_include_directories(CopyToolNet PRIVATE Server Client)

