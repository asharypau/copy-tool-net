#include <boost/asio.hpp>
#include <iostream>

#include "../common/network/tcp/OperationException.h"
#include "infrastructure/ApplicationBuilder.h"
#include "utils/CmdArgs.h"
#include "utils/CmdArgsParser.h"

boost::asio::awaitable<void> run2()
{
    throw Tcp::OperationException(1, "OperationException");
}

boost::asio::awaitable<void> run1()
{
    while (true)
    {
        try
        {
            co_await run2();
        }
        catch (const Tcp::OperationException& ex)
        {
            std::cout << "Error: " << ex.what() << std::endl;

            // ex data is not available without this break
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    boost::asio::io_context context;
    boost::asio::co_spawn(context, run1(), boost::asio::detached);
    context.run();

    return 0;

    CmdArgsParser parser;
    ApplicationBuilder builder;

    CmdArgs args = parser.parse(argc, argv);
    std::unique_ptr<IApplication> app = builder.build(args);
    app->run();

    return 0;
}
