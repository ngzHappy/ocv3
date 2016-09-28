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

#include <QtNetwork>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>

inline QString operator""_qs(
    const char *arg,
    std::size_t n) {
    return QString::fromUtf8(arg,int(n));
}

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    auto testString=
    u8R"__([emotion pic_type=1 width=30 height=30]http://tb2.bdstatic.com/tb/editor/images/face/i_f15.png?t=20140803[/emotion])__"_qs;

   // qDebug()<<
   // testString.toUtf8().toBase64(QByteArray::Base64UrlEncoding);

    qDebug()<<
    QUrl::toPercentEncoding(testString," ").replace(' ','+');

    QUrlQuery xxx;
    xxx.addQueryItem("a","b");
    xxx.addQueryItem("b","b b");
    xxx.addQueryItem("b"_qs,u8R"(b<今天>b)"_qs);
    auto xans= xxx.toString(
        QUrl::EncodeUnicode/**/| 
        QUrl::EncodeDelimiters/**/| 
        QUrl::EncodeReserved/**/
    );

    qDebug()<<xans;

    auto testString1=
    testString.toHtmlEscaped()
        .toUtf8().toPercentEncoding();

    qDebug()<<testString1;

    QHttpPart p;
    p.setRawHeader("content","[emotion pic_type=1 width=30 height=30]http://tb2.bdstatic.com/tb/editor/images/face/i_f15.png?t=20140803[/emotion]");

    QHttpMultiPart m;
    m.append(p);

    QNetworkAccessManager xm;
    xm.post( 
        QNetworkRequest(QUrl(QString("http://www.baidu.com"))),
        &m);

    XTextEdit edit;
    edit.show();

    return app.exec();
}
