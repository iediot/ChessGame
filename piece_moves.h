#ifndef CHESS_PIECE_MOVES_H
#define CHESS_PIECE_MOVES_H

#pragma once
#include <vector>

std::vector<std::pair<int, int>> getValidMoves(int board[8][8], int row, int col);
std::vector<std::pair<int, int>> getPawnMoves(int board[8][8], int row, int col, bool isWhite);
std::vector<std::pair<int, int>> getRookMoves(int board[8][8], int row, int col, bool isWhite);
std::vector<std::pair<int, int>> getKnightMoves(int board[8][8], int row, int col, bool isWhite);
std::vector<std::pair<int, int>> getBishopMoves(int board[8][8], int row, int col, bool isWhite);
std::vector<std::pair<int, int>> getQueenMoves(int board[8][8], int row, int col, bool isWhite);
std::vector<std::pair<int, int>> getKingMoves(int board[8][8], int row, int col, bool isWhite);

#endif //CHESS_PIECE_MOVES_H