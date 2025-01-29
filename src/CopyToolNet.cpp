#include <thread>

#include "common/ApplicationType.h"
#include "common/CmdArgs.h"
#include "utils/ApplicationBuilder.h"
#include "utils/CmdArgsParser.h"

int main(int argc, char* argv[])
{
    CmdArgsParser parser;
    ApplicationBuilder builder;

    CmdArgs cmd_args = parser.parse(argc, argv);
    std::unique_ptr<IApplication> app = builder.build(cmd_args.app_type);
    app->run(cmd_args);

    // std::unique_ptr<IApplication> application1 = builder.build(ApplicationType::SERVER);
    // std::unique_ptr<IApplication> application2 = builder.build(ApplicationType::CLIENT);

    // std::thread server_thread([&]
    //                           { application1->run(cmd_args); });
    // std::thread client_thread([&]
    //                           { application2->run(cmd_args); });

    // server_thread.join();
    // client_thread.join();

    return 0;
}
