#include "MainWindow.hpp"
#include <QApplication>
#include <QCustomPlot.hpp>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    /*Test for main Window*/
    MainWindow window;
    window.show();

    /*Test for qcustomplot*/
    QCustomPlot plot;
    plot.resize(512,512);
    plot.show();

    return app.exec();
}
