#pragma once

#include "SubCmd.hpp"

#include "PluginRegistratorCmd.hpp"

class RootCmd : public SubCmd {
private:

public:
    RootCmd() {
        add_subcommand("pr", std::make_shared<PluginRegistratorCmd>());
    }

    void cmd() override {}

    std::string get_cmd_description() override {
        return "Root command";
    }
};
