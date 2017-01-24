#include <QQueue>
#include <QStack>
#include <QTimer>
#include <QKeyEvent>

#include "tower.h"
#include "ui_tower.h"
#include "disk.h"
#include "pole.h"
#include "move.h"
/*
 * shortcuts
 * key N->New
 * key S->setDisks
 * key U->undo
 * key A->undoAll
 * key P->Auto play
 */

float scale =2.0;
disk *moving = NULL;

tower::tower(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tower)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    value = ui->spinBox->value();
    poles[0] = new pole(0, value,ui->pushButton_0);
    poles[1] = new pole(1, 0,ui->pushButton_1);
    poles[2] = new pole(2, 0,ui->pushButton_2);
    ui->spinBox->hide();
    ui->label->hide();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(delayedAction()));

    autoplay = false;

    //Sandy - update
    updateUndoStatus();
}

tower::~tower()
{
    delete ui;
    for (int i=0;i<3;i++){
        delete poles[i];
    }
    moving = NULL;
    undoStack.clear();
    schedule.clear();
}

void tower::resizeEvent(QResizeEvent *){

    scale = qMin(width()/(360.0), height()/(300.0));
    ui->pushButton_0->resize(scale * 120, scale * 300);
    ui->pushButton_1->resize(scale * 120, scale * 300);
    ui->pushButton_2->resize(scale * 120, scale * 300);

    ui->pushButton_1->move(scale * 120, 0);
    ui->pushButton_2->move(scale * 240,0);
}
/*
 * This function is for mouse free to control the game
 * override the keyPressEvent to process keyboard event
 * key 1 -> pole 0
 * key 2 -> pole 1
 * key 3 -> pole 2
 */
void tower::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_1:
        on_pushButton_clicked(poles[0]);
        break;
    case Qt::Key_2:
        on_pushButton_clicked(poles[1]);
        break;
    case Qt::Key_3:
        on_pushButton_clicked(poles[2]);
        break;
    default:
        break;
    }
}

void tower::on_pushButton_clicked(pole *p)
{
    if(!moving){
        moving = p->take();
    }else{
        int from = moving->On()->getIndex();
        if(p->put(moving)){
            Move mov(from,p->getIndex());
            undoStack.push(mov);
            moving = NULL;
        }else{
            moving->On()->put(moving);
            moving = NULL;
        }
    }
    //Sandy - update
    updateUndoStatus();
}

void tower::on_pushButton_0_clicked()
{
    on_pushButton_clicked(poles[0]);
}

void tower::on_pushButton_1_clicked()
{
    on_pushButton_clicked(poles[1]);
}

void tower::on_pushButton_2_clicked()
{
    on_pushButton_clicked(poles[2]);
}

void tower::on_actionNew_triggered()
{
    for (int i=0;i<3;i++){delete poles[i]; }
    if(moving)delete moving;
    moving = NULL;
    undoStack.clear();
    schedule.clear();

    //Sandy - update
    updateUndoStatus();

    poles[0] = new pole(0, value,ui->pushButton_0);
    poles[1] = new pole(1, 0,ui->pushButton_1);
    poles[2] = new pole(2, 0,ui->pushButton_2);
    ui->spinBox->hide();
}

void tower::on_actionSet_Disks_triggered()
{
    ui->spinBox->show();
    ui->label->show();
}

void tower::on_actionExit_triggered()
{
    close();
}

void tower::on_actionUndo_triggered()
{
    if(!undoStack.empty()){
        Move undo = undoStack.pop();
        pole* to = NULL;
        pole* from = NULL;
        to = poles[undo.getTo()];
        from = poles[undo.getFrom()];
        if(moving){
            moving->On()->put(moving);
            moving = NULL;
        }
        moving = to->take();
        from->put(moving);
        moving = NULL;
    }
    //Sandy - update
    updateUndoStatus();
}

void tower::on_actionUndo_All_triggered()
{
    autoplay=false;
    timer->start(33);
    //Sandy - update
    updateUndoStatus();
    //ui->actionUndo->setDisabled(true);
}

void tower::CalculateSchedule(int count, int from, int to, int spare)
{
    if(count < 1) return;
    CalculateSchedule(count-1, from, spare, to);
    schedule.enqueue(Move(from, to));
    CalculateSchedule(count-1, spare, to, from);
}

void tower::on_actionAuto_Play_triggered()
{
    on_actionNew_triggered();
    CalculateSchedule(poles[0]->getNumDisks(),0,2,1);
    timer->start(1000); // 1 second
    autoplay = true;
    ui->pushButton_0->setDisabled(true);
    ui->pushButton_1->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    //Sandy - update
    updateUndoStatus();
}

void tower::on_spinBox_valueChanged(int arg1)
{
    value = arg1;
}

void tower::delayedAction()
{
    if(autoplay){
        if(!schedule.empty()){
            Move autom = schedule.dequeue();
            moving = poles[autom.getFrom()]->take();
            poles[autom.getTo()]->put(moving);
            moving = NULL;
            undoStack.push(autom);
        }else{
            autoplay = false;
            ui->pushButton_0->setEnabled(true);
            ui->pushButton_1->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            timer->stop();
        }

    }else{
        if(undoStack.empty()){
            ui->pushButton_0->setEnabled(true);
            ui->pushButton_1->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            timer->stop();
        }else{
            on_actionUndo_triggered();
        }
    }
}

// Sandy - The newly added method
void tower::updateUndoStatus()
{
    if(autoplay)
    {
        ui->actionUndo->setDisabled(true);
        ui->actionUndo_All->setDisabled(false);
    }
    else
    {
        if(undoStack.empty())
        {
            ui->actionUndo->setDisabled(true);
            ui->actionUndo_All->setDisabled(true);
        }
        else
        {
            ui->actionUndo->setDisabled(false);
            ui->actionUndo_All->setDisabled(false);
        }
    }
}

bool tower::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->spinBox && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
            ui->spinBox->hide();
            ui->label->hide();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

