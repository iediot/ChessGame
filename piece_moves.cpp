//
// Created by edi on 12/11/25.
//

#include "piece_moves.h"

#include <vector>
#include <math.h>

std::vector<std::pair<int, int>> getValidMoves(int board[8][8], int row, int col) {
    int piece = board[row][col];
    int type = abs(piece);

    switch (type)
    {
    case 1:
        return getPawnMoves(board, row, col, piece > 0);
    case 2:
        return getRookMoves(board, row, col, piece > 0);
    case 3:
        return getKnightMoves(board, row, col, piece > 0);
    case 4:
        return getBishopMoves(board, row, col, piece > 0);
    case 5:
        return getQueenMoves(board, row, col, piece > 0);
    case 6:
        return getKingMoves(board, row, col, piece > 0);
    default:
        return {};
    }
}

std::vector<std::pair<int, int>> getPawnMoves(int board[8][8], int row, int col, bool isWhite) {
    std::vector<std::pair<int, int>> valid_moves;

    if (isWhite) {
        if (row - 1 >= 0 && board[row - 1][col] == 0) {
            valid_moves.push_back({row - 1, col});
            if (row == 6 && board[row - 2][col] == 0)
                valid_moves.push_back({row - 2, col});
        }
        if (col > 0 && row - 1 >= 0 && board[row - 1][col - 1] < 0)
                valid_moves.push_back({row - 1, col - 1});
        if (col < 7 && row - 1 >= 0 && board[row - 1][col + 1] < 0)
                valid_moves.push_back({row - 1, col + 1});
    } else {
        if (row + 1 <= 7 && board[row + 1][col] == 0) {
            valid_moves.push_back({row + 1, col});
            if (row == 1 && board[row + 2][col] == 0)
                valid_moves.push_back({row + 2, col});
        }
        if (col > 0 && row + 1 <= 7 && board[row + 1][col - 1] > 0)
                valid_moves.push_back({row + 1, col - 1});
        if (col < 7 && row + 1 <= 7 && board[row + 1][col + 1] > 0)
                valid_moves.push_back({row + 1, col + 1});
    }
    return valid_moves;
}

std::vector<std::pair<int, int>> getRookMoves(int board[8][8], int row, int col, bool isWhite) {
    std::vector<std::pair<int, int>> valid_moves;

    for (int i = row - 1; i >= 0; i--) {
        if (board[i][col] == 0)
            valid_moves.push_back({i, col});
        else if (board[i][col] < 0 && isWhite) {
            valid_moves.push_back({i, col});
            break;
        }
        else if (board[i][col] > 0 && !isWhite) {
            valid_moves.push_back({i, col});
            break;
        }
        else
            break;
    }

    for (int i = row + 1; i <= 7; i++) {
        if (board[i][col] == 0)
            valid_moves.push_back({i, col});
        else if (board[i][col] < 0 && isWhite) {
            valid_moves.push_back({i, col});
            break;
        }
        else if (board[i][col] > 0 && !isWhite) {
            valid_moves.push_back({i, col});
            break;
        }
        else
            break;
    }

    for (int i = col - 1; i >= 0; i--) {
        if (board[row][i] == 0)
            valid_moves.push_back({row, i});
        else if (board[row][i] < 0 && isWhite) {
            valid_moves.push_back({row, i});
            break;
        }
        else if (board[row][i] > 0 && !isWhite) {
            valid_moves.push_back({row, i});
            break;
        }
        else
            break;
    }

    for (int i = col + 1; i <= 7; i++) {
        if (board[row][i]== 0)
            valid_moves.push_back({row, i});
        else if (board[row][i] < 0 && isWhite) {
            valid_moves.push_back({row, i});
            break;
        }
        else if (board[row][i] > 0 && !isWhite) {
            valid_moves.push_back({row, i});
            break;
        }
        else
            break;
    }

    return valid_moves;
}

