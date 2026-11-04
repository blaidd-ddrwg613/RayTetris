#include "Tetromino.h"

Tetromino::Tetromino(PieceTypes::PieceType type) : type(type) { }

void Tetromino::MoveRight()
{
    x++;
}

void Tetromino::MoveLeft()
{
    x--;
}

void Tetromino::MoveDown()
{
    y++;
}

// dir: +1 = clockwise, -1 = counter-clockwise.
// The double % keeps rotation in 0..3 for any dir, because % in C++
// can return a negative number when the left side is negative.
void Tetromino::Rotate(int dir)
{
    rotation = ((rotation + dir) % 4 + 4) % 4;
}

