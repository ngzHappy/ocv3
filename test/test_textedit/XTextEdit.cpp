#include "XTextEdit.hpp"
#include <QtGui/qpainter.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qpointer.h>
#include <QtCore>

constexpr int MyPaintObjectID=QTextFormat::UserObject+1;
constexpr int MyPaintPropertyID=1;

XTextEdit::XTextEdit(QWidget * var):QTextEdit(var) {
    this->resize(768,768);
    register_();

    insertPlainText("12345\n");
    insertPlainText("12345\n");
    insertPlainText("12345\n");
    insertPlainText("12345\n");
    insertPlainText("12345\n");
    insertPlainText("12345\n");
    insertPlainText("12345");
    insertMyPaint();
    for (int i=0; i<1000; ++i) {
        insertPlainText("\n12345");
    }
}

XTextEdit::~XTextEdit() {

}

void XTextEdit::insertMyPaint() {

    QTextCharFormat myPaint;
    myPaint.setObjectType(MyPaintObjectID);

    auto tmp=MyPaint::instance(this);
    auto varMyPaint=QVariant::fromValue(tmp);
    myPaint.setProperty(MyPaintPropertyID,varMyPaint);

    QTextCursor cursor=textCursor();
    cursor.insertText(QString(QChar::ObjectReplacementCharacter),myPaint);
    setTextCursor(cursor);

}

void XTextEdit::register_() {

    auto varDocument=this->document();
    auto varLayout=varDocument->documentLayout();

    auto varMyPaint=new MyPaintTextObject;
    varMyPaint->setParent(this);

    varLayout->registerHandler(MyPaintObjectID,varMyPaint);

}

MyPaintTextObject::MyPaintTextObject() {
     
}

MyPaintTextObject::~MyPaintTextObject() {

}

void MyPaintTextObject::drawObject(
    QPainter *painter,
    const QRectF &rect,
    QTextDocument *doc,
    int posInDocument,
    const QTextFormat &format) {

    auto varMyPaint=format.property(MyPaintPropertyID).value<QSharedPointer<MyPaint>>();
    varMyPaint->setVisible(true);
    
    auto varParent=
    dynamic_cast<QTextEdit *>(varMyPaint->parentWidget());

    QTextCursor varTextCursor(doc);
    varTextCursor.setPosition(posInDocument);

    auto varTextCursorPos=varParent->cursorRect(varTextCursor);
    auto varObjectRect=rect.toRect();
    
    varMyPaint->setGeometry(
        QRect(
        varObjectRect.x(),
        varTextCursorPos.y(),
        varObjectRect.width(),
        varObjectRect.height())
    );

    return;
    (void)format;
}

QSizeF MyPaintTextObject::intrinsicSize(
    QTextDocument *doc,
    int posInDocument,
    const QTextFormat &format) {
    auto varMyPaint=format.property(MyPaintPropertyID).value<QSharedPointer<MyPaint>>();
    return varMyPaint->size();
    (void)doc; (void)posInDocument; (void)format;
}

QSharedPointer<MyPaint> MyPaint::instance(QWidget * argParent) {

    QPointer<MyPaint> varMyPaint{ new MyPaint };
    varMyPaint->setParent(argParent);

    return{ varMyPaint.data(),
        [varMyPaint](MyPaint*arg) {
        if (varMyPaint) { 
            arg->setVisible(false);
            arg->deleteLater();
        }
    } };

}

void MyPaint::paintEvent(QPaintEvent * ) {

    QPainter painter(this);
    painter.fillRect(rect(),QColor(123,222,123));
    
}



