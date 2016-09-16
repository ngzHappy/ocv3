﻿#include "MainWindow.hpp"
#include <QApplication>
#include <QCustomPlot.hpp>
#include <memory/MemoryLibrary.hpp>
#include <lua/lua.hpp>

#include <lua/src/deep_copy_table.hpp>
#include <lua/src/print_table.hpp>
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

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    /*Test for main Window*/
    MainWindow window;
    window.show();

    lua::State * L=luaL::newstate();
    test_lua(L);
    lua::close(L);

    return app.exec();
}