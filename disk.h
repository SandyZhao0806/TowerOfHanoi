#ifndef DISK_H
#define DISK_H
#include<QWidget>
class pole;//forward declaration
class disk: public QWidget
{
private:
    pole *on;
    int size; // size of disk.
    int pos; // index on pole.

    void setPolePos(pole *d, int p);
    void Lift();
    int Size(){return size;}

public:
    disk(int sz1,int position,pole*,QWidget *);
    void paintEvent(QPaintEvent *);
    pole *On(){return on;}//the disk on which pole
    friend class pole;
};

#endif // DISK_H
