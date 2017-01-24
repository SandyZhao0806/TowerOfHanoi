#ifndef TOWER_H
#define TOWER_H
#include <QMainWindow>
#include <QQueue>
#include <QStack>

class pole;
class Move;
//forward declaration
namespace Ui {
class tower;
}
class tower : public QMainWindow
{
    Q_OBJECT

public:
    explicit tower(QWidget *parent = 0);
    ~tower();
    void resizeEvent(QResizeEvent *); // resize the window
    void keyPressEvent(QKeyEvent *event);//override this function to process keyboard event,mouse-free
private slots:
    void on_pushButton_0_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_actionNew_triggered();

    void on_actionSet_Disks_triggered();

    void on_actionExit_triggered();

    void on_actionUndo_triggered();

    void on_actionUndo_All_triggered();

    void on_actionAuto_Play_triggered();

    void on_spinBox_valueChanged(int arg1);

    void delayedAction();

    // Sandy - A new method to update the "undo", "undo all" status(enable/disable)
    // So that each time we want to update it. we just need to call this function
     void updateUndoStatus();

private:
    Ui::tower *ui;
    pole* poles[3];
    QQueue<Move> schedule;
    QStack<Move> undoStack;
    QTimer *timer;
    bool autoplay;
    int value;
    void on_pushButton_clicked(pole * p);
    void CalculateSchedule(int count, int from, int to, int spare);//... 1. argu 2. to do autopaly 3. slowly to visible

};

#endif // TOWER_H
