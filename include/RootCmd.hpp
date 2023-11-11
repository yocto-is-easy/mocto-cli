#pragma once

#include "SubCmd.hpp"

#include "PluginCmd.hpp"
#include "FunctionCmd.hpp"

class RootCmd : public SubCmd {
private:

public:
    RootCmd() {
        add_subcommand("plugin", std::make_shared<PluginCmd>());
        add_subcommand("function", std::make_shared<FunctionCmd>());
    }

    void cmd() override {}

    std::string get_cmd_description() override {
        return "Root command";
    }
};
