#include <string>
#include <vector>
#include <memory>
#include <map>

#include "SubCmd.hpp"

int main(int argc, char** argv) {
    RootCmd root_cmd;

    root_cmd.proceed(argc, argv);
    return 0;
}