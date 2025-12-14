#ifndef CHESS_PIECE_MOVES_H
#define CHESS_PIECE_MOVES_H

#pragma once
#include <vector>

std::vector<std::pair<int, int>> getValidMoves(int board[8][8], int row, int col);
std::vector<std::pair<int, int>> get_pawn_moves(int board[8][8], int row, int col, bool is_white);
std::vector<std::pair<int, int>> get_rook_moves(int board[8][8], int row, int col, bool is_white);
std::vector<std::pair<int, int>> get_knight_moves(int board[8][8], int row, int col, bool is_white);
std::vector<std::pair<int, int>> get_bishop_moves(int board[8][8], int row, int col, bool is_white);
std::vector<std::pair<int, int>> get_queen_moves(int board[8][8], int row, int col, bool is_white);
std::vector<std::pair<int, int>> get_king_moves(int board[8][8], int row, int col, bool is_white);
bool is_square_attacked(int board[8][8], int row, int col, bool by_white);
bool can_castle (int board[8][8], bool is_white, bool queenside, bool can_castle_queenside, bool can_castle_kingside);

#endif //CHESS_PIECE_MOVES_H