#ifndef LUA_LIB
#define LUA_LIB
#endif

#include"default_error_function.hpp"

#include <iostream>

namespace luaL {

int default_lua_error_function(lua::State*L) {
    if (L==nullptr) { return 0; }
    
    std::size_t length;
    auto ans=luaL::tolstring(L,-1,&length);

    if (ans&&length) {
        std::cout.write(ans,length)<<std::endl;
    } else{
        std::cout<<"lua:unknow error\n" ;
    }

    return 0;
}

}
