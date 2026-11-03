#pragma once
#include "Tetrominoes.h"

class Tetromino
{
public:
    Tetromino(PieceTypes::PieceType type);
    void MoveRight();
    void MoveLeft();
    void MoveDown();
    void Rotate(int dir);

    int GetX() const, GetY() const;
    int GetRotation() const;

    PieceTypes::PieceType GetType() const;

private:
    // Top Left Corner of the 4x4 Box on the grid
    int x = 3;
    int y = 0;
    int rotation = 0;
    PieceTypes::PieceType type;
};
