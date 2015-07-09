#ifndef ABSTRACTDOCKITEM_H
#define ABSTRACTDOCKITEM_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include "Widgets/appicon.h"
#include "Widgets/arrowrectangle.h"

class AbstractDockItem : public QFrame
{
    Q_OBJECT
public:
    explicit AbstractDockItem(QWidget *parent = 0) :
        QFrame(parent) {}
    virtual ~AbstractDockItem() {}

    virtual QWidget * getContents() { return NULL; }

    virtual bool moveable() { return m_moveable; }
    virtual bool actived() { return m_isActived; }

    void resize(int width,int height){
        QFrame::resize(width,height);
        emit widthChanged();
    }
    void resize(const QSize &size){
        QFrame::resize(size);
        emit widthChanged();
    }

    QPoint getNextPos() { return m_itemNextPos; }
    void setNextPos(const QPoint &value) { m_itemNextPos = value; }
    void setNextPos(int x, int y) { m_itemNextPos.setX(x); m_itemNextPos.setY(y); }

    int globalX(){return mapToGlobal(QPoint(0,0)).x();}
    int globalY(){return mapToGlobal(QPoint(0,0)).y();}
    QPoint globalPos(){return mapToGlobal(QPoint(0,0));}

    void showPreview(){
        QWidget *tmpContent = getContents();
        QSize tmpSize(tmpContent->width() + m_previewAR->getMargin() * 2,
                      tmpContent->height() + m_previewAR->getMargin() * 2 + m_previewAR->getArrowHeight());
        m_previewAR->setMinimumSize(tmpSize);
        m_previewAR->resize(tmpSize);
        m_previewAR->setContent(tmpContent);
        m_previewAR->showAtBottom(globalX() + width() / 2,globalY() - 5);
    }

    void hidePreview(int interval = 100){
        m_previewAR->delayHide(interval);
    }


signals:
    void dragStart();
    void dragEntered(QDragEnterEvent * event);
    void dragExited(QDragLeaveEvent * event);
    void drop(QDropEvent * event);
    void mouseEntered();
    void mouseExited();
    void mousePress(int x, int y);
    void mouseRelease(int x, int y);
    void mouseDoubleClick();
    void widthChanged();

protected:

    bool m_moveable = true;
    bool m_isActived = false;
    ArrowRectangle *m_previewAR = new ArrowRectangle();

    QPoint m_itemNextPos;
};

#endif // ABSTRACTDOCKITEM_H
