#include <QPainter>
#include "disk.h"
#include "pole.h"

extern float scale;

/*
 *constructor of the class pole
 */
pole::pole(int index, int numDisks, QWidget * parent)
{
    this->index = index;
    this->numDisks = numDisks;
    this->setParent(parent);
    move(0,0);
    resize(scale*120,scale*240);
    for(int i=0;i<numDisks;i++){
        disk* d = new disk(numDisks-i,i,this, parent);
        stack[i] = d;
    }
    setEnabled(true);
    show();
    raise();
}

/*
 * destructor of the class pole
 */
pole::~pole(){
    setParent(NULL);
    for (int i=0;i<numDisks;i++){
    delete stack[i];
    }
}

/*
 * paint one pole.
 * pole's height is determined by the number of disks
 */
void pole::paintEvent(QPaintEvent *){
    QPainter p(this);
    QColor poleColor(250, 180, 50);
    p.setRenderHint(QPainter::Antialiasing);
    resize(scale *120, scale *240);

    p.setPen(Qt::NoPen);
    p.setBrush(poleColor);
    p.scale(scale,scale);
    p.drawRoundRect(57,114,6,100-numDisks*8,3,2);

}

/*
 * take the top disk on the pole
 * then return the current top disk on the pole, otherwise NULL
 */
disk* pole::take()
{
    disk *d = NULL;
    if(numDisks >0){
        d = stack[--numDisks];
        stack[numDisks] = NULL;
        d->Lift();
    }
    return d;
}

/*
 * put the disk on the pole
 * success return true
 * false means size is wrong or pointer NULL.
 */
bool pole::put(disk* d)
{

    if(numDisks == 0){
        stack[0] = d;
        numDisks++;
        d->setPolePos(this,0);
        raise();
        return true;
    }else{
        if(stack[numDisks-1]->Size() < d->Size()){
            return false;
        }else{
            stack[numDisks] = d;
            numDisks++;
            d->setPolePos(this,numDisks-1);
            raise();
            return true;
        }
    }
}

