#ifndef _m_EXCEPTION__HPP_BASE__
#define _m_EXCEPTION__HPP_BASE__() 1

#include <exception>
#include <stdexcept>
#include "../cplusplus_base_library_global.hpp"

#ifndef HANDLE_EXCEPTION_EXPORT
#define HANDLE_EXCEPTION_EXPORT CPLUSPLUS_BASE_LIBRARYSHARED_EXPORT
#endif

namespace exception {

/*
throw : lua_exception_type
*/
HANDLE_EXCEPTION_EXPORT void exception_handle(
    int line,
    const char * functionName,
    const char * fileName) noexcept(false);

}

#ifndef exception_catched
#define exception_catched() exception::exception_handle(__LINE__,__func__,__FILE__)
#endif 

#endif

