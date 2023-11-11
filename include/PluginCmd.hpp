#pragma once

#include <vector>
#include <string>

#include "SubCmd.hpp"

#include "i-plugin-manager.hpp"

class PluginActionGeneralCmd : public SubCmd {
private:
    std::string m_plugin_name;

public:
    PluginActionGeneralCmd() {
        this->get_description().add_options()
            ("name", po::value<std::string>(&m_plugin_name)->required(), "name of the plugin to act on")
        ;
    }

    virtual std::string get_plugin_name() {
        return m_plugin_name;
    }
};

class PluginRegisterCmd : public PluginActionGeneralCmd {
public:
    PluginRegisterCmd() = default;

    void cmd() override {
        bool res = plugin_manager::register_plugin(get_plugin_name());
        if (!res) {
            std::cout << "Failed to register plugin " << get_plugin_name() << std::endl;
        } else {
            std::cout << "Plugin " << get_plugin_name() << " registered" << std::endl;
        }
    }

    std::string get_cmd_description() override {
        return "PluginRegister sub-command - registers a plugin";
    }
};

class PluginUnregisterCmd : public PluginActionGeneralCmd {
public:
    PluginUnregisterCmd() = default;

    void cmd() override {
        bool res = plugin_manager::unregister_plugin(get_plugin_name());
        if (!res) {
            std::cout << "Failed to unregister plugin " << get_plugin_name() << std::endl;
        } else {
            std::cout << "Plugin " << get_plugin_name() << " unregistered" << std::endl;
        }
    }

    std::string get_cmd_description() override {
        return "PluginUnregister sub-command - unregisters a plugin";
    }
};

class PluginInfoCmd : public PluginActionGeneralCmd {
public:
    PluginInfoCmd() = default;

    void cmd() override {
        midf::JsonAsRet info = plugin_manager::get_plugin_info(get_plugin_name());
        std::cout << info.get().dump(4) << std::endl;
    }

    std::string get_cmd_description() override {
        return "PluginInfo sub-command - gets info about a plugin";
    }
};

class PluginCmd : public SubCmd {
public:
    PluginCmd() {
        add_subcommand("register", std::make_shared<PluginRegisterCmd>());
        add_subcommand("unregister", std::make_shared<PluginUnregisterCmd>());
        add_subcommand("info", std::make_shared<PluginInfoCmd>());
    }

    void cmd() override {}

    std::string get_cmd_description() override {
        return "Plugin sub-command - manages plugins";
    }

};
