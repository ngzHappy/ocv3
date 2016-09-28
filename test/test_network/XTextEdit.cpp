#include "XTextEdit.hpp"
#include <QtGui/qpainter.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qpointer.h>
#include <QtCore>

constexpr int MyPaintObjectID=QTextFormat::UserObject+1;
constexpr int MyPaintPropertyID=1;

XTextEdit::XTextEdit(QWidget * var):QTextEdit(var) {
    _p_connect();
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
    for (int i=0; i<100; ++i) {
        insertPlainText("\n12345");
    }
    insertMyPaint();
    for (int i=0; i<100; ++i) {
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

void XTextEdit::_p_connect() {
    connect(this,&XTextEdit::_p_s_if_updata_once,
        this,&XTextEdit::_p_if_updata_once,Qt::QueuedConnection);
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
    

    auto varParent=
        dynamic_cast<XTextEdit *>(varMyPaint->parentWidget());

    QTextCursor varTextCursor(doc);
    varTextCursor.setPosition(posInDocument);

    auto varTextCursorPos=varParent->cursorRect(varTextCursor);
    auto varObjectRect=rect.toRect();

    QRect varAboutDraw(
        varObjectRect.x(),
        varTextCursorPos.y(),
        varObjectRect.width(),
        varObjectRect.height());
    varMyPaint->setGeometry(varAboutDraw);

    
    if (varParent->underMouse()) {
        if (varAboutDraw.contains(varParent->cursor().pos())) {
            varMyPaint->setVisible(true);
        }
    }
    
    varMyPaint->setVisible(false);
    

    QImage imagexxx(
        varAboutDraw.width(),varAboutDraw.height(),QImage::Format::Format_RGBA8888);
    imagexxx.fill(QColor(222,111,111));
    painter->drawImage(rect,imagexxx);

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
    varMyPaint->setVisible(false);

    return{ varMyPaint.data(),
        [varMyPaint](MyPaint*arg) {
        if (varMyPaint) {
            arg->setVisible(false);
            arg->deleteLater();
        }
    } };

}

void MyPaint::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    painter.fillRect(rect(),QColor(123,222,123));

}

void XTextEdit::paintEvent(QPaintEvent *argEvent) {
    
    return QTextEdit::paintEvent(argEvent);
    
}

void XTextEdit::_p_if_updata_once() {

    if (_m_aboutToHidden.empty()) { return; }

    qDebug()<<"hidden event called";

    class UpdataEnableLock {
        QWidget * const _m_Widget;
        bool _m_IsUpdateEnabledBack;
    public:
        UpdataEnableLock(QWidget *var):_m_Widget(var) {
            if (_m_Widget) {
                _m_IsUpdateEnabledBack=!var->testAttribute(Qt::WA_ForceUpdatesDisabled);
            }
            _m_Widget->setUpdatesEnabled(false);
        }
        ~UpdataEnableLock() {
            if (_m_Widget) {
                _m_Widget->setUpdatesEnabled(_m_IsUpdateEnabledBack);
            }
        }
    };

    UpdataEnableLock varUpdataEnableLockThis{ this };
    UpdataEnableLock varUpdataEnableLockViewPort{ this->viewport() };

    /***************************************/
    for (auto & i:_m_aboutToHidden) {
        if (i) { i->hide(); }
    }
    _m_aboutToHidden.clear();
    /***************************************/

    if (this->viewport()) {
        this->viewport()->update();
    }
    else {
        this->update();
    }

}




