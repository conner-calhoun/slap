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

template <typename T>
static T get_table_key(const std::string& table, const std::string& var);

static void exec_string(const std::string& cmd);
static void exec_file(const std::string& file_path);
static void check_state(int r);

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

/// Need one for each type sadly:(
template <typename T>
static auto get_table_key(const std::string& table, const std::string& var) -> T {
    auto* vm = slap::get_vm();
    T to_return{};

    lua_getglobal(vm, table.c_str());
    if (lua_istable(vm, -1)) {
        lua_pushstring(vm, var.c_str());
        lua_gettable(vm, -2);

        /// Use constexpr evaluation to determine which accessor needs to be used
        if constexpr (std::is_same<T, size_t>::value) {
            if (lua_isinteger(vm, -1)) {
                to_return = lua_tointeger(vm, -1);
            }
        }
        if constexpr (std::is_same<T, float>::value) {
            if (lua_isnumber(vm, -1)) {
                to_return = lua_tonumber(vm, -1);
            }
        }
        if constexpr (std::is_same<T, bool>::value) {
            if (lua_isboolean(vm, -1)) {
                to_return = lua_toboolean(vm, -1);
            }
        }
        if constexpr (std::is_same<T, std::string>::value) {
            if (lua_isstring(vm, -1)) {
                to_return = lua_tostring(vm, -1);
            }
        }

        lua_pop(vm, 1);
    }

    return to_return;
}

static void exec_string(const std::string& cmd) {
    auto* vm = slap::get_vm();
    int r = luaL_dostring(vm, cmd.c_str());

    slap::check_state(r);
}

static void exec_file(const std::string& file_path) {
    auto* vm = slap::get_vm();
    int r = luaL_dofile(vm, file_path.c_str());

    slap::check_state(r);
}

static void check_state(int r) {
    auto* vm = get_vm();

    if (r != LUA_OK) {
        std::string errs = lua_tostring(vm, -1);
        ERRLOG(errs);
    }
}

static void close() {
    auto* vm = get_vm();
    lua_close(vm);
}

#endif  // SLAP_IMPL

}  // namespace slap
