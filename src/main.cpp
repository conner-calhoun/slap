#include <iostream>

#define SLAP_IMPL
#include "slap.h"

int main() {
    std::string test_lua = "test.lua";
    slap::exec_file(test_lua);

    std::string name = slap::get_table_key<std::string>("person", "name");
    size_t age = slap::get_table_key<size_t>("person", "age");  /// pls use size_t for "ints" to avoid warnings
    bool is_cool = slap::get_table_key<bool>("person", "cool");

    slap::LOG("Name: ", name, ", Age: ", age, ", Is Cool? ", is_cool);

    slap::close();  // always close vm
    return 0;
}
