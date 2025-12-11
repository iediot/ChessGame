#ifndef CHESS_PIECE_MOVES_H
#define CHESS_PIECE_MOVES_H

#pragma once
#include <vector>

std::vector<std::pair<int, int>> getValidMoves(int board[8][8], int row, int col);
std::vector<std::pair<int, int>> getPawnMoves(int board[8][8], int row, int col, bool isWhite);

#endif //CHESS_PIECE_MOVES_H