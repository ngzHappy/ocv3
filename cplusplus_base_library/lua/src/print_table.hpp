#ifndef _lm_PRINT_TABLE_HPP
#define _lm_PRINT_TABLE_HPP

#include "../lua.hpp"

namespace luaL {

class PrintTableCallback{
public:

    class TempStringData{
    public:
        char * data;
        std::size_t length/*at least 128*/;
    };

    virtual ~PrintTableCallback()=default;
    PrintTableCallback()=default;
    PrintTableCallback(const PrintTableCallback&)=default;
    PrintTableCallback(PrintTableCallback&&)=default;
    PrintTableCallback&operator=(const PrintTableCallback&)=default;
    PrintTableCallback&operator=(PrintTableCallback&&)=default;
    virtual TempStringData temp_space()const =0;
    virtual void write_string(const char*,std::size_t)=0;
};

LUA_API void print_table(lua::State*,PrintTableCallback*);

}/*luaL*/

#endif
