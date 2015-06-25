#include "docklayout.h"

DockLayout::DockLayout(QWidget *parent) :
    QWidget(parent)
{
}

void DockLayout::setParent(QWidget *parent)
{
    this->setParent(parent);
}

void DockLayout::addItem(AppItem *item)
{
    insertItem(item,appList.count());
}

void DockLayout::insertItem(AppItem *item, int index)
{
    item->setParent(this);
    int appCount = appList.count();
    index = index > appCount ? appCount : (index < 0 ? 0 : index);

    appList.insert(index,item);
    connect(item, SIGNAL(mouseMove(int,int,AppItem*)),this,SLOT(slotItemDrag(int,int,AppItem*)));
    connect(item,SIGNAL(mouseRelease(int,int,AppItem*)),this,SLOT(slotItemRelease(int,int,AppItem*)));
    connect(item,SIGNAL(mouseEntered(AppItem*)),this,SLOT(slotItemEntered(AppItem*)));
    connect(item, SIGNAL(mouseExited(AppItem*)),this,SLOT(slotItemExited(AppItem*)));

    relayout();
}

void DockLayout::removeItem(int index)
{
    delete appList.takeAt(index);
}

void DockLayout::moveItem(int from, int to)
{
    appList.move(from,to);
    relayout();
}

void DockLayout::setItemMoveable(int index, bool moveable)
{
    appList.at(index)->setMoveable(moveable);
}

void DockLayout::setMargin(qreal margin)
{
    this->leftMargin = margin;
    this->rightMargin = margin;
    this->topMargin = margin;
    this->bottomMargin = margin;
}

void DockLayout::setMargin(DockLayout::MarginEdge edge, qreal margin)
{
    switch(edge)
    {
    case DockLayout::LeftMargin:
        this->leftMargin = margin;
        break;
    case DockLayout::RightMargin:
        this->rightMargin = margin;
        break;
    case DockLayout::TopMargin:
        this->topMargin = margin;
        break;
    case DockLayout::BottomMargin:
        this->bottomMargin = margin;
        break;
    default:
        break;
    }
}

void DockLayout::setSpacing(qreal spacing)
{
    this->itemSpacing = spacing;
}

void DockLayout::setSortDirection(DockLayout::Direction value)
{
    this->sortDirection = value;
}

void DockLayout::relayout()
{
    switch (sortDirection)
    {
    case LeftToRight:
        sortLeftToRight();
        break;
    case RightToLeft:
        sortRightToLeft();
        break;
    case TopToBottom:
        sortTopToBottom();
        break;
    case BottomToTop:
        sortBottomToTop();
        break;
    default:
        break;
    }
}

void DockLayout::dragoutFromLayout(int index)
{
    AppItem * tmpItem = appList.takeAt(index);
    tmpItem->setVisible(false);

    if (index == appList.count())//note,target hast been remove before
    {
        qWarning() << "out of range...";
        return;//at the end of list
    }

    //move follow item,note,target hast been remove before
    AppItem * followItem = appList.at(index);
    followItem->setNextPos(followItem->x() - tmpItem->width() - itemSpacing,0);
    //move last item
    for (int i = index + 1; i < appList.count(); i ++)
    {
        AppItem * frontItem = appList.at(i - 1);
        AppItem * targetItem = appList.at(i);
        targetItem->setNextPos(frontItem->getNextPos().x() + frontItem->width() + itemSpacing,0);
    }

    for (int i = index; i < appList.count(); i ++)
    {
        AppItem *button= appList.at(i);
        QPropertyAnimation *animation = new QPropertyAnimation(button, "pos");
        animation->setStartValue(button->pos());
        animation->setEndValue(button->getNextPos());
        animation->setDuration(500 + i * 100);
        animation->setEasingCurve(QEasingCurve::InOutBack);

        animation->start();
    }

}

void DockLayout::sortLeftToRight()
{
    if (appList.count() <= 0)
        return;

    appList.at(0)->move(0,0);

    for (int i = 1; i < appList.count(); i ++)
    {
        AppItem * frontItem = appList.at(i - 1);
        appList.at(i)->move(frontItem->pos().x() + frontItem->width() + itemSpacing,0);
    }
}

void DockLayout::sortRightToLeft()
{

}

void DockLayout::sortTopToBottom()
{

}

void DockLayout::sortBottomToTop()
{

}

int DockLayout::indexOf(AppItem *item)
{
    return appList.indexOf(item);
}

int DockLayout::indexOf(int x, int y)
{
    //TODO
    return 0;
}

void DockLayout::slotItemDrag(int x, int y, AppItem *item)
{
//    qWarning() << "Item draging..."<<x<<y<<item;
    int tmpIndex = indexOf(item);
    if (tmpIndex != -1)
    {
        dragoutFromLayout(tmpIndex);
    }
}

void DockLayout::slotItemRelease(int x, int y, AppItem *item)
{
    //outside frame,destroy it
    //inside frame,insert it
    item->setVisible(true);
    if (indexOf(item) == -1)
    {
        qWarning() << "---------" << lastHoverIndex;
        insertItem(item,lastHoverIndex);
    }
}

void DockLayout::slotItemEntered(AppItem *item)
{
    this->lastHoverIndex = indexOf(item);
}

void DockLayout::slotItemExited(AppItem *item)
{

}