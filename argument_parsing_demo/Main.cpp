#include <iostream>
#include <fstream>
#include <istream>
#include <string>

#include "ProgramOptions.hpp"

int wmain(int argc, wchar_t* argv[])
{
    arg_parsing_demo::ArgsPOD arguments;

    try
    {
        arg_parsing_demo::ProgramArgs pa;
        auto optArguments = pa.Process(argc, argv);

        if (optArguments.has_value())
        {
            arguments = optArguments.value();
        }
        else
        {
            return 0;
        }
    }
    catch (...)
    {
        std::cout << "Incorrect commandline arguments provided. use \""
                  << argv[0] << " --help\" for usage info." << std::endl;

        return 1;
    }

    // We are all set to continue to business logic
}
