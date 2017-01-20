#ifndef MOVE_H
#define MOVE_H

class Move
{
    int from;
    int to;

public:
    Move(int, int);
    Move();

    int getFrom() const {return from;}
    int getTo() const {return to;}
};

#endif // MOVE_H
