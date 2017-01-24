#include "tower.h"
#include <QApplication>
/*
 * shortcuts
 * key N->New (Please type tab if you want to a new game after type S for setDisks)
 * key S->setDisks
 * key U->undo
 * key A->undoAll
 * key P->Auto play
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon.png"));
    tower w;
    w.show();

    return a.exec();
}
