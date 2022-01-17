#include <iostream>

#define SLAP_IMPL
#include "slap.h"

int main() {
    std::string test_lua = "test.lua";
    slap::exec_file(test_lua);

    std::string name = slap::get_table_key<std::string>("person", "name");

    slap::LOG("My Result: ", name);

    slap::close();  // always close vm
    return 0;
}
