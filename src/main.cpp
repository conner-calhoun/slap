#include <iostream>

#define SLAP_IMPL
#include "slap.h"

int main() {
    std::string cmd = "a = 6 + 11";

    slap::exec_string(cmd);
    float a = slap::get_global<float>("a");

    slap::LOG("My Result: ", a);

    slap::close();  // always close vm
    return 0;
}
