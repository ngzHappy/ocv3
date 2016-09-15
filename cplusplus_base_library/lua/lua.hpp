#ifndef __CPP_LUA_HXX_HPP_0x00
#define __CPP_LUA_HXX_HPP_0x00

/*
为lua函数、编译期常量重命名
以后lua升级就可以轻松适配
*/

/*c++ libs*/
#include <cmath>
#include <chrono>
#include <cfloat>
#include <cstdio>/*snprintf*/
#include <climits>
#include <cstddef>
#include <cstdarg>
#include <utility>
#include <algorithm>
#include <cinttypes>
using namespace std::literals::chrono_literals;
/*lua lib*/
#include "___lua.hpp"

namespace lua {

inline namespace _import_user_define {
using enum_base_type=int;
using unknow_return_type=int;
}

inline namespace _import_luaconf_h {
/*
LUA_USE_LINUX
LUA_USE_MACOSX
LUA_USE_WINDOWS
*/

/*
LUA_COMPAT_5_2
LUA_COMPAT_5_1
*/

enum :enum_base_type {
    BITSINT=LUAI_BITSINT
};

enum :enum_base_type {
    INT_INT=LUA_INT_INT,
    INT_LONG=LUA_INT_LONG,
    INT_LONGLONG=LUA_INT_LONGLONG,
};

enum :enum_base_type {
    FLOAT_FLOAT=LUA_FLOAT_FLOAT,
    FLOAT_DOUBLE=LUA_FLOAT_DOUBLE,
    FLOAT_LONGDOUBLE=LUA_FLOAT_LONGDOUBLE,
};

enum :enum_base_type {
    INT_TYPE=LUA_INT_TYPE,
    FLOAT_TYPE=LUA_FLOAT_TYPE,
};

enum :enum_base_type {
    MAXSTACK=LUAI_MAXSTACK,
    EXTRASPACE=LUA_EXTRASPACE,
    IDSIZE=LUA_IDSIZE,
    BUFFERSIZE=LUAL_BUFFERSIZE,
};

/*in lobject.c?? MAXNUMBER2STR*/
/*change define #define l_sprintf(s,sz,f,i)	snprintf(s,sz,f,i)*/
/*return buffer size*/
inline auto number2str(char * argBuffer,std::size_t argBufferSize,LUA_NUMBER argNumber)->std::size_t { return lua_number2str(argBuffer,argBufferSize,argNumber); }
/*return if success*/
inline auto numbertointeger(lua_Number argDouble,lua_Integer*argAns) ->bool { return lua_numbertointeger(argDouble,argAns); }
/*@see: std::strtod */
inline auto str2number(const char *argNumber,char **argEnd)->lua_Number { return lua_str2number(argNumber,argEnd); }

}

inline namespace _import_lua_h {

enum :enum_base_type { MULTRET=LUA_MULTRET };
inline lua_Integer upvalueindex(lua_Integer i) { return lua_upvalueindex(i); }

/* thread status */
enum ThreadStatus :enum_base_type {
    OK=LUA_OK,
    YIELD=LUA_YIELD,
    ERRRUN=LUA_ERRRUN,
    ERRSYNTAX=LUA_ERRSYNTAX,
    ERRMEM=LUA_ERRMEM,
    ERRGCMM=LUA_ERRGCMM,
    ERRERR=LUA_ERRERR,
};

typedef lua_State State;
typedef lua_Number Number;
typedef lua_Integer Integer;
typedef lua_Unsigned Unsigned;
typedef lua_KContext KContext;
typedef lua_CFunction CFunction;
typedef lua_KFunction KFunction;
typedef lua_Reader Reader;
typedef lua_Writer Writer;
typedef lua_Alloc Alloc;

enum BaseTypes :enum_base_type {
    TNONE=LUA_TNONE,
    TNIL=LUA_TNIL,
    TBOOLEAN=LUA_TBOOLEAN,
    TLIGHTUSERDATA=LUA_TLIGHTUSERDATA,
    TNUMBER=LUA_TNUMBER,
    TSTRING=LUA_TSTRING,
    TTABLE=LUA_TTABLE,
    TFUNCTION=LUA_TFUNCTION,
    TUSERDATA=LUA_TUSERDATA,
    TTHREAD=LUA_TTHREAD,
    NUMTAGS=LUA_NUMTAGS,
};

enum :enum_base_type {
    MINSTACK=LUA_MINSTACK,
    RIDX_MAINTHREAD=LUA_RIDX_MAINTHREAD,
    RIDX_GLOBALS=LUA_RIDX_GLOBALS,
    RIDX_LAST=LUA_RIDX_LAST,
};

enum ArithmeticFunctionOptions :enum_base_type {
    OPADD=LUA_OPADD,
    OPSUB=LUA_OPSUB,
    OPMUL=LUA_OPMUL,
    OPMOD=LUA_OPMOD,
    OPPOW=LUA_OPPOW,
    OPDIV=LUA_OPDIV,
    OPIDIV=LUA_OPIDIV,
    OPBAND=LUA_OPBAND,
    OPBOR=LUA_OPBOR,
    OPBXOR=LUA_OPBXOR,
    OPSHL=LUA_OPSHL,
    OPSHR=LUA_OPSHR,
    OPUNM=LUA_OPUNM,
    OPBNOT=LUA_OPBNOT,
};

enum LogicalFunctionOptions :enum_base_type {
    OPEQ=LUA_OPEQ,
    OPLT=LUA_OPLT,
    OPLE=LUA_OPLE,
};

enum GCOptions :enum_base_type {
    GCSTOP=LUA_GCSTOP,
    GCRESTART=LUA_GCRESTART,
    GCCOLLECT=LUA_GCCOLLECT,
    GCCOUNT=LUA_GCCOUNT,
    GCCOUNTB=LUA_GCCOUNTB,
    GCSTEP=LUA_GCSTEP,
    GCSETPAUSE=LUA_GCSETPAUSE,
    GCSETSTEPMUL=LUA_GCSETSTEPMUL,
    GCISRUNNING=LUA_GCISRUNNING,
};

/*创建新的lua虚拟机,并返回主栈区*/
inline auto newstate(Alloc f,void *ud)->State * { return lua_newstate(f,ud); }
/*关闭lua虚拟机*/
inline auto close(lua_State *L)->void { return lua_close(L); }
/*创建新的线程（协程）*/
inline auto newthread(State *L)->State * { return lua_newthread(L); }
/*内存不足回调函数*/
inline auto atpanic(State *L,CFunction panicf)->CFunction { return lua_atpanic(L,panicf); }
/*返回lua版本号*/
inline auto version(State *L)->const lua_Number *{ return lua_version(L); }
/*返回栈区绝对路径*/
inline auto absindex(State *L,int idx)->int { return lua_absindex(L,idx); }
/*返回栈区大小*/
inline auto gettop(State *L)->int { return lua_gettop(L); }
/*调整栈区大小*/
inline auto settop(State *L,int idx)->void { return lua_settop(L,idx); }
/*将栈区idx值拷贝一遍并放到栈顶*/
inline auto pushvalue(State *L,int idx)->void { return lua_pushvalue(L,idx); }
/*在top和idx之间平移n个单位,如果n>0则向top方向平移,如果n<0则向idx方向平移*/
inline auto rotate(State *L,int idx,int n)->void { return lua_rotate(L,idx,n); }
/*将栈区fromidx拷贝到toidx*/
inline auto copy(State *L,int fromidx,int toidx)->void { return lua_copy(L,fromidx,toidx); }
/*如果栈区没有n个空余空间则强制分配*/
inline auto checkstack(State *L,int n)->bool { return lua_checkstack(L,n)>0; }
/*将from协程的n个value弹出并压入to协程*/
inline auto xmove(State *from,State *to,int n)->void { return lua_xmove(from,to,n); }
/*判断是否是number*/
inline auto isnumber(State *L,int idx)->bool { return lua_isnumber(L,idx)>0; }
/*判断是否是string*/
inline auto isstring(State *L,int idx)->bool { return lua_isstring(L,idx)>0; }
/*判断是否是cfunction*/
inline auto iscfunction(State *L,int idx)->bool { return lua_iscfunction(L,idx)>0; }
/*判断是否是integer*/
inline auto isinteger(State *L,int idx)->bool { return lua_isinteger(L,idx)>0; }
/*判断是否是userdata*/
inline auto isuserdata(State *L,int idx)->bool { return lua_isuserdata(L,idx)>0; }
/*获得type类型*/
inline auto type(State *L,int idx)->BaseTypes { return static_cast<BaseTypes>(lua_type(L,idx)); }
/*输出type类型的名字*/
inline auto type_name(State *L,BaseTypes tp)->const char * { return lua_typename(L,tp); }
/*转为小数*/
inline auto tonumberx(State *L,int idx,int *isnum)->lua_Number { return lua_tonumberx(L,idx,isnum); }
/*转为整数*/
inline auto tointegerx(State *L,int idx,int *isnum)->lua_Integer { return lua_tointegerx(L,idx,isnum); }
/*转为布尔*/
inline auto toboolean(State *L,int idx)->int { return lua_toboolean(L,idx); }
/*转为字符串*/
inline auto tolstring(State *L,int idx,size_t *len)->const char * { return lua_tolstring(L,idx,len); }
/*获得len*/
inline auto rawlen(State *L,int idx)->std::size_t { return lua_rawlen(L,idx); }
/*转为cfunction*/
inline auto tocfunction(State *L,int idx)->lua_CFunction { return lua_tocfunction(L,idx); }
/*转为userdata*/
inline auto touserdata(State *L,int idx)->void * { return lua_touserdata(L,idx); }
/*转为协程*/
inline auto tothread(State *L,int idx)->State * { return lua_tothread(L,idx); }
/*转为指针*/
inline auto topointer(State *L,int idx)->const void * { return lua_topointer(L,idx); }
/*执行数字运算*/
inline auto arith(State *L,ArithmeticFunctionOptions op) ->void { return lua_arith(L,static_cast<int>(op)); }
/*比较两个值是否相等*/
inline auto rawequal(State *L,int idx1,int idx2)->bool { return lua_rawequal(L,idx1,idx2)>0; }
/*
LUA_OPEQ: compares for equality (==)
LUA_OPLT: compares for less than (<)
LUA_OPLE: compares for less or equal (<=)
*/
inline auto compare(State *L,int idx1,int idx2,LogicalFunctionOptions op)->bool { return lua_compare(L,idx1,idx2,op)>0; }
/**/
inline auto pushnil(State *L)->void { return lua_pushnil(L); }
/**/
inline auto pushnumber(State *L,lua_Number n)->void { return lua_pushnumber(L,n); }
/**/
inline auto pushinteger(State *L,lua_Integer n)->void { return lua_pushinteger(L,n); }
/**/
inline auto pushlstring(State *L,const char *s,size_t len)->const char * { return lua_pushlstring(L,s,len); }
template<std::size_t N>
inline auto pushlstring(State *L,const char(&arg)[N]) ->const char * { return pushlstring(L,arg,N-1); }
/**/
inline auto pushstring(State *L,const char *s)->const char * { return lua_pushstring(L,s); }
/**/
inline auto pushvfstring(State *L,const char *fmt,va_list argp)->const char * { return lua_pushvfstring(L,fmt,argp); }
/**/
inline auto pushfstring(State *L,const char *fmt,...)->const char * { struct __Lock { va_list ap; __Lock(const char * fmt) { va_start(ap,fmt); } ~__Lock() { va_end(ap); } }argp={ fmt }; return pushvfstring(L,fmt,argp.ap); }
/**/
inline auto pushcclosure(State *L,lua_CFunction fn,int n)->void { return lua_pushcclosure(L,fn,n); }
/**/
inline auto pushboolean(State *L,int b)->void { return lua_pushboolean(L,b); }
/**/
inline auto pushlightuserdata(State *L,void *p)->void { return lua_pushlightuserdata(L,p); }
/* Returns 1 if this thread is the main thread of its state*/
inline auto pushthread(State *L)->int { return lua_pushthread(L); }
/**/
inline auto getglobal(State *L,const char *name)->BaseTypes { return static_cast<BaseTypes>(lua_getglobal(L,name)); }
/**/
inline auto gettable(State *L,int idx)->BaseTypes { return static_cast<BaseTypes>(lua_gettable(L,idx)); }
/**/
inline auto getfield(State *L,int idx,const char *k)->BaseTypes { return static_cast<BaseTypes>(lua_getfield(L,idx,k)); }
/**/
inline auto geti(State *L,int idx,lua_Integer n)->BaseTypes { return static_cast<BaseTypes>(lua_geti(L,idx,n)); }
/**/
inline auto rawget(State *L,int idx)->BaseTypes { return static_cast<BaseTypes>(lua_rawget(L,idx)); }
/**/
inline auto rawgeti(State *L,int idx,lua_Integer n)->BaseTypes { return static_cast<BaseTypes>(lua_rawgeti(L,idx,n)); }
/**/
inline auto rawgetp(State *L,int idx,const void *p)->BaseTypes { return static_cast<BaseTypes>(lua_rawgetp(L,idx,p)); }
/**/
inline auto createtable(State *L,int narr,int nrec)->void { return lua_createtable(L,narr,nrec); }
/**/
inline auto newuserdata(State *L,size_t sz)->void * { return lua_newuserdata(L,sz); }
/**/
inline auto getmetatable(State *L,int objindex)->int { return lua_getmetatable(L,objindex); }
/**/
inline auto getuservalue(State *L,int idx)->BaseTypes { return static_cast<BaseTypes>(lua_getuservalue(L,idx)); }
/**/
inline auto setglobal(State *L,const char *name)->void { return lua_setglobal(L,name); }
/**/
inline auto settable(State *L,int idx) ->void { return lua_settable(L,idx); }
/**/
inline auto setfield(State *L,int idx,const char *k)->void { return lua_setfield(L,idx,k); }
/**/
inline auto seti(State *L,int idx,lua_Integer n) ->void { return lua_seti(L,idx,n); }
/**/
inline auto rawset(State *L,int idx) ->void { return lua_rawset(L,idx); }
/**/
inline auto rawseti(State *L,int idx,lua_Integer n) ->void { return lua_rawseti(L,idx,n); }
/**/
inline auto rawsetp(State *L,int idx,const void *p) ->void { return lua_rawsetp(L,idx,p); }
/**/
inline auto setmetatable(State *L,int objindex)->int { return lua_setmetatable(L,objindex); }
/*replace user value*/
inline auto setuservalue(State *L,int idx)->void { return lua_setuservalue(L,idx); }
/**/
inline auto callk(State *L,int nargs,int nresults,KContext ctx,KFunction k)->void { return lua_callk(L,nargs,nresults,ctx,k); }
/**/
inline auto call(State * L,int n,int r) ->void { return lua_callk(L,n,r,0,nullptr); }
/**/
inline auto pcallk(State *L,int nargs,int nresults,int errfunc,KContext ctx,KFunction k)->ThreadStatus { return static_cast<ThreadStatus>(lua_pcallk(L,nargs,nresults,errfunc,ctx,k)); }
/**/
inline auto pcall(State*L,int n,int r,int errorFunction)->ThreadStatus { return pcallk(L,n,r,errorFunction,0,nullptr); }
/**/
inline auto load(State *L,Reader reader,void *dt,const char *chunkname,const char *mode) ->ThreadStatus { return static_cast<ThreadStatus>(lua_load(L,reader,dt,chunkname,mode)); }
/**/
inline auto dump(State *L,Writer writer,void *data,int strip)->ThreadStatus { return static_cast<ThreadStatus>(lua_dump(L,writer,data,strip)); }
/**/
inline auto yieldk(State *L,int nresults,KContext ctx,KFunction k) ->unknow_return_type { return lua_yieldk(L,nresults,ctx,k); }
/**/
inline auto resume(State *L,State *from,int narg)->ThreadStatus { return static_cast<ThreadStatus>(lua_resume(L,from,narg)); }
/**/
inline auto status(State *L) ->ThreadStatus { return static_cast<ThreadStatus>(lua_status(L)); }
/**/
inline auto isyieldable(State *L)->bool { return lua_isyieldable(L)>0; }
/**/
inline auto yield(State *L,int n)->decltype(auto) { return yieldk(L,n,0,nullptr); }
/**/
inline auto gc(State *L,GCOptions what,int data)->unknow_return_type { return lua_gc(L,what,data); }
/**/
inline auto error(State *L)->unknow_return_type { return lua_error(L); }
/**/
inline auto next(State *L,int idx)->bool { return lua_next(L,idx)>0; }
/**/
inline auto concat(State *L,int n)->void { return lua_concat(L,n); }
/**/
inline auto len(State *L,int idx)->void { return lua_len(L,idx); }
/**/
inline auto stringtonumber(State *L,const char *s)->std::size_t { return lua_stringtonumber(L,s); }
/**/
inline auto getallocf(State *L,void **ud)->Alloc { return lua_getallocf(L,ud); }
/**/
inline auto setallocf(State *L,Alloc f,void *ud)->void { return lua_setallocf(L,f,ud); }
/**/
inline auto getextraspace(State* L) ->void * { return ((void *)((char *)(L)-LUA_EXTRASPACE)); }
/**/
inline auto tonumber(State*L,int i)->lua_Number { return tonumberx(L,(i),nullptr); }
/**/
inline auto tointeger(State*L,int i) ->lua_Integer { return tointegerx(L,(i),nullptr); }
/**/
inline auto pop(State*L,int n) ->void { return settop(L,-(n)-1); }
/**/
inline auto newtable(State*L) ->void { return createtable(L,0,0); }
/**/
inline auto pushcfunction(State*L,CFunction f) ->void { return pushcclosure(L,f,0); }
/**/
inline auto register_function(State*L,const char * n,CFunction f) { pushcfunction(L,f); return setglobal(L,n); }
/**/
inline auto isfunction(State*L,int n)->bool { return type(L,(n))==BaseTypes::TFUNCTION; }
/**/
inline auto istable(State*L,int n)->bool { return type(L,(n))==LUA_TTABLE; }
/**/
inline auto islightuserdata(State*L,int n)->bool { return type(L,(n))==LUA_TLIGHTUSERDATA; }
/**/
inline auto isnil(State*L,int n)->bool { return type(L,(n))==LUA_TNIL; }
/**/
inline auto isboolean(State*L,int n)->bool { return type(L,(n))==LUA_TBOOLEAN; }
/**/
inline auto isthread(State*L,int n)->bool { return type(L,(n))==LUA_TTHREAD; }
/**/
inline auto isnone(State*L,int n)->bool { return type(L,(n))==LUA_TNONE; }
/**/
inline auto isnoneornil(State*L,int n)->bool { return type(L,(n))<=0; }
/**/
inline auto pushliteral(State*L,const char *s)->const char * { return pushstring(L,s); }
/**/
inline auto pushglobaltable(State*L) ->void { ((void)lua_rawgeti(L,LUA_REGISTRYINDEX,LUA_RIDX_GLOBALS)); return; }
/**/
inline auto tostring(State*L,int i) ->const char * { return tolstring(L,(i),nullptr); }
/**/
inline auto insert(State*L,int idx)->void { return lua_rotate(L,idx,1); }
/**/
inline auto remove(State*L,int idx) ->void { lua_rotate(L,(idx),-1); return pop(L,1); }
/**/
inline auto replace(State*L,int idx) ->void { lua_copy(L,-1,(idx)); return pop(L,1); }

inline namespace _debug_api_ {

enum EventCodes :enum_base_type {
    HOOKCALL=LUA_HOOKCALL,
    HOOKRET=LUA_HOOKRET,
    HOOKLINE=LUA_HOOKLINE,
    HOOKCOUNT=LUA_HOOKCOUNT,
    HOOKTAILCALL=LUA_HOOKTAILCALL,
};

enum EventMasks :enum_base_type {
    MASKCALL=LUA_MASKCALL,
    MASKRET=LUA_MASKRET,
    MASKLINE=LUA_MASKLINE,
    MASKCOUNT=LUA_MASKCOUNT,
};

typedef lua_Debug Debug;
typedef lua_Hook Hook;

inline auto getstack(State *L,int level,Debug *ar)->bool { return lua_getstack(L,level,ar)>0; }
inline auto getinfo(State *L,const char *what,Debug *ar)->bool { return lua_getinfo(L,what,ar)>0; }
inline auto getlocal(State *L,const Debug *ar,int n)->const char * { return lua_getlocal(L,ar,n); }
inline auto setlocal(State *L,const Debug *ar,int n)->const char * { return lua_setlocal(L,ar,n); }
inline auto getupvalue(State *L,int funcindex,int n)->const char * { return lua_getupvalue(L,funcindex,n); }
inline auto setupvalue(State *L,int funcindex,int n)->const char * { return lua_setupvalue(L,funcindex,n); }
inline auto upvalueid(State *L,int fidx,int n)->void * { return lua_upvalueid(L,fidx,n); }
inline auto upvaluejoin(State *L,int fidx1,int n1,int fidx2,int n2)->void { return lua_upvaluejoin(L,fidx1,n1,fidx2,n2); }
inline auto sethook(State *L,Hook func,int mask,int count)->void { return lua_sethook(L,func,mask,count); }
inline auto gethook(State *L)->Hook { return lua_gethook(L); }
inline auto gethookmask(State *L)->int { return lua_gethookmask(L); }
inline auto gethookcount(State *L)->int { return lua_gethookcount(L); }


}/*_debug_api_*/


}/*_import_lua_h*/

inline namespace _import_lualib_h {

inline auto open_base(State *L)->int { return luaopen_base(L); }
inline auto open_coroutine(State *L)->int { return luaopen_coroutine(L); }
inline auto open_table(State *L)->int { return luaopen_table(L); }
inline auto open_io(State *L)->int { return luaopen_io(L); }
inline auto open_os(State *L)->int { return luaopen_os(L); }
inline auto open_string(State *L)->int { return luaopen_string(L); }
inline auto open_utf8(State *L)->int { return luaopen_utf8(L); }
inline auto open_bit32(State *L)->int { return luaopen_bit32(L); }
inline auto open_math(State *L)->int { return luaopen_math(L); }
inline auto open_debug(State *L)->int { return luaopen_debug(L); }
inline auto open_package(State *L)->int { return luaopen_package(L); }
inline auto openlibs(State *L)->void { return luaL_openlibs(L); }

}/*_import_lualib_h*/

}/*lua*/

