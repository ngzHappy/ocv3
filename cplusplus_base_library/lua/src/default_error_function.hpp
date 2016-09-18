#ifndef _m_HPP_LUA_DEFAULT_ERROR_FUNCTION_CPP
#define _m_HPP_LUA_DEFAULT_ERROR_FUNCTION_CPP() 1

#include "../lua.hpp"

namespace luaL {

class StateLock {
    lua::State * L_;
    int top_;
public:
    explicit StateLock(decltype(nullptr)):L_(nullptr),top_(0) {}
    explicit StateLock(lua::State * const _s):L_(_s),top_(0) {}
    StateLock(lua::State * const _s,const int _t):L_(_s),top_(_t) { reset(); }
    StateLock():L_(nullptr),top_(0) {}
    ~StateLock() { if (L_) { if (lua::gettop(L_)==top_) { return; } lua::settop(L_,top_); } }
    void reset(const int _t) { top_=_t; }
    void reset(lua::State *const _L) { L_=_L; }
    void reset() { if (L_) { top_=lua::gettop(L_); } }
    void reset(lua::State *const _L,const int _t) { L_=_L; top_=_t; }
    StateLock(const StateLock&)=delete;
    StateLock&operator=(const StateLock&)=delete;
    StateLock(StateLock&&arg):L_(arg.L_),top_(arg.top_) { arg.top_=0; arg.L_=nullptr; }
    StateLock&operator=(StateLock&&arg) {
        if (this==&arg) { return *this; }
        this->top_=arg.top_; this->L_=arg.L_;
        arg.top_=0; arg.L_=nullptr;
        return *this;
    }
    operator lua::State *() { return L_; }
    operator const lua::State *() const { return L_; }
};

LUA_API int default_lua_error_function(lua::State*);

}

#endif
