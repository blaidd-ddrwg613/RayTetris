#pragma once

#include <array>
#include "Tetromino.h"

class GameBoard
{
public:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;

    GameBoard();

    void Draw() const;
    bool IsCellOccupied(int x, int y) const;
    bool IsInsideBoard(int x, int y) const;
    int ClearLines();
    void PlacePiece(const Tetromino& piece);
    bool Fits(const Tetromino& piece) const;

private:
    std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> board {};
};