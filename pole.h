#ifndef POLE_H
#define POLE_H
#include<QWidget>
const int MAX_DISKS = 8;
class disk;//forward declaration
class pole :public QWidget{
    //Q_OBJECT
    int index;  //pole number
    int numDisks;   //currently on pole
    disk* stack[MAX_DISKS];//stack of disks on this pole.

public:
    pole(int index,int count, QWidget *);
    ~pole();
    disk* take();//returns the top disk on the pole
    bool put(disk* d);//puts a disk onto a pole.
                        //return false means size is wrong or pointer NULL.

    int getIndex(){return index;}
    int getNumDisks(){return numDisks;}
    void paintEvent(QPaintEvent * event);
};
#endif // POLE_H