namespace luaL {

inline namespace _import_luaxlib_h {

using namespace lua::_import_user_define;
typedef luaL_Reg Reg;

enum RefOptions :enum_base_type {
    NOREF=LUA_NOREF,
    REFNIL=LUA_REFNIL,
    AllOthersIsOk,
};

inline auto checkversion_(lua::State *L,lua_Number ver,size_t sz)->void { luaL_checkversion_(L,ver,sz); }
inline auto checkversion(lua::State *L) ->void { return checkversion_(L,LUA_VERSION_NUM,LUAL_NUMSIZES); }
inline auto getmetafield(lua::State *L,int obj,const char *e)->lua::BaseTypes { return static_cast<lua::BaseTypes>(luaL_getmetafield(L,obj,e)); }
inline auto callmeta(lua::State *L,int obj,const char *e)->bool { return luaL_callmeta(L,obj,e)>0; }
inline auto tolstring(lua::State *L,int idx,size_t *len)->const char * { return luaL_tolstring(L,idx,len); }
inline auto argerror(lua::State *L,int arg,const char *extramsg)->unknow_return_type { return luaL_argerror(L,arg,extramsg); }
inline auto checklstring(lua::State *L,int arg,size_t *l)->const char * { return luaL_checklstring(L,arg,l); }
inline auto optlstring(lua::State *L,int arg,const char *def,size_t *l)->const char * { return luaL_optlstring(L,arg,def,l); }
inline auto checknumber(lua::State *L,int arg)->lua::Number { return luaL_checknumber(L,arg); }
inline auto optnumber(lua::State *L,int arg,lua::Number def)->lua_Number { return luaL_optnumber(L,arg,def); }
inline auto checkinteger(lua::State *L,int arg)->lua::Integer { return luaL_checkinteger(L,arg); }
inline auto optinteger(lua::State *L,int arg,lua::Integer def)->lua::Integer { return luaL_optinteger(L,arg,def); }
inline auto checkstack(lua::State *L,int sz,const char *msg)->void { return luaL_checkstack(L,sz,msg); }
inline auto checktype(lua::State *L,int arg,lua::BaseTypes t)->void { return luaL_checktype(L,arg,t); }
inline auto checkany(lua::State *L,int arg)->void { return luaL_checkany(L,arg); }
/*return true if the table is new created,else return false*/
inline auto newmetatable(lua::State *L,const char *tname)->bool { return luaL_newmetatable(L,tname)>0; }
inline auto setmetatable(lua::State *L,const char *tname)->void { return luaL_setmetatable(L,tname); }
inline auto testudata(lua::State *L,int ud,const char *tname)->void * { return luaL_testudata(L,ud,tname); }
inline auto checkudata(lua::State *L,int ud,const char *tname)->void * { return luaL_checkudata(L,ud,tname); }
inline auto where(lua::State *L,int lvl)->void { return luaL_where(L,lvl); }
inline auto error(lua::State *L,const char *fmt,...)->unknow_return_type { va_list argp; va_start(argp,fmt); luaL_where(L,1); lua_pushvfstring(L,fmt,argp); va_end(argp); lua_concat(L,2); return lua::error(L); }
inline auto checkoption(lua::State *L,int arg,const char *def,const char *const lst[])->unknow_return_type { return luaL_checkoption(L,arg,def,lst); }
inline auto fileresult(lua::State *L,int stat,const char *fname)->int { return luaL_fileresult(L,stat,fname); }
inline auto execresult(lua::State *L,int stat)->int { return luaL_execresult(L,stat); }
inline auto ref(lua::State *L,int t)->RefOptions { return static_cast<RefOptions>(luaL_ref(L,t)); }
inline auto unref(lua::State *L,int t,int ref)->void { return luaL_unref(L,t,ref); }
inline auto loadfilex(lua::State *L,const char *filename,const char *mode)->lua::ThreadStatus { return static_cast<lua::ThreadStatus>(luaL_loadfilex(L,filename,mode)); }
inline auto loadbufferx(lua::State *L,const char *buff,size_t sz,const char *name,const char *mode)->lua::ThreadStatus { return static_cast<lua::ThreadStatus>(luaL_loadbufferx(L,buff,sz,name,mode)); }
inline auto loadstring(lua::State *L,const char *s)->lua::ThreadStatus { return static_cast<lua::ThreadStatus>(luaL_loadstring(L,s)); }
inline auto newstate(void)->lua::State * { return luaL_newstate(); }
inline auto len(lua::State *L,int idx)->lua::Integer { return luaL_len(L,idx); }
inline auto gsub(lua::State *L,const char *s,const char *p,const char *r)->const char * { return luaL_gsub(L,s,p,r); }
inline auto setfuncs(lua::State *L,const Reg *l,int nup)->void { return luaL_setfuncs(L,l,nup); }
/*return true if it created a new table*/
inline auto getsubtable(lua::State *L,int idx,const char *fname)->bool { return luaL_getsubtable(L,idx,fname)>0; }
inline auto traceback(lua::State *L,lua::State *L1,const char *msg,int level)->void { return luaL_traceback(L,L1,msg,level); }
inline auto requiref(lua::State *L,const char *modname,lua::CFunction openf,int glb)->void { return luaL_requiref(L,modname,openf,glb); }
inline auto loadfile(lua::State * L,const char *f)->decltype(auto) { return loadfilex(L,f,nullptr); }
template<std::size_t _N_>
inline auto newlibtable(lua::State *L,const Reg(&l)[_N_])->void { return lua_createtable(L,0,sizeof(l)/sizeof((l)[0])-1); }
template<std::size_t _N_>
inline auto newlib(lua::State *L,const Reg(&l)[_N_])->void { checkversion(L); newlibtable(L,l); return setfuncs(L,l,0); }
inline auto argcheck(lua::State * L,int cond,int arg,const char *extramsg)->void { return ((void)((cond)||luaL_argerror(L,(arg),(extramsg)))); }
inline auto checkstring(lua::State *L,int n) ->const char * { return checklstring(L,n,nullptr); }
inline auto optstring(lua::State * L,int n,const char *d)->const char * { return optlstring(L,n,d,nullptr); }
inline auto type_name(lua::State *L,int i)->const char * { return lua::type_name(L,lua::type(L,(i))); }
/*false is no error ,true is has error*/
inline auto dofile(lua::State *L,const char * fn) ->bool { return (loadfile(L,fn)||lua::pcall(L,0,LUA_MULTRET,0)); }
/*false is no error ,true is has error*/
inline auto dostring(lua::State *L,const char *s) ->bool { return (luaL_loadstring(L,s)||lua_pcall(L,0,LUA_MULTRET,0)); }
inline auto getmetatable(lua::State * L,const char * n) ->lua::BaseTypes { return (lua::getfield(L,LUA_REGISTRYINDEX,(n))); }
inline auto loadbuffer(lua::State *L,const char *s,size_t sz,const char *n) { return loadbufferx(L,s,sz,n,NULL); }

typedef luaL_Buffer Buffer;

inline auto buffinit(lua_State *L,luaL_Buffer *B)->void { return luaL_buffinit(L,B); }
inline auto prepbuffsize(luaL_Buffer *B,size_t sz)->char * { return luaL_prepbuffsize(B,sz); }
inline auto addlstring(luaL_Buffer *B,const char *s,size_t l)->void { return luaL_addlstring(B,s,l); }
inline auto addstring(luaL_Buffer *B,const char *s)->void { return luaL_addstring(B,s); }
inline auto addvalue(luaL_Buffer *B)->void { return luaL_addvalue(B); }
inline auto pushresult(luaL_Buffer *B)->void { return luaL_pushresult(B); }
inline auto pushresultsize(luaL_Buffer *B,size_t sz)->void { return luaL_pushresultsize(B,sz); }
inline auto buffinitsize(lua_State *L,luaL_Buffer *B,size_t sz)->char * { return luaL_buffinitsize(L,B,sz); }
inline auto addchar(Buffer *B,char c)->void { luaL_addchar(B,c); };
inline auto addsize(Buffer *B,std::size_t s)->void { ((B)->n+=(s)); }
inline auto prepbuffer(Buffer *B) ->char * { return prepbuffsize(B,LUAL_BUFFERSIZE); }
}/*_import_luaxlib_h*/

}/*luaL*/

#endif
