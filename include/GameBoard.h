#pragma once

#include <array>
#include "raylib.h"
#include "Tetromino.h"

class GameBoard
{
public:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;

    GameBoard() = default;          // board{} below already starts every cell empty

    // --- Queries ---------------------------------------------------------
    bool IsInsideBoard(int x, int y) const;
    bool IsCellOccupied(int x, int y) const;   // assumes IsInsideBoard(x, y)
    bool Fits(const Tetromino& piece) const;

    // --- Changes ---------------------------------------------------------
    void PlacePiece(const Tetromino& piece);   // lock a piece into the board
    int  ClearLines();                         // returns how many rows were cleared
    void Reset();                              // empty the whole board

    // --- Drawing ---------------------------------------------------------
    // originX/originY: top-left pixel of the board on screen.
    void Draw(int originX, int originY, int cellSize) const;

    // Draws one grid cell. Static so Game can reuse it for the falling piece,
    // the ghost piece and the next-piece preview.
    static void DrawCell(int gridX, int gridY, Color color,
                         int originX, int originY, int cellSize);

private:
    bool IsRowFull(int y) const;

    // board[y][x]: row first, then column.
    // 0 = empty, otherwise (PieceType + 1) so the color is PIECE_COLORS[cell - 1].
    std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> board {};
};