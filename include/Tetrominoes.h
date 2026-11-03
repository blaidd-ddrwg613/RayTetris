#pragma once
#include "raylib.h"

// The 7 tetromino types. The order matches the PIECES and PIECE_COLORS tables.
namespace PieceTypes
{
  enum PieceType { I = 0, O, T, S, Z, J, L, PIECE_COUNT };
}



// PIECES[type][rotation][row][col]
//  - 4 rotations per piece, in clockwise order (0 -> 1 -> 2 -> 3 -> 0)
//  - each rotation is a 4x4 grid: 1 = filled cell, 0 = empty
//  - shapes follow the standard (SRS) layout used by modern Tetris
//
// Rotate clockwise:         rotation = (rotation + 1) % 4
// Rotate counter-clockwise: rotation = (rotation + 3) % 4
constexpr int PIECES[PIECE_COUNT][4][4][4] = {
    // I
    {
        {{0,0,0,0},
         {1,1,1,1},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,0,1,0},
         {0,0,1,0},
         {0,0,1,0},
         {0,0,1,0}},

        {{0,0,0,0},
         {0,0,0,0},
         {1,1,1,1},
         {0,0,0,0}},

        {{0,1,0,0},
         {0,1,0,0},
         {0,1,0,0},
         {0,1,0,0}},
    },
    // O (identical in every rotation)
    {
        {{0,1,1,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,1,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,1,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,1,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},
    },
    // T
    {
        {{0,1,0,0},
         {1,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,0,0},
         {0,1,1,0},
         {0,1,0,0},
         {0,0,0,0}},

        {{0,0,0,0},
         {1,1,1,0},
         {0,1,0,0},
         {0,0,0,0}},

        {{0,1,0,0},
         {1,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},
    },
    // S
    {
        {{0,1,1,0},
         {1,1,0,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,0,0},
         {0,1,1,0},
         {0,0,1,0},
         {0,0,0,0}},

        {{0,0,0,0},
         {0,1,1,0},
         {1,1,0,0},
         {0,0,0,0}},

        {{1,0,0,0},
         {1,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},
    },
    // Z
    {
        {{1,1,0,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,0,1,0},
         {0,1,1,0},
         {0,1,0,0},
         {0,0,0,0}},

        {{0,0,0,0},
         {1,1,0,0},
         {0,1,1,0},
         {0,0,0,0}},

        {{0,1,0,0},
         {1,1,0,0},
         {1,0,0,0},
         {0,0,0,0}},
    },
    // J
    {
        {{1,0,0,0},
         {1,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,1,0},
         {0,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},

        {{0,0,0,0},
         {1,1,1,0},
         {0,0,1,0},
         {0,0,0,0}},

        {{0,1,0,0},
         {0,1,0,0},
         {1,1,0,0},
         {0,0,0,0}},
    },
    // L
    {
        {{0,0,1,0},
         {1,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},

        {{0,1,0,0},
         {0,1,0,0},
         {0,1,1,0},
         {0,0,0,0}},

        {{0,0,0,0},
         {1,1,1,0},
         {1,0,0,0},
         {0,0,0,0}},

        {{1,1,0,0},
         {0,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},
    },
};

// Standard guideline colors, indexed by PieceType.
// Tip: store (type + 1) in your board cells so 0 can mean "empty",
// then draw a locked cell with PIECE_COLORS[cell - 1].
inline constexpr Color PIECE_COLORS[PieceTypes::PIECE_COUNT] = {
    SKYBLUE,  // I
    YELLOW,   // O
    PURPLE,   // T
    GREEN,    // S
    RED,      // Z
    BLUE,     // J
    ORANGE,   // L
};

// Convenience helper: is cell (row, col) of this piece/rotation filled?
constexpr bool pieceCell(int type, int rotation, int row, int col) {
    return PIECES[type][rotation][row][col] != 0;
}