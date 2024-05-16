#include<random>
using std::mt19937;
#pragma once
void printMainWindow();
void printMenu();
void printHelp();
void printExit();
void waitEnter();
void playGame();
void initBoard(int board[4][4], mt19937& rnd);
void moveBoard(int board[4][4], int& score, int& step, int direct);
void printInterface(int board[4][4], int score, int step);
int findMax(int board[4][4]);
bool isGameOver(int board[4][4]);
void gameOver(int board[4][4], int score, int step, const char* msg="游戏结束！");