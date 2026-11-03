#include "GameBoard.h"

GameBoard::GameBoard()
{
}

void GameBoard::Draw() const
{
}

// Assumes (x, y) is inside the board; check IsInsideBoard() first.
bool GameBoard::IsCellOccupied(int x, int y) const
{
    return board[y][x] != 0;
}

bool GameBoard::IsInsideBoard(int x, int y) const
{
    return x >= 0 && x < BOARD_WIDTH &&
           y >= 0 && y < BOARD_HEIGHT;
}

int GameBoard::ClearLines()
{
}

void GameBoard::PlacePiece(const Tetromino& piece)
{
}

// True if every filled cell of the piece is on an empty cell of the board.
// Cells above the top of the board (y < 0) are allowed, as long as they are
// between the walls, so pieces can spawn and rotate partly off-screen.
bool GameBoard::Fits(const Tetromino& piece) const
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (!pieceCell(piece.GetType(), piece.GetRotation(), row, col))
                continue;

            int boardX = piece.GetX() + col;
            int boardY = piece.GetY() + row;

            if (boardY < 0)
            {
                // Above the visible board: only the side walls matter.
                if (boardX < 0 || boardX >= BOARD_WIDTH)
                    return false;
                continue;
            }

            if (!IsInsideBoard(boardX, boardY) || IsCellOccupied(boardX, boardY))
                return false;
        }
    }
    return true;
}
