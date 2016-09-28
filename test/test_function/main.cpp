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

#include <utility/StaticStack.hpp>
#include <atomic>
#include <tuple>
class FunctionStack {

    class Step0 {public:
        static bool run(FunctionStack *) {} 
    };

    class Step1 {public:
        static bool run(FunctionStack *) {}
    };

    class Step2 {public:static 
        bool run(FunctionStack *) {}
    };

    class Step3 {
    public:static bool run(FunctionStack *) {}
    };

    using Steps=std::tuple<Step0,Step1,Step2,Step3>  ;
    bool run_step(int n) {
#undef run_step_n
#define run_step_n(_n_) case (_n_):return std::tuple_element_t<(_n_),Steps>::run(this);break
        
        switch (n) {
            run_step_n(0);
            run_step_n(1);
            run_step_n(2);
            run_step_n(3);
        }
        return false;

#undef run_step_n
    }

    int _m_step=0;
    std::atomic<bool>_m_is_finished=false;
    std::atomic<bool> _m_is_error=false;
public:
    
    bool isFinished()const { return _m_is_finished.load(); }
    bool hasError() const { return _m_is_error.load(); }

    void run() {
        if (_m_is_finished) { return; }
        auto is_ok_=run_step(_m_step);
        if (is_ok_) { ++_m_step; return; }
        else {
            _m_is_finished=true;
            if (_m_is_error) {}
        }
    }

};

void xxx() {
}

#include <QtCore/qfile.h>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    {
        QFile file_("test.txt");
        file_.open(QIODevice::WriteOnly);
        file_.resize(1024*1024);
    }

    /*Test for main Window*/
    MainWindow window;
    window.show();

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
