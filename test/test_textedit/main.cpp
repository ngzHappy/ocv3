#include "MainWindow.hpp"
#include <QApplication>
#include <QCustomPlot.hpp>
#include <memory/MemoryLibrary.hpp>
#include <lua/lua.hpp>

#include <lua/src/deep_copy_table.hpp>
#include <lua/src/print_table.hpp>
#include <lua/src/default_error_function.hpp>
#include <iostream>
#include "XTextEdit.hpp"

inline QString operator""_qs(
    const char *arg,
    std::size_t n) {
    return QString::fromUtf8(arg,int(n));
}

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    auto testString=
    u8R"__([emotion pic_type=1 width=30 height=30]http://tb2.bdstatic.com/tb/editor/images/face/i_f15.png?t=20140803[/emotion])__"_qs;

    auto testString1=
    testString.toHtmlEscaped()
        .toUtf8().toPercentEncoding();

    XTextEdit edit;
    edit.show();
    
    return app.exec();
}
