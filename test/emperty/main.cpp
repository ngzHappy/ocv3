#include "MainWindow.hpp"
#include <QApplication>
#include <QCustomPlot.hpp>
#include <memory/MemoryLibrary.hpp>
#include <lua/lua.hpp>

#include <lua/src/deep_copy_table.hpp>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    /*Test for main Window*/
    MainWindow window;
    window.show();

    /*Test for qcustomplot*/
    QCustomPlot plot;
    plot.resize(512,512);
    plot.show();

    {
        memory::constructStatic();
        auto tmp=memory::malloc(12);
        memory::free(tmp);
    }

    {
        auto L=luaL_newstate();
        luaL_openlibs(L);

        lua_newuserdata(L,32);
        
        lua::pushcfunction(L,&luaL::function_deep_copy_table);
        lua::setglobal(L,"deep_copy_table");

        luaL::dostring(L,u8R"____(

--deep_copy_table(1,2)

print("-----------")

a={1,2,3,4,5,}
a.b={4,5,6,7,8}
a.m={1,2,3,4}
a.b.c={8,9,0,12,3}
a.t=a.b
a.n=a.b

b={}
deep_copy_table(a,b)

a.b.c[1]=110

for k,v in pairs(b) do
    print(k,v)
end

print("-----------")

for k,v in pairs(a) do
    print(k,v)
end

print("-----------")
)____");

        //lua::call(L,0,lua::MULTRET);

        lua_close(L);
    }

    return app.exec();
}
