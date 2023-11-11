#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

namespace po = boost::program_options;

class SubCmd {
    std::map<std::string, std::shared_ptr<SubCmd>> subcommands;
    po::options_description opt_desc;
    std::vector<po::option> m_options;

public:
    SubCmd() {
        opt_desc.add_options()
            ("help", "Print help message")
        ;
    }

    virtual ~SubCmd() = default;

    virtual void add_subcommand(const std::string& key, std::shared_ptr<SubCmd> subcommand) {
        subcommands[key] = subcommand;
    }

    virtual void proceed(int argc, char** argv) {
        std::vector<std::string> args(argv + 1, argv + argc);
        if (args.size() > 0) {
            if(args[0] == "?") {
                help();
                return;
            }

            auto subcommand = subcommands.find(args[0]);
            if (subcommand != subcommands.end()) {
                subcommand->second->proceed(argc - 1, argv + 1);
            } else {
                cmd_proceeder(argc, argv);
            }
        } else {
            cmd_proceeder(argc, argv);
        }
    }

    virtual void help() {
        if(get_cmd_description() != ""){
            std::cout << "Description: " 
                      << get_cmd_description()
                      << std::endl;
        }

        if(subcommands.size() > 0) {
            std::cout << "Subcommands:" << std::endl;
            for (auto& subcommand : subcommands) {
                std::cout << "\t" << subcommand.first << std::endl;
            }
        }

        std::cout << "Parameters:" << std::endl;
        std::cout << opt_desc << std::endl;
    }

    virtual po::options_description& get_description() {
        return opt_desc;
    }

    virtual std::vector<po::option> get_options() {
        return m_options;
    }

    virtual void cmd_proceeder(int argc, char** argv) {
        po::variables_map vm;
        try{
            po::parsed_options parsed = po::parse_command_line(argc, argv, opt_desc);
            m_options = parsed.options;
            po::store(parsed, vm);
            po::notify(vm);
        } catch(std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Use --help or '?' to get help" << std::endl;
            return;
        }

        if (vm.count("help")) {
            help();
            return;
        }

        cmd();
    }

    virtual void cmd() = 0;
    virtual std::string get_cmd_description() = 0;
};
