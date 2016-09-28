#ifndef XTEXTEDIT_HPP
#define XTEXTEDIT_HPP

#include <QtGui/qtextobject.h>
#include <QtWidgets/qtextedit.h>
#include <QTextObjectInterface> 
#include <QtCore/qsharedpointer.h>
#include <QtCore/qpointer.h>
#include <list>
#include <set>

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
    friend class MyPaint;
    friend class MyPaintTextObject;
    std::set<QPointer<MyPaint>>_m_ThisPaint;
    std::set<QPointer<MyPaint>>_m_LastPaint;
    std::set<QPointer<MyPaint>>_m_aboutToHidden;
public:
    explicit XTextEdit(QWidget * /**/=nullptr);
    ~XTextEdit();

    void insertMyPaint();

protected:
    void register_();
    void _p_connect();
    Q_SIGNAL void _p_s_if_updata_once();
    void _p_if_updata_once();
    void paintEvent(QPaintEvent *event)override;
};

#endif // XTEXTEDIT_HPP
