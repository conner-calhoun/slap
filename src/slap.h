/**
 * Simple Lua App Plugin (slap)
 *
 *
 * Note: This library requires lua libraries. They should be bundled with
 * this repo, but you can substitute your own as well. You will need to determine
 * the best way to build for you system though ¯\_(ツ)_/¯
 *
 */
#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "luaconf.h"
}

namespace slap {

///==================================================
/// HEADER
///==================================================

template <typename... Args>
static void LOG(Args&&... args);
template <typename... Args>
static void ERRLOG(Args&&... args);

static lua_State* get_vm();

template <typename T>
static T get_global(const std::string& var);

static void exec_string(const std::string& cmd);
static void report_errors();
static void close();

///==================================================
/// IMPLMENTATION
///==================================================
#ifdef SLAP_IMPL

template <typename... Args>
static void LOG(Args&&... args) {
    (std::cout << ... << args) << std::endl;
}
template <typename... Args>
static void ERRLOG(Args&&... args) {
    std::cerr << "slap::error: ";
    (std::cerr << ... << args) << std::endl;
}

/// Globals
static lua_State* s_vm = nullptr;

static lua_State* get_vm() {
    if (s_vm) return s_vm;

    s_vm = luaL_newstate();
    return s_vm;
}

template <typename T>
static T get_global(const std::string& var) {
    auto* vm = slap::get_vm();
    lua_getglobal(vm, var.c_str());
    if (lua_isnumber(vm, -1)) {
        return (float)lua_tonumber(vm, -1);
    }

    return -1;
}

static void exec_string(const std::string& cmd) {
    auto* vm = slap::get_vm();
    int r = luaL_dostring(vm, cmd.c_str());

    if (r != LUA_OK) {
        slap::report_errors();
    }
}

static void report_errors() {
    auto* vm = get_vm();

    std::string errs = lua_tostring(vm, -1);
    ERRLOG(errs);
}

static void close() {
    auto* vm = get_vm();
    lua_close(vm);
}

#endif  // SLAP_IMPL

}  // namespace slap