std::vector<std::pair<int, int>> getKnightMoves(int board[8][8], int row, int col, bool isWhite) {
    std::vector<std::pair<int, int>> valid_moves;
    int logical_moves[8][2] = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},
        {2, -1},  {2, 1}
    };

    for (auto& logical_move : logical_moves) {
        int newRow = row + logical_move[0];
        int newCol = col + logical_move[1];

        if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            if (board[newRow][newCol] == 0)
                valid_moves.push_back({newRow, newCol});
            else if (board[newRow][newCol] < 0 && isWhite)
                valid_moves.push_back({newRow, newCol});
            else if (board[newRow][newCol] > 0 && !isWhite)
                valid_moves.push_back({newRow, newCol});
        }
    }

    return valid_moves;
}

std::vector<std::pair<int, int>> getBishopMoves(int board[8][8], int row, int col, bool isWhite) {
    std::vector<std::pair<int, int>> valid_moves;

    for (int i = 1; row - i >= 0 && col - i >= 0; i++) {
        if (board[row - i][col - i] == 0)
            valid_moves.push_back({row - i, col - i});
        else if (board[row - i][col - i] < 0 && isWhite) {
            valid_moves.push_back({row - i, col - i});
            break;
        }
        else if (board[row - i][col - i] > 0 && !isWhite) {
            valid_moves.push_back({row - i, col - i});
            break;
        }
        else
            break;
    }

    for (int i = 1; row - i >= 0 && col + i <= 7; i++) {
        if (board[row - i][col + i] == 0)
            valid_moves.push_back({row - i, col + i});
        else if (board[row - i][col + i] < 0 && isWhite) {
            valid_moves.push_back({row - i, col + i});
            break;
        }
        else if (board[row - i][col + i] > 0 && !isWhite) {
            valid_moves.push_back({row - i, col + i});
            break;
        }
        else
            break;
    }

    for (int i = 1; row + i <= 7 && col - i >= 0; i++) {
        if (board[row + i][col - i] == 0)
            valid_moves.push_back({row + i, col - i});
        else if (board[row + i][col - i] < 0 && isWhite) {
            valid_moves.push_back({row + i, col - i});
            break;
        }
        else if (board[row + i][col - i] > 0 && !isWhite) {
            valid_moves.push_back({row + i, col - i});
            break;
        }
        else
            break;
    }

    for (int i = 1; row + i <= 7 && col + i <= 7; i++) {
        if (board[row + i][col + i] == 0)
            valid_moves.push_back({row + i, col + i});
        else if (board[row + i][col + i] < 0 && isWhite) {
            valid_moves.push_back({row + i, col + i});
            break;
        }
        else if (board[row + i][col + i] > 0 && !isWhite) {
            valid_moves.push_back({row + i, col + i});
            break;
        }
        else
            break;
    }

    return valid_moves;
}

std::vector<std::pair<int, int>> getQueenMoves(int board[8][8], int row, int col, bool isWhite) {
    std::vector<std::pair<int, int>> valid_moves = getRookMoves(board, row, col, isWhite);
    std::vector<std::pair<int, int>> bishop_moves = getBishopMoves(board, row, col, isWhite);

    valid_moves.insert(valid_moves.end(), bishop_moves.begin(), bishop_moves.end());
    return valid_moves;
}
std::vector<std::pair<int, int>> getKingMoves(int board[8][8], int row, int col, bool isWhite) {
    std::vector<std::pair<int, int>> valid_moves;
    int logical_moves[8][2] = {
        {-1, -1},   {-1, 0},  {-1, 1},
        {0, -1},                 {0, 1},
        {1, -1},    {1, 0},   {1, 1}
    };

    for (auto& logical_move : logical_moves) {
        int newRow = row + logical_move[0];
        int newCol = col + logical_move[1];

        if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            if (board[newRow][newCol] == 0)
                valid_moves.push_back({newRow, newCol});
            else if (board[newRow][newCol] < 0 && isWhite)
                valid_moves.push_back({newRow, newCol});
            else if (board[newRow][newCol] > 0 && !isWhite)
                valid_moves.push_back({newRow, newCol});
        }
    }

    return valid_moves;
}