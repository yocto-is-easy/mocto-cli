#pragma once

#include <vector>
#include <string>

#include "SubCmd.hpp"

#include "i-plugin-manager.hpp"

class FunctionInfoCmd : public SubCmd {
private:
    std::string m_plugin_name;
    std::string m_function_name;

public:
    FunctionInfoCmd() {
        this->get_description().add_options()
            ("plugin", po::value<std::string>(&m_plugin_name)->required(), "name of the plugin")
            ("function", po::value<std::string>(&m_function_name)->required(), "name of the function to get info on")
        ;
    }

    void cmd() override {
        if(plugin_manager::function_exists(m_plugin_name, m_function_name)) {
            auto func_info = plugin_manager::get_function_info(m_plugin_name, m_function_name);
            std::cout << func_info.get().dump(4) << std::endl;
        } else {
            std::cout << "Function " << m_function_name << " does not exist" << std::endl;
        }
    }

    std::string get_cmd_description() override {
        return "function info - gets info about a function";
    }
};

class FunctionCallCmd : public SubCmd {
private:
    std::vector<std::vector<std::string>> m_params;
    std::string m_plugin_name;
    std::string m_function_name;

    bool verify_params() {
        for(int i = 0; i < m_params.size(); i++) {
            auto& param = m_params[i];
            if(param.size() != 2) {
                std::cout << "Invalid parameter: " << "\"--param ";
                for(int j = 0; j < param.size(); j++) {
                    std::cout << param[j];
                    if(j != param.size() - 1) {
                        std::cout << " ";
                    }
                }
                std::cout << "\"" << std::endl;

                std::cout << "Required format: --param <key> <value>" << std::endl;
                return false;
            }
        }
        return true;
    }

    void parse_params() {
        for (const po::option& opt : this->get_options()) {
            if (opt.string_key == "param") {
                m_params.push_back(opt.value);
            }
        }
    }

public:
    FunctionCallCmd() {
        this->get_description().add_options()
            ("plugin", po::value<std::string>(&m_plugin_name)->required(), "name of the plugin")
            ("function", po::value<std::string>(&m_function_name)->required(), "name of the function to get info on")
            ("param", po::value<std::vector<std::string>>()->multitoken(), "function params")
        ;
    }

    void cmd() override {
        parse_params();

        if(!verify_params()) {
            return;
        }

        midf::JsonAsVar func_info = plugin_manager::get_function_info(m_plugin_name, m_function_name);
        if(func_info.get()["arguments"].size() != m_params.size()) {
            std::cout << "Invalid number of arguments" << std::endl;
            return;
        }

        // verify each param name
        for(auto& param : m_params) {
            bool found = false;
            for(auto& arg : func_info.get()["arguments"]) {
                if(arg["name"] == param[0]) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                std::cout << "Invalid argument name: " << param[0] << std::endl;
                // print valid parameter names and types
                std::cout << "Valid arguments:" << std::endl;
                for(auto& arg : func_info.get()["arguments"]) {
                    std::string name = arg["name"];
                    std::string type = arg["type"];
                    std::string description;
                    if(arg.contains("description")) {
                        description = arg["description"];
                    }

                    std::cout << "\t" << name << " : " << type << " - " << description << std::endl;
                }
                return;
            }
        }

        midf::JsonAsParam input;
        for(auto& param : m_params) {
            input.get()[param[0]] = param[1];
        }

        auto func = plugin_manager::get_function(m_plugin_name, m_function_name);
        auto output = func(input);

        std::cout << output.get().dump(4) << std::endl;
    }

    std::string get_cmd_description() override {
        return "function call - calls a function";
    }
};

class FunctionCmd : public SubCmd {
public:
    FunctionCmd() {
        add_subcommand("info", std::make_shared<FunctionInfoCmd>());
        add_subcommand("call", std::make_shared<FunctionCallCmd>());
    }

    void cmd() override {}

    std::string get_cmd_description() override {
        return "Function command";
    }
};
