#ifndef _m_HPP_LUA_DEFAULT_ERROR_FUNCTION_CPP
#define _m_HPP_LUA_DEFAULT_ERROR_FUNCTION_CPP() 1

#include "../lua.hpp"

namespace luaL {

class StateLock {
    lua::State * L_;
    int top_;
public:
    StateLock(lua::State *_s):L_(_s) { if (L_) { top_=lua::gettop(L_); } }
    ~StateLock() { if (L_) { lua::settop(L_,top_); } }
    void clear() { L_=nullptr; }
};


LUA_API int default_lua_error_function(lua::State*);

}

#endif
