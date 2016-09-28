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

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    XTextEdit edit;
    edit.show();
    
    return app.exec();
}
