add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

add_executable(
    CopyToolNet
    main.cpp
    infrastructure/ApplicationBuilder.cpp
    utils/CmdArgsParser.cpp
)

target_link_libraries(
    CopyToolNet PRIVATE
    Common
    Server
    Client
)

target_include_directories(
    CopyToolNet PRIVATE
    Common
    Server
    Client
)
