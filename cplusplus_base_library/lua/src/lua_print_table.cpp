#ifndef LUA_LIB
#define LUA_LIB
#endif

#include "../../memory/MemoryLibrary.hpp"
#include "print_table.hpp"
#include "default_error_function.hpp"
#include "3rd/double-conversion/include/double-conversion/double-conversion.h"
#include <cstdlib>
#include <string>
#include <regex>
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <sstream>

namespace {
namespace __private {

using IntType=int;
using string=std::basic_string<char,std::char_traits<char>,memory::Allocator<char>>;
using TablePath=std::list<IntType,memory::Allocator<int>>;

class Function {
    void *data_;
    void(*call_)(void*);
    void(*free_)(void*);
    Function(const Function&o)=delete;
    Function&operator=(const Function&o)=delete;
public:
    template<typename _a,typename _b>
    Function(void * d,_a&&c,_b&&f):data_(d),call_(c),free_(f) {}
    Function(Function&&o):data_(o.data_),call_(o.call_),free_(o.free_) {
        o.data_=nullptr;
        o.call_=nullptr;
        o.free_=nullptr;
    }
    Function&operator=(Function&&o) {
        if (this==&o) { return *this; }
        this->~Function();
        this->data_=o.data_; o.data_=nullptr;
        this->free_=o.free_; o.free_=nullptr;
        this->call_=o.call_; o.call_=nullptr;
        return *this;
    }
    void call() { if (call_) { call_(data_); } }
    ~Function() { if (free_) { free_(data_); } }
};
using DutiesType=std::list<Function,memory::Allocator<Function>>;

class KeyItem {
public:
    string tableName;
    bool isTableNameIsIntAndIsContinueInParent=false;
    IntType parentTableIndex;
    bool isArrayKeyContinue=true;
    template<typename _T_>
    KeyItem(_T_&&tn,IntType pti):tableName(std::forward<_T_>(tn)),
        parentTableIndex(pti) {}
    KeyItem() { parentTableIndex=-1; }
};

using AllTables=std::vector<KeyItem,memory::Allocator<KeyItem>>;
using TablesMap=std::map<const void *,IntType,std::less<const void *>,memory::Allocator<int>>;

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
    return{ nullptr,0 };
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
    for (data=begin_; (begin_!=end_); --begin_) {
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
    for (data=begin_; (begin_!=end_); --begin_) {
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
    for (data=begin_; (begin_!=end_); --begin_) {
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

// virtual TempStringData temp_space()const =0;
//virtual void write_string(const char*,std::size_t)=0;

inline std::size_t find_string_op(const char * begin,const char *end) {
    if (begin>=end) { return 0; }
    {
        /* add [[ ]] */
        const static std::regex op_begin(u8R"___(=+)___");

        std::size_t count_=0;
        std::cmatch result;

        {
            const char * pos=begin;
            while (std::regex_search(pos,end,result,op_begin)) {
                auto &r0=result[0];
                count_=std::max<std::size_t>(count_,r0.length());
                pos=r0.second;
            }
        }

        return 2+count_;

    }
    return 0;
}

template<typename _T_>
string to_string(lua::State*L,int k,_T_*) {
    std::size_t length_=0;
    const char * data_=luaL::tolstring(L,k,&length_);
    if (length_==0) {
        lua::pop(L,1);
        return{};
    }
    auto op_size_=find_string_op(data_,data_+length_);
    const string op_(op_size_,'=');
    string str(data_,length_);
    lua::pop(L,1);
    return "["+op_+"["
        +std::move(str)
        +"]"+op_+"]";
}

template<typename _T_>
string key_string(lua::State*L,int k,_T_*c) {
    if (lua::isinteger(L,k)) {
        auto key=lua::tointeger(L,k);
        auto tmp_data=c->temp_space();
        auto string_key=number_string(key,tmp_data.data,tmp_data.length);
        if (string_key.length==0) { return{}; }
        return string(string_key.data,string_key.length);
    }
    else {
        return to_string(L,k,c);
    }
}

template<typename _T_>
string value_string(lua::State*L,int v,_T_*c) {
    lua::BaseTypes t=lua::type(L,v);
    switch (t) {
        case lua::TNONE:return"nil"; break;
        case lua::TNIL:return"nil"; break;
        case lua::TBOOLEAN: if (lua::toboolean(L,v)) { return"true"; }
                            else { return "false"; } break;
        case lua::TLIGHTUSERDATA:return"TLIGHTUSERDATA"; break;
        case lua::TNUMBER:
            if (lua::isinteger(L,v)) {
                auto i=lua::tointeger(L,v);
                auto tmp_data=c->temp_space();
                auto ans=number_string(i,tmp_data.data,tmp_data.length);
                if (ans.data&&ans.length) { return string(ans.data,ans.length); }
            }
            else {
                auto d=lua::tonumber(L,v);
                auto tmp_data=c->temp_space();
                auto ans=number_string(d,tmp_data.data,tmp_data.length);
                if (ans.data&&ans.length) { return string(ans.data,ans.length); }
            }
            break;
        case lua::TSTRING: return to_string(L,v,c); break;
        case lua::TTABLE:return"TTABLE"; break;
        case lua::TFUNCTION:return"TFUNCTION"; break;
        case lua::TUSERDATA:return"TUSERDATA"; break;
        case lua::TTHREAD:return"TTHREAD"; break;
        case lua::NUMTAGS:return"NUMTAGS"; break;
        default:break;
    }
    return{};
}


template<typename _T_>
class DataPrintTable {
public:
    _T_ * callback;
    TablePath tablePath;
    AllTables allTables;
    TablesMap tablesMap;
    DutiesType duties;
    lua::State * L;
    IntType sources_table;
    IntType table_count;
};

template<typename _C_>
class PrintString {
    string _m_String;
    DataPrintTable<_C_> * _m_DataPrintTable;
public:
    template<typename _S_>
    PrintString(_S_&&s,DataPrintTable<_C_> *p):_m_String(std::move(s)),
        _m_DataPrintTable(p) {}
    void call() {
        _m_DataPrintTable->callback->write_string(
            _m_String.c_str(),
            static_cast<int>(_m_String.size()));
    }
private:
    MEMORY_CLASS_NEW_DELETE
};

template<typename _C_>
class BeginPrintATable {
    IntType _m_TableIndex;
    DataPrintTable<_C_> * _m_DataPrintTable;
public:
    BeginPrintATable(IntType ti,DataPrintTable<_C_> *dt):
        _m_TableIndex(ti),
        _m_DataPrintTable(dt) {
    }

    void call() {
        auto & keyItem=_m_DataPrintTable->allTables[_m_TableIndex];
        if (false==keyItem.isTableNameIsIntAndIsContinueInParent) {
            _m_DataPrintTable->callback->write_string("[ ",2);
            _m_DataPrintTable->callback->write_string(
                keyItem.tableName.c_str(),
                static_cast<int>(keyItem.tableName.size()));
            _m_DataPrintTable->callback->write_string(" = { ",5);
            _m_DataPrintTable->callback->write_string(" ]",2);
        }
        else {
            _m_DataPrintTable->callback->write_string("{ ",2);
        }
    }

private:
    MEMORY_CLASS_NEW_DELETE
};

template<typename _C_>
class EndPrintATable {
    IntType _m_TableIndex;
    DataPrintTable<_C_> * _m_DataPrintTable;
public:
    EndPrintATable(IntType ti,DataPrintTable<_C_> *dt):
        _m_TableIndex(ti),
        _m_DataPrintTable(dt) {
    }

    void call() {
        _m_DataPrintTable->callback->write_string("},",2);
        _m_DataPrintTable->tablePath.pop_back();
    }
private:
    MEMORY_CLASS_NEW_DELETE
};

template<typename _C_>
class PrintATable {
    IntType _m_TableIndex;
    DataPrintTable<_C_> * _m_DataPrintTable;
public:
    PrintATable(IntType ti,DataPrintTable<_C_> *dt):
        _m_TableIndex(ti),
        _m_DataPrintTable(dt) {
    }

    void call() {
        auto L=_m_DataPrintTable->L;
        const auto lock_top=lua::gettop(L);

#ifndef function_return
#define function_return() lua::settop(L,lock_top);return
#endif 

        _m_DataPrintTable->tablePath.push_back(_m_TableIndex);
        _m_DataPrintTable->duties.emplace_front(
            new EndPrintATable<_C_>(_m_TableIndex,_m_DataPrintTable),
            [](void *arg) {reinterpret_cast<EndPrintATable<_C_>*>(arg)->call(); },
            [](void *arg) {delete reinterpret_cast<EndPrintATable<_C_>*>(arg); }
        );

        auto duties_insert_pos=_m_DataPrintTable->duties.begin();
        _m_DataPrintTable->duties.emplace(duties_insert_pos,
            new BeginPrintATable<_C_>(_m_TableIndex,_m_DataPrintTable),
            [](void *arg) {reinterpret_cast<BeginPrintATable<_C_>*>(arg)->call(); },
            [](void *arg) {delete reinterpret_cast<BeginPrintATable<_C_>*>(arg); }
        );

        lua::rawgeti(L,_m_DataPrintTable->sources_table,_m_TableIndex);
        const auto table_index=lua::gettop(L);

        auto & keyItem=_m_DataPrintTable->allTables[_m_TableIndex];
        {/*children：*/
            lua::pushnil(L);
            const auto stack_lock=lua::gettop(L);
            constexpr auto key_=-2;
            constexpr auto value_=-1;
            IntType arrayKey=1;
            while (lua::next(L,table_index)) {

                /*is key is int and continue*/
                if (keyItem.isArrayKeyContinue) {
                    keyItem.isArrayKeyContinue=lua::isinteger(L,key_);
                    if (keyItem.isArrayKeyContinue) {
                        auto ikey=lua::tointeger(L,key_);
                        if (ikey==arrayKey) {
                            ++arrayKey;
                        }
                        else {
                            keyItem.isArrayKeyContinue=false;
                        }
                    }
                }

                if (lua::istable(L,value_)) {
                    auto table_id=lua::topointer(L,value_);
                    auto table_pos=_m_DataPrintTable->tablesMap.find(table_id);
                    if (table_pos==_m_DataPrintTable->tablesMap.end()) {/*new child*/
                        {
                            KeyItem thisKeyItem;
                            thisKeyItem.isTableNameIsIntAndIsContinueInParent=keyItem.isArrayKeyContinue;
                            thisKeyItem.parentTableIndex=_m_TableIndex;
                            thisKeyItem.tableName=key_string(L,key_,_m_DataPrintTable->callback);
                            _m_DataPrintTable->allTables.push_back(std::move(thisKeyItem));
                        }

                        auto this_table_index=++_m_DataPrintTable->table_count;
                        _m_DataPrintTable->tablesMap.emplace(table_id,this_table_index);
                        lua::pushvalue(L,value_);
                        lua::rawseti(L,_m_DataPrintTable->sources_table,this_table_index);

                        _m_DataPrintTable->duties.emplace(
                            duties_insert_pos,
                            new PrintATable<_C_>{ this_table_index,_m_DataPrintTable },
                            [](void *arg) {reinterpret_cast<PrintATable<_C_>*>(arg)->call(); },
                            [](void *arg) {delete reinterpret_cast<PrintATable<_C_>*>(arg); }
                        );

                        lua::settop(L,stack_lock);
                    }
                    else {/*old one*/

                        lua::settop(L,stack_lock);
                    }

                }
                else {
                    string str;
                    str.reserve(64);
                    if (keyItem.isArrayKeyContinue) {
                        str=value_string(L,value_,_m_DataPrintTable->callback);
                        str+=" , ";
                    }
                    else {
                        str=" [ ";
                        str+=key_string(L,key_,_m_DataPrintTable->callback);
                        str+=" ] = ";
                        str=value_string(L,value_,_m_DataPrintTable->callback);
                        str+=" , ";
                    }
                    _m_DataPrintTable->duties.emplace(
                        duties_insert_pos,
                        new PrintString<_C_>{ std::move(str),_m_DataPrintTable },
                        [](void *arg) {reinterpret_cast<PrintString<_C_>*>(arg)->call(); },
                        [](void *arg) {delete reinterpret_cast<PrintString<_C_>*>(arg); }
                    );
                    lua::settop(L,stack_lock);
                }
            }
        }

        function_return();
#undef function_return
    }
private:
    MEMORY_CLASS_NEW_DELETE
};

template<typename _T_>
int print_table(lua::State *L) {
    constexpr std::size_t source_table_index=1;
    constexpr std::size_t source_callback_index=-1;

    if (lua::istable(L,source_table_index)==false) {
        lua::pushlstring(L,"source is not a table");
        lua::error(L);
    }

    if (lua::islightuserdata(L,source_callback_index)==false) {
        lua::pushlstring(L,"call back is not right");
        lua::error(L);
    }

    DataPrintTable<_T_> dataPrintTable;

    {/*init datas*/
        lua::newtable(L);
        dataPrintTable.sources_table=lua::gettop(L);
        lua::pushvalue(L,source_table_index);
        lua::rawseti(L,dataPrintTable.sources_table,1);
        dataPrintTable.L=L;
        dataPrintTable.callback=
            reinterpret_cast<_T_*>(lua::touserdata(L,source_callback_index));
        dataPrintTable.allTables.emplace_back("",-1)/*zefo is never used*/;
        dataPrintTable.allTables.emplace_back("ans",0)/*one is root*/;
        dataPrintTable.table_count=1;
        dataPrintTable.tablesMap.emplace(lua::topointer(L,source_table_index),1);
        dataPrintTable.duties.emplace_front(
            new PrintATable<_T_>{ 1,&dataPrintTable },
            [](void *arg) {reinterpret_cast<PrintATable<_T_>*>(arg)->call(); },
            [](void *arg) {delete reinterpret_cast<PrintATable<_T_>*>(arg); }
        );
    }

    while (dataPrintTable.duties.empty()==false) {
        auto duty=std::move(*dataPrintTable.duties.begin());
        dataPrintTable.duties.pop_front();
        duty.call();
    }

    return 0;
}

}/*__private*/
}/*namespace*/

namespace luaL {

lua::ThreadStatus print_table(lua::State*L,int t,PrintTableCallback*c) {
    if (L==nullptr) { return lua::ERRERR; }
    t=lua::absindex(L,t);

    {
        lua::pushcfunction(L,&__private::print_table<PrintTableCallback>);
        lua::pushvalue(L,t);
        lua::pushcfunction(L,&luaL::default_lua_error_function);
        auto epos=lua::gettop(L);
        lua::pushlightuserdata(L,c);
        return lua::pcall(L,3,lua::MULTRET,epos);
    }

}

int function_print_table(lua::State*L) {

    if (L==nullptr) { return 0; }

    class _PrintTableCallback final {
        char tdata_[128];
        std::basic_stringstream<char,std::char_traits<char>,
            memory::Allocator<char> >  about_to_write;
    public:
        PrintTableCallback::TempStringData temp_space()const {
            return{ (char*)(tdata_),128 };
        }
        void write_string(const char*d,std::size_t l) {
            about_to_write.write(d,l);
        }
        void begin() {}
        void finished() { std::cout<<about_to_write.rdbuf(); }
        void end() {}
    };

    _PrintTableCallback _c;
    lua::pushlightuserdata(L,&_c);

    __private::print_table<_PrintTableCallback>(L);

    return 0;
}

}

/**/
