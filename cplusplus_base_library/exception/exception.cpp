#include "exception.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "../lua/lua.hpp"

namespace {
namespace __private {

class TestInt {};

std::ostream & log_stream() { return std::cout; }

class Handle {
    int memLine;
    const char * memFunctionName;
    const char * memFileName;
    int memExceptionDepth=0;

    void bad_exception_handle() {
    }

    /*add your own exception functions here*/
    void int_exception_handle(const TestInt &e) {

        try {
        }
        catch (...) {
            bad_exception_handle();
        }

        try {
            std::rethrow_if_nested(e);
        }
        catch (...) {
            exception_handle();
        }

    }

public:

    Handle(
        int argLine,
        const char * argFunctionName,
        const char * argFileName):memLine(argLine),
        memFunctionName(argFunctionName),
        memFileName(argFileName) {
    }

    void exception_handle() noexcept(true) {

        ++memExceptionDepth;
        try {
            std::rethrow_exception(std::current_exception());
        }/*add your own exception functions here*/
        catch (const TestInt &e) {
            int_exception_handle(e);
        }
        catch (...) {
            /*unknow exception handle*/
            {
                std::stringstream out;
                out<<"get a unknow exception @\n"
                    <<"from : "
                    <<"\nline : "<<memLine
                    <<"\nfileName : "<<memFileName
                    <<"\nfunctionName : "<<memFileName
                    <<"\n("<<__LINE__<<" , "<<__FILE__<<" , "<<__func__<<")"
                    <<std::endl;
                log_stream()<<out.rdbuf();
            }
            /*quit the application*/
            std::exit(0);
        }

    }

};

}/*__private*/
}/*namespace*/

namespace exception {

void exception_handle(
    int line,
    const char * functionName,
    const char * fileName) noexcept(false) {

    try {
        std::rethrow_exception(std::current_exception());
    }/*do not catched exception*/
    catch (const lua_exception_type &e) {
        throw(e);
    }
    catch (const std::bad_alloc &e) {
        __private::log_stream()<<"bad_alloc:"
            <<functionName<<":"<<fileName
            <<":"<<line;
        std::quick_exit(-1);
        throw(e);
    }
    catch (...) {
        __private::Handle handle{ line,functionName,fileName };
        handle.exception_handle();
    }

}/**/

}/**/

