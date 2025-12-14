//
// Created by edi on 12/11/25.
//

#include "piece_moves.h"

#include <vector>
#include <cmath>
#include <csetjmp>

std::vector<std::pair<int, int>> getValidMoves(int board[8][8], int row, int col) {
    int piece = board[row][col];

    switch (abs(piece))
    {
    case 1:
        return get_pawn_moves(board, row, col, piece > 0);
    case 2:
        return get_rook_moves(board, row, col, piece > 0);
    case 3:
        return get_knight_moves(board, row, col, piece > 0);
    case 4:
        return get_bishop_moves(board, row, col, piece > 0);
    case 5:
        return get_queen_moves(board, row, col, piece > 0);
    case 6:
        return get_king_moves(board, row, col, piece > 0);
    default:
        return {};
    }
}

std::vector<std::pair<int, int>> get_pawn_moves(int board[8][8], int row, int col, bool is_white) {
    std::vector<std::pair<int, int>> valid_moves;

    if (is_white) {
        if (row - 1 >= 0 && board[row - 1][col] == 0) {
            valid_moves.emplace_back(row - 1, col);
            if (row == 6 && board[row - 2][col] == 0)
                valid_moves.emplace_back(row - 2, col);
        }
        if (col > 0 && row - 1 >= 0 && board[row - 1][col - 1] < 0)
                valid_moves.emplace_back(row - 1, col - 1);
        if (col < 7 && row - 1 >= 0 && board[row - 1][col + 1] < 0)
                valid_moves.emplace_back(row - 1, col + 1);
    } else {
        if (row + 1 <= 7 && board[row + 1][col] == 0) {
            valid_moves.emplace_back(row + 1, col);
            if (row == 1 && board[row + 2][col] == 0)
                valid_moves.emplace_back(row + 2, col);
        }
        if (col > 0 && row + 1 <= 7 && board[row + 1][col - 1] > 0)
                valid_moves.emplace_back(row + 1, col - 1);
        if (col < 7 && row + 1 <= 7 && board[row + 1][col + 1] > 0)
                valid_moves.emplace_back(row + 1, col + 1);
    }
    return valid_moves;
}

std::vector<std::pair<int, int>> get_rook_moves(int board[8][8], int row, int col, bool is_white) {
    std::vector<std::pair<int, int>> valid_moves;

    for (int i = row - 1; i >= 0; i--) {
        if (board[i][col] == 0)
            valid_moves.emplace_back(i, col);
        else if (board[i][col] < 0 && is_white) {
            valid_moves.emplace_back(i, col);
            break;
        }
        else if (board[i][col] > 0 && !is_white) {
            valid_moves.emplace_back(i, col);
            break;
        }
        else
            break;
    }

    for (int i = row + 1; i <= 7; i++) {
        if (board[i][col] == 0)
            valid_moves.emplace_back(i, col);
        else if (board[i][col] < 0 && is_white) {
            valid_moves.emplace_back(i, col);
            break;
        }
        else if (board[i][col] > 0 && !is_white) {
            valid_moves.emplace_back(i, col);
            break;
        }
        else
            break;
    }

    for (int i = col - 1; i >= 0; i--) {
        if (board[row][i] == 0)
            valid_moves.emplace_back(row, i);
        else if (board[row][i] < 0 && is_white) {
            valid_moves.emplace_back(row, i);
            break;
        }
        else if (board[row][i] > 0 && !is_white) {
            valid_moves.emplace_back(row, i);
            break;
        }
        else
            break;
    }

    for (int i = col + 1; i <= 7; i++) {
        if (board[row][i]== 0)
            valid_moves.emplace_back(row, i);
        else if (board[row][i] < 0 && is_white) {
            valid_moves.emplace_back(row, i);
            break;
        }
        else if (board[row][i] > 0 && !is_white) {
            valid_moves.emplace_back(row, i);
            break;
        }
        else
            break;
    }

    return valid_moves;
}

std::vector<std::pair<int, int>> get_knight_moves(int board[8][8], int row, int col, bool is_white) {
    std::vector<std::pair<int, int>> valid_moves;
    int logical_moves[8][2] = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},
        {2, -1},  {2, 1}
    };

    for (auto& logical_move : logical_moves) {
        int new_row = row + logical_move[0];
        int new_col = col + logical_move[1];

        if (new_row >= 0 && new_row <= 7 && new_col >= 0 && new_col <= 7) {
            if (board[new_row][new_col] == 0)
                valid_moves.emplace_back(new_row, new_col);
            else if (board[new_row][new_col] < 0 && is_white)
                valid_moves.emplace_back(new_row, new_col);
            else if (board[new_row][new_col] > 0 && !is_white)
                valid_moves.emplace_back(new_row, new_col);
        }
    }

    return valid_moves;
}

