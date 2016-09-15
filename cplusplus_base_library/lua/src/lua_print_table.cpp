#ifndef LUA_LIB
#define LUA_LIB
#endif

#include "print_table.hpp"

namespace  {
namespace __private {

template<typename _T_>
void print_table(lua::State*L,int t,_T_*c){
    if(L==nullptr){return;}
    if(c==nullptr){return;}
    t=lua::absindex(L,t);
    if(lua::istable(L,t)==false){
        lua::pushlstring(L,"input is not a table");
        lua::error(L);
    }
}

}/*__private*/
}/*namespace*/

namespace luaL{

void print_table(lua::State*L,int t,PrintTableCallback*c){
    return __private::print_table(L,t,c);
}

}

/**/
