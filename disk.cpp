#include <QWidget>
#include <QPainter>
#include "disk.h"
#include "pole.h"

extern float scale;
QColor diskColor(213, 133, 170);
QColor diskEgdeColor(0,0,0);
QColor diskCenterColor(100,100,0);

/*
 *constructor of the class disk
 */
disk::disk(int sz1,int position,pole *onthis,QWidget *parent)
    :size(sz1),on(onthis),pos(position){
    this->setParent(parent);
    show();
    raise();
}

/*
 * paint one disk.
 * pos determine the position and size determine the size of the disk.
 */
void disk::paintEvent(QPaintEvent *){
    QPainter painter(this);
    resize(120*scale,240*scale);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(scale,scale);
    painter.setPen(diskEgdeColor);
    painter.setBrush(diskColor);
    const int top=210;

   painter.drawEllipse(QPointF(60,top-(pos-1)*8),(size+2)*5,(size+2)*1);

   painter.setPen(Qt::NoPen);
   painter.drawRect(60-((size+2)*5),top-pos*8,(size+2)*10,8);

   painter.setPen(diskEgdeColor);
   painter.drawLine(60-((size+2)*5),top-pos*8,60-((size+2)*5),top-(pos-1)*8);
   painter.drawLine(60+((size+2)*5),top-pos*8,60+((size+2)*5),top-(pos-1)*8);
   painter.drawEllipse(QPointF(60,top-pos*8),(size+2)*5,(size+2)*1);


   painter.setBrush(diskCenterColor);
   painter.drawEllipse(QPointF(60,top-pos*8),6,2);
}

/*
 * Lift the top disk.
 */
void disk::Lift(){
    pos = 16;
    repaint();
}

/*
 * Set the disk on which pole and which position of the pole.
 */
void disk::setPolePos(pole *d, int p)
{
    if(on !=d){
        on = d;
        this->setParent(d->parentWidget());
        show();
    }
    pos = p;
    repaint();
}
