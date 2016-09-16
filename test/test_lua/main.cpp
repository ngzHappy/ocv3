#include "MainWindow.hpp"
#include <QApplication>
#include <QCustomPlot.hpp>
#include <memory/MemoryLibrary.hpp>
#include <lua/lua.hpp>

#include <lua/src/deep_copy_table.hpp>
#include <lua/src/print_table.hpp>
#include <lua/src/default_error_function.hpp>
#include <iostream>

class XStatePack{
public:
    int &a;
    double &b;
    float &c;
};

int error_test(lua::State *L){

    std::size_t length;
    auto * ans=luaL::tolstring(L,-1,&length);
    if (length&&ans) {
        std::cout.write(ans,length);
        std::cout<<std::endl;
    }
    else {
        std::cout<<"unknow error"<<std::endl;
    }

    return 0;
}

int l_x_test(lua::State *L){

    auto pack=
    reinterpret_cast<XStatePack *>( lua::touserdata(L,2) );

    std::cout<<pack->a<<std::endl;
    std::cout<<pack->b<<std::endl;
    std::cout<<pack->c<<std::endl;

    lua::pushlstring(L,"test error");
    lua::error(L);

    return 0;
}

void x_test(lua::State *L,int a,double b,float c){
    XStatePack pack{a,b,c};

    {
        lua::pushcfunction(L,&l_x_test);

        lua::pushcfunction(L,&error_test);
        auto error_function_pos=lua::gettop(L);

        lua::pushlightuserdata(L,&pack);

        std::cout<<std::boolalpha<<
        (lua::pcall(L,2,lua::MULTRET,error_function_pos)==lua::OK)<<std::endl;
    }
}

void test_lua(lua::State * L){
    x_test(L,1,2,3);
}

int error_test1(lua::State *L) {
    class TestStack {
        lua::State *L_;
    public:
        TestStack(lua::State *L__):L_(L__) {

        }
        ~TestStack() { 
            std::cout<<"---c"<<std::endl;
            lua::settop(L_,0);
        }
    };

    TestStack stack{L};
    lua::pushlstring(L,"test error1");
    lua::error(L);

    return 0;
}

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    /*Test for main Window*/
    MainWindow window;
    window.show();
    
    {
        luaL::error(0,0);
    }

    {
        lua::State * L=luaL::newstate();
        lua::pushcfunction(L,&error_test1);
        lua::pushcfunction(L,luaL::default_lua_error_function);
        lua::pcall(L,1,lua::MULTRET,0);
        luaL::default_lua_error_function(L);
        lua::close(L);
    }

    {
        lua::State * L=luaL::newstate();
        test_lua(L);
        lua::close(L);
    }

    {
        lua::State * L=luaL::newstate();
        lua::openlibs(L);
        
        lua::pushcfunction(L,&luaL::function_print_table);
        lua::setglobal(L,"print_table");

        lua::pushcfunction(L,&luaL::function_table_tostring);
        lua::setglobal(L,"table_tostring");

        luaL::dostring(L,u8R"___(

(function() print("abc") end)()

xxx ={ 1 ,2, 3, z = {1.1,2.2,3.3, y ={4,5,6}},4,function()  end ,{7,9.999}}

xxx.m = xxx.z
xxx.n = xxx

print_table(xxx)

--print_table(table)

--for k,v in pairs(_G) do
--    print(k,v)
--end

 
xans = load( table_tostring(xxx) )()  

print( xans[1] )

 )___");
             
        lua::close(L);
    }

    return app.exec();
}
