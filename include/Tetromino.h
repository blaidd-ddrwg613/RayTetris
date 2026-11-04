#pragma once
#include "Tetrominoes.h"

class Tetromino
{
public:
    explicit Tetromino(PieceTypes::PieceType type);
    void MoveRight();
    void MoveLeft();
    void MoveDown();
    void Rotate(int dir);

    // Small getters defined in the class are automatically inline.
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetRotation() const { return rotation; }
    PieceTypes::PieceType GetType() const { return type; }

private:
    // Top Left Corner of the 4x4 Box on the grid
    int x = 3;
    int y = 0;
    int rotation = 0;
    PieceTypes::PieceType type;
};
