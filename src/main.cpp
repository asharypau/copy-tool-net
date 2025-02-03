#include "common/ApplicationBuilder.h"
#include "models/CmdArgs.h"
#include "utils/CmdArgsParser.h"

int main(int argc, char* argv[])
{
    CmdArgsParser parser;
    ApplicationBuilder builder;

    CmdArgs cmd_args = parser.parse(argc, argv);
    std::unique_ptr<IApplication> app = builder.build(cmd_args);
    app->run();

    return 0;
}
