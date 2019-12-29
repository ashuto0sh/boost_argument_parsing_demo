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

        int add;
        int like;
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

    template <typename T>
    bool CompareStringInsensitive(const std::basic_string<T>& l, const std::basic_string<T>& r)
    {
        std::basic_string<T> l_lower, r_lower;
        std::transform(begin(l), end(l), back_inserter(l_lower), ::tolower);
        std::transform(begin(r), end(r), back_inserter(r_lower), ::tolower);

        return l_lower == r_lower;
    }

    struct ProgramArgs
    {
        std::optional<ArgsPOD> Process(int argc, wchar_t* argv[])
        {
            ArgsPOD args_;

            po::options_description desc("help");
            po::variables_map vm;

            desc.add_options()
                ("help,h",                                                         "Get help")
                ("intArg",      po::wvalue<int>(&args_.intArg)->default_value(42), "Specifies the integer argument")
                ("strArg",      po::wvalue<std::wstring>(&args_.strArg),           "Specifies the std::string argument")
                ("enumArg",     po::wvalue<TernaryArg>(&args_.enumArg),            "Specifies the custom enum arg")
                ("positional",  po::wvalue<std::vector<std::wstring> >())
                ;
            po::positional_options_description positionalOptions;
            positionalOptions.add("positional", 2);

            try
            {
                po::store(po::wcommand_line_parser(argc, argv)
                            .options(desc)
                            .positional(positionalOptions).run(),
                          vm); // throws on error
                notify(vm);
            }
            catch (const po::required_option & e)
            {
                std::cout << "Error: " << e.what() << std::endl;
                throw std::invalid_argument("Arguments not correcly specified");
            }
            catch (boost::program_options::error & e)
            {
                std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            }

            if (vm.count("help") > 0)
            {
                std::cout << desc << std::endl;

                return {};
            }

            if (vm.count("positional"))
            {
                for (auto pos_param : vm["positional"].as<std::vector<std::wstring>>())
                {
                    std::wcout << L"positional: " << pos_param << std::endl;
                }
            }

            return { args_ };
        }
    };
}
