#pragma once

#include <string>
#include <iostream>
#include <istream>
#include <optional>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace arg_parsing_demo
{
    enum class TernaryArg
    {
        Value_1,
        Value_2,
        Value_3
    };

    struct ArgsPOD
    {
        int intArg;
        TernaryArg enumArg;
        std::wstring strArg;
    };

    std::wistream& operator>>(std::wistream& in, TernaryArg& enumArg)
    {
        std::wstring token;
        in >> token;

        std::transform(token.begin(), token.end(), token.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (token == L"value_1")
            enumArg = TernaryArg::Value_1;
        else if (token == L"value_2")
            enumArg = TernaryArg::Value_2;
        else
            enumArg = TernaryArg::Value_3;

        return in;
    }

    struct ProgramArgs
    {
        std::optional<ArgsPOD> Process(int argc, wchar_t* argv[])
        {
            ArgsPOD args_;

            po::options_description desc("help");
            po::variables_map vm;

            desc.add_options()
                ("help,h",                                                     "Get help")
                ("intArg",  po::wvalue<int>(&args_.intArg)->default_value(42), "Specifies the integer argument")
                ("strArg",  po::wvalue<std::wstring>(&args_.strArg),           "Specifies the std::string argument")
                ("enumArg", po::wvalue<TernaryArg>(&args_.enumArg),            "Specifies the custom enum arg")
                ;

            store(parse_command_line(argc, argv, desc), vm);

            try
            {
                notify(vm);
            }
            catch (const po::required_option & e)
            {
                std::cout << "Error: " << e.what() << std::endl;
                throw std::invalid_argument("Arguments not correcly specified");
            }

            if (vm.count("help") > 0)
            {
                std::cout << desc << std::endl;
            }

            return { args_ };
        }
    };
}