std::vector<std::pair<int, int>> get_bishop_moves(int board[8][8], int row, int col, bool is_white) {
    std::vector<std::pair<int, int>> valid_moves;

    for (int i = 1; row - i >= 0 && col - i >= 0; i++) {
        if (board[row - i][col - i] == 0)
            valid_moves.emplace_back(row - i, col - i);
        else if (board[row - i][col - i] < 0 && is_white) {
            valid_moves.emplace_back(row - i, col - i);
            break;
        }
        else if (board[row - i][col - i] > 0 && !is_white) {
            valid_moves.emplace_back(row - i, col - i);
            break;
        }
        else
            break;
    }

    for (int i = 1; row - i >= 0 && col + i <= 7; i++) {
        if (board[row - i][col + i] == 0)
            valid_moves.emplace_back(row - i, col + i);
        else if (board[row - i][col + i] < 0 && is_white) {
            valid_moves.emplace_back(row - i, col + i);
            break;
        }
        else if (board[row - i][col + i] > 0 && !is_white) {
            valid_moves.emplace_back(row - i, col + i);
            break;
        }
        else
            break;
    }

    for (int i = 1; row + i <= 7 && col - i >= 0; i++) {
        if (board[row + i][col - i] == 0)
            valid_moves.emplace_back(row + i, col - i);
        else if (board[row + i][col - i] < 0 && is_white) {
            valid_moves.emplace_back(row + i, col - i);
            break;
        }
        else if (board[row + i][col - i] > 0 && !is_white) {
            valid_moves.emplace_back(row + i, col - i);
            break;
        }
        else
            break;
    }

    for (int i = 1; row + i <= 7 && col + i <= 7; i++) {
        if (board[row + i][col + i] == 0)
            valid_moves.emplace_back(row + i, col + i);
        else if (board[row + i][col + i] < 0 && is_white) {
            valid_moves.emplace_back(row + i, col + i);
            break;
        }
        else if (board[row + i][col + i] > 0 && !is_white) {
            valid_moves.emplace_back(row + i, col + i);
            break;
        }
        else
            break;
    }

    return valid_moves;
}

std::vector<std::pair<int, int>> get_queen_moves(int board[8][8], int row, int col, bool is_white) {
    std::vector<std::pair<int, int>> valid_moves = get_rook_moves(board, row, col, is_white);
    std::vector<std::pair<int, int>> bishop_moves = get_bishop_moves(board, row, col, is_white);

    valid_moves.insert(valid_moves.end(), bishop_moves.begin(), bishop_moves.end());
    return valid_moves;
}

std::vector<std::pair<int, int>> get_king_moves(int board[8][8], int row, int col, bool is_white) {
    std::vector<std::pair<int, int>> valid_moves;
    int logical_moves[8][2] = {
        {-1, -1},   {-1, 0},  {-1, 1},
        {0, -1},                 {0, 1},
        {1, -1},    {1, 0},   {1, 1}
    };

    for (auto& logical_move : logical_moves) {
        int new_row = row + logical_move[0];
        int new_col = col + logical_move[1];
        bool attacked_square = is_square_attacked(board, new_row, new_col, !is_white);

        if (new_row >= 0 && new_row <= 7 && new_col >= 0 && new_col <= 7) {
            if (board[new_row][new_col] == 0 && !attacked_square)
                valid_moves.emplace_back(new_row, new_col);
            else if (board[new_row][new_col] < 0 && is_white && !attacked_square)
                valid_moves.emplace_back(new_row, new_col);
            else if (board[new_row][new_col] > 0 && !is_white && !attacked_square)
                valid_moves.emplace_back(new_row, new_col);
        }
    }

    return valid_moves;
}

