#include "MainWindow.hpp"
#include <QApplication>
#include <QCustomPlot.hpp>
#include <memory/MemoryLibrary.hpp>

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

    return app.exec();
}
