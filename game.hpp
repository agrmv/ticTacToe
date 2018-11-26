#pragma once
#include <iostream>
#include <memory>

class Game {
    std::pair<std::string, std::string> gamerName;
protected:
    size_t checkDraw;
    std::pair<uint8_t, bool> gameStatus;
    const static size_t sizeOfBoard = 3;
    std::array<std::array<int32_t, sizeOfBoard>, sizeOfBoard> board;
    Game() : gamerName{"gamerX","gamerO"}, checkDraw(0), gameStatus{0, true} {
        gameInit();
    }
    Game(const std::string& name1, const std::string& name2) : gamerName{name1,name2}, gameStatus{0, true},
    checkDraw(0) {
        gameInit();
    }
    void gameInit();
    void checkVertical(const size_t& col);
    void checkHorizontal(const size_t& row);
    void checkMainDiagonal();
    void checkSideDiagonal();
    const std::string& winnerName() const;
};


void Game::gameInit() {

    for (auto &i : board) {
        for (auto &j : i) {
            j = -1;
        }
    }
}

void Game::checkVertical(const size_t& col) {
    bool state = false;

    for (const auto &i : board) {
        if (i[col] != gameStatus.first) {
            state = true;
            break;
        }
    }
    if(!state) {
        gameStatus.second = false;
    }
}

void Game::checkHorizontal(const size_t&row) {
    bool state = false;

    for (size_t i = 0; i < board.size(); ++i) {
        if (board[row][i] != gameStatus.first) {
            state = true;
            break;
        }
    }
    if(!state) {
        gameStatus.second = false;
    }
}

void Game::checkMainDiagonal() {
    bool state = false;

    for (size_t i = 0; i <  board.size(); ++i) {
        if (board[i][i] != gameStatus.first) {
            state = true;
            break;
        }
    }
    if(!state) {
        gameStatus.second = false;
    }
}

void Game::checkSideDiagonal() {
    bool state = false;

    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i][board.size() - i - 1] != gameStatus.first) {
            state = true;
            break;
        }
    }
    if(!state) {
        gameStatus.second = false;
    }
}

const std::string& Game::winnerName() const {
    if (!gameStatus.second) {
        return (gameStatus.first) ? gamerName.first : gamerName.second;
    }
}