bool is_square_attacked(int board[8][8], int row, int col, bool by_white) {
    auto in_bounds = [](int x, int y){
      return x >= 0 && x <= 7 && y >= 0 && y <= 7;
    };

    // pawns
    if (by_white) {
        if (in_bounds(row + 1, col - 1) && board[row + 1][col - 1] == 1)
            return true;
        if (in_bounds(row + 1, col + 1) && board[row + 1][col + 1] == 1)
            return true;
    } else {
        if (in_bounds(row - 1, col - 1) && board[row - 1][col - 1] == -1)
            return true;
        if (in_bounds(row - 1, col + 1) && board[row - 1][col + 1] == -1)
            return true;
    }

    // knights
    int logical_moves[8][2] = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},
        {2, -1},  {2, 1}
    };

    for (auto& logical_move : logical_moves) {
        int new_row = row + logical_move[0];
        int new_col = col + logical_move[1];
        if (!in_bounds(new_row, new_col))
            continue;
        if (by_white && board[new_row][new_col] == 3) return true;
        if (!by_white && board[new_row][new_col] == -3) return true;
    }

    // rook or queen
    int orthogonal[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto &dir : orthogonal) {
        int new_row = row + dir[0];
        int new_col = col + dir[1];

        while (in_bounds(new_row, new_col)) {
            int piece = board[new_row][new_col];
            if (piece != 0) {
                int abs_piece = std::abs(piece);
                if (by_white) {
                    if (piece > 0 && (abs_piece == 2 || abs_piece == 5))
                        return true;
                } else {
                    if (piece < 0 && (abs_piece == 2 || abs_piece == 5))
                        return true;
                }
                break;
            }
            new_row += dir[0];
            new_col += dir[1];
        }
    }

    // bishop or queen
    int diagonal[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (auto &dir : diagonal) {
        int new_row = row + dir[0];
        int new_col = col + dir[1];

        while (in_bounds(new_row, new_col)) {
            int piece = board[new_row][new_col];
            if (piece != 0) {
                int abs_piece = std::abs(piece);
                if (by_white) {
                    if (piece > 0 && (abs_piece == 4 || abs_piece == 5))
                        return true;
                } else {
                    if (piece < 0 && (abs_piece == 4 || abs_piece == 5))
                        return true;
                }
                break;
            }
            new_row += dir[0];
            new_col += dir[1];
        }
    }

    // opp king
    for (int dir_row = -1; dir_row <= 1; dir_row++) {
        for (int dir_col = -1; dir_col <= 1; dir_col++) {
            if (dir_row == 0 && dir_col == 0)
                continue;
            int new_row = row + dir_row;
            int new_col = col + dir_col;
            if (!in_bounds(new_row, new_col))
                continue;
            if (by_white && board[new_row][new_col] == 6)
                return true;
            if (!by_white && board[new_row][new_col] == -6)
                return true;
        }
    }
    return false;
}

bool can_castle (int board[8][8], bool is_white, bool queenside, bool can_castle_queenside, bool can_castle_kingside) {
    bool attacker_is_white = !is_white;

    if (is_white) {
        if (queenside) {
            if (!can_castle_queenside) return false;
            if (board[7][3] != 0 || board[7][2] != 0 || board[7][1] != 0) return false;
            if (board[7][0] != 2) return false;
            if (is_square_attacked(board, 7, 4, attacker_is_white)) return false;
            if (is_square_attacked(board, 7, 3, attacker_is_white)) return false;
            if (is_square_attacked(board, 7, 2, attacker_is_white)) return false;
            return true;
        } else {
            if (!can_castle_kingside) return false;
            if (board[7][5] != 0 || board[7][6] != 0) return false;
            if (board[7][7] != 2) return false;
            if (is_square_attacked(board, 7, 4, attacker_is_white)) return false;
            if (is_square_attacked(board, 7, 5, attacker_is_white)) return false;
            if (is_square_attacked(board, 7, 6, attacker_is_white)) return false;
            return true;
        }
    } else {
        if (queenside) {
            if (!can_castle_queenside) return false;
            if (board[0][3] != 0 || board[0][2] != 0 || board[0][1] != 0) return false;
            if (board[0][0] != -2) return false;
            if (is_square_attacked(board, 0, 4, attacker_is_white)) return false;
            if (is_square_attacked(board, 0, 3, attacker_is_white)) return false;
            if (is_square_attacked(board, 0, 2, attacker_is_white)) return false;
            return true;
        } else {
            if (!can_castle_kingside) return false;
            if (board[0][5] != 0 || board[0][6] != 0) return false;
            if (board[0][7] != -2) return false;
            if (is_square_attacked(board, 0, 4, attacker_is_white)) return false;
            if (is_square_attacked(board, 0, 5, attacker_is_white)) return false;
            if (is_square_attacked(board, 0, 6, attacker_is_white)) return false;
            return true;
        }
    }
}