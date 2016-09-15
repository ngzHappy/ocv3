#ifndef LUA_LIB
#define LUA_LIB
#endif

#include "../../memory/MemoryLibrary.hpp"
#include "print_table.hpp"
#include "3rd/double-conversion/include/double-conversion/double-conversion.h"
#include <cstdlib>
#include <string>

namespace {
namespace __private {

inline luaL::PrintTableCallback::TempStringData double_to_string(double d,
    char * data,
    int length) {
    auto & conversion=
        double_conversion::DoubleToStringConverter::EcmaScriptConverter();
    double_conversion::StringBuilder builder{ data,length };
    conversion.ToShortest(d,&builder);
    luaL::PrintTableCallback::TempStringData ans=
    { data,builder.position() };
    if (ans.length>0) { return ans; }
    return{nullptr,0};
}

constexpr const static char num_10_to_ascii[]
={
   '0','1','2','3','4','5','6','7','8','9',
   'a','b','c','d','e','f','g','h','i','j',
   'k','l','m','n','o','p','q','r','s','t',
   'u','v','w','x','y','z',
    'A','B','C','D','E','F','G','H','I','J',
    'K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z',
};

inline luaL::PrintTableCallback::TempStringData uint_to_string(
    int d,char * data,int length
) {
    char * begin_=data+length-1;
    char * end_=data-1;
    for (data=begin_; (begin_!=end_); ++begin_) {
        auto tmp=std::div(d,10);
        d=tmp.quot;
        *begin_=num_10_to_ascii[tmp.rem];
        if (d==0) { return{ begin_,data-begin_+1 }; }
    }
    return{ nullptr,0 };
}

inline luaL::PrintTableCallback::TempStringData uint_to_string(
    long d,char * data,int length
) {
    char * begin_=data+length-1;
    char * end_=data-1;
    for (data=begin_; (begin_!=end_); ++begin_) {
        auto tmp=std::ldiv(d,10);
        d=tmp.quot;
        *begin_=num_10_to_ascii[tmp.rem];
        if (d==0) { return{ begin_,data-begin_+1 }; }
    }
    return{ nullptr,0 };
}

inline luaL::PrintTableCallback::TempStringData uint_to_string(
    long long d,char * data,int length
) {
    char * begin_=data+length-1;
    char * end_=data-1;
    for (data=begin_; (begin_!=end_); ++begin_) {
        auto tmp=std::lldiv(d,10);
        d=tmp.quot;
        *begin_=num_10_to_ascii[tmp.rem];
        if (d==0) { return{ begin_,data-begin_+1 }; }
    }
    return{ nullptr,0 };
}

template<typename _T_>
inline luaL::PrintTableCallback::TempStringData int_to_string(
    _T_ d,
    char * data,
    int length
) {
    if (length<=0) { return{ nullptr,0 }; }
    if (data==nullptr) { return{ nullptr,0 }; }
    if (d==0) { *data='0'; return{ data,1 }; }
    if (d<0) {
        luaL::PrintTableCallback::TempStringData ans=
            uint_to_string(-d,data,length);
        if (ans.data==nullptr) { return ans; }
        if (ans.data==data) { return{ nullptr,0 }; }
        *(--ans.data)='-';
        ++ans.length;
        return ans;
    }
    else {
        return uint_to_string(d,data,length);
    }
}

inline luaL::PrintTableCallback::TempStringData number_string(double n,char *d,int l) {
    return double_to_string(n,d,l);
}
inline luaL::PrintTableCallback::TempStringData number_string(int n,char *d,int l) {
    return int_to_string(n,d,l);
}
inline luaL::PrintTableCallback::TempStringData number_string(long n,char *d,int l) {
    return int_to_string(n,d,l);
}
inline luaL::PrintTableCallback::TempStringData number_string(long long n,char *d,int l) {
    return int_to_string(n,d,l);
}


template<typename _T_>
void print_table(lua::State*L,int t,_T_*c) {
    if (L==nullptr) { return; }
    if (c==nullptr) { return; }
    t=lua::absindex(L,t);
    if (lua::istable(L,t)==false) {
        lua::pushlstring(L,"input is not a table");
        lua::error(L);
    }
}

}/*__private*/
}/*namespace*/

namespace luaL {

void print_table(lua::State*L,int t,PrintTableCallback*c) {
    return __private::print_table(L,t,c);
}

}

/**/
