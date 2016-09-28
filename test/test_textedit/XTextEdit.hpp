#ifndef XTEXTEDIT_HPP
#define XTEXTEDIT_HPP

#include <QtGui/qtextobject.h>
#include <QtWidgets/qtextedit.h>
#include <QTextObjectInterface> 
#include <QtCore/qsharedpointer.h>

class MyPaint :public QWidget{
    Q_OBJECT

public:
    static QSharedPointer<MyPaint> instance(QWidget * argParent);

protected:
    void paintEvent(QPaintEvent *event)override;
};

class MyPaintTextObject :public QObject,public QTextObjectInterface {
    Q_OBJECT
private:
    Q_INTERFACES(QTextObjectInterface)

public:
    MyPaintTextObject();
    ~MyPaintTextObject();
protected:

    virtual void drawObject(QPainter *painter,const QRectF &rect,QTextDocument *doc,int posInDocument,const QTextFormat &format)override;
    virtual QSizeF intrinsicSize(QTextDocument *doc,int posInDocument,const QTextFormat &format)override;


};

class XTextEdit : public QTextEdit {
    Q_OBJECT

private:

public:
    explicit XTextEdit(QWidget * /**/=nullptr);
    ~XTextEdit();

    void insertMyPaint();

protected:
    void register_();
};

#endif // XTEXTEDIT_HPP
