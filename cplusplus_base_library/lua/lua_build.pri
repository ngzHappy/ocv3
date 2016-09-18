include($$PWD/lua_base.pri)
include($$PWD/src/3rd/double-conversion/double-conversion.pri)

HEADERS += \
    $$PWD/lua-5.3.3/src/lapi.h \
    $$PWD/lua-5.3.3/src/lauxlib.h \
    $$PWD/lua-5.3.3/src/lcode.h \
    $$PWD/lua-5.3.3/src/lctype.h \
    $$PWD/lua-5.3.3/src/ldebug.h \
    $$PWD/lua-5.3.3/src/ldo.h \
    $$PWD/lua-5.3.3/src/lfunc.h \
    $$PWD/lua-5.3.3/src/lgc.h \
    $$PWD/lua-5.3.3/src/llex.h \
    $$PWD/lua-5.3.3/src/llimits.h \
    $$PWD/lua-5.3.3/src/lmem.h \
    $$PWD/lua-5.3.3/src/lobject.h \
    $$PWD/lua-5.3.3/src/lopcodes.h \
    $$PWD/lua-5.3.3/src/lparser.h \
    $$PWD/lua-5.3.3/src/lprefix.h \
    $$PWD/lua-5.3.3/src/lstate.h \
    $$PWD/lua-5.3.3/src/lstring.h \
    $$PWD/lua-5.3.3/src/ltable.h \
    $$PWD/lua-5.3.3/src/ltm.h \
    $$PWD/lua-5.3.3/src/lua.h \
    $$PWD/lua-5.3.3/src/lua.hpp \
    $$PWD/lua-5.3.3/src/luaconf.h \
    $$PWD/lua-5.3.3/src/lualib.h \
    $$PWD/lua-5.3.3/src/lundump.h \
    $$PWD/lua-5.3.3/src/lvm.h \
    $$PWD/lua-5.3.3/src/lzio.h \
    $$PWD/src/deep_copy_table.hpp \
    $$PWD/src/print_table.hpp \
    $$PWD/src/default_error_function.hpp

SOURCES += \
    $$PWD/lua-5.3.3/src/lapi.cpp \
    $$PWD/lua-5.3.3/src/lauxlib.cpp \
    $$PWD/lua-5.3.3/src/lbaselib.cpp \
    $$PWD/lua-5.3.3/src/lbitlib.cpp \
    $$PWD/lua-5.3.3/src/lcode.cpp \
    $$PWD/lua-5.3.3/src/lctype.cpp \
    $$PWD/lua-5.3.3/src/ldblib.cpp \
    $$PWD/lua-5.3.3/src/ldebug.cpp \
    $$PWD/lua-5.3.3/src/ldo.cpp \
    $$PWD/lua-5.3.3/src/ldump.cpp \
    $$PWD/lua-5.3.3/src/lfunc.cpp \
    $$PWD/lua-5.3.3/src/lgc.cpp \
    $$PWD/lua-5.3.3/src/linit.cpp \
    $$PWD/lua-5.3.3/src/liolib.cpp \
    $$PWD/lua-5.3.3/src/llex.cpp \
    $$PWD/lua-5.3.3/src/lmathlib.cpp \
    $$PWD/lua-5.3.3/src/lmem.cpp \
    $$PWD/lua-5.3.3/src/loadlib.cpp \
    $$PWD/lua-5.3.3/src/lobject.cpp \
    $$PWD/lua-5.3.3/src/lopcodes.cpp \
    $$PWD/lua-5.3.3/src/loslib.cpp \
    $$PWD/lua-5.3.3/src/lparser.cpp \
    $$PWD/lua-5.3.3/src/lstate.cpp \
    $$PWD/lua-5.3.3/src/lstring.cpp \
    $$PWD/lua-5.3.3/src/lstrlib.cpp \
    $$PWD/lua-5.3.3/src/ltable.cpp \
    $$PWD/lua-5.3.3/src/ltablib.cpp \
    $$PWD/lua-5.3.3/src/ltm.cpp \
    $$PWD/lua-5.3.3/src/lundump.cpp \
    $$PWD/lua-5.3.3/src/lutf8lib.cpp \
    $$PWD/lua-5.3.3/src/lvm.cpp \
    $$PWD/lua-5.3.3/src/lzio.cpp \
    $$PWD/src/lua_deep_copy_table.cpp \
    $$PWD/src/lua_print_table.cpp \
    $$PWD/src/lua_default_error_function.cpp







