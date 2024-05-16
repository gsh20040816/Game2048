#include<iostream>
#include<iomanip>
#include<random>
#include<ctime>
#include<Windows.h>
#include<conio.h>
#include"Game2048.h"
using namespace std;
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define KEY_ESC 27
#define WIN_VALUE 2048
const int dx[4] = { -1,1,0,0 };
const int dy[4] = { 0,0,-1,1 };
int queryed;

void playGame()
{
	queryed = false;
	int board[4][4] = {};
	int score = 0, step = 0;
	char choice = '\0';
	mt19937 rand(time(0));
	initBoard(board,rand);
	printInterface(board, score, step);
	while (1)
	{
		choice = _getch();
		if ((uint8_t)choice == 224)
		{
			choice = _getch();
			switch (choice)
			{
			case ARROW_UP:
				moveBoard(board, score, step, UP);
				break;
			case ARROW_DOWN:
				moveBoard(board, score, step, DOWN);
				break;
			case ARROW_LEFT:
				moveBoard(board, score, step, LEFT);
				break;
			case ARROW_RIGHT:
				moveBoard(board, score, step, RIGHT);
				break;
			default:
				cout << "\n输入错误，请重新输入" << endl;
				waitEnter();
			}
		}
		else if (choice == KEY_ESC)
		{
			gameOver(board, score, step, "您选择了退出游戏！");
			printExit();
			return;
		}
		else
		{
			cout << "\n输入错误，请重新输入" << endl;
			waitEnter();
		}
		printInterface(board, score, step);
		if (isGameOver(board))
		{
			gameOver(board,score,step);
			printExit();
			return;
		}
		printInterface(board, score, step);
	}
}

int findMax(int board[4][4])
{
	int max = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] > max)max = board[i][j];
		}
	}
	return max;
}

bool isGameOver(int board[4][4])
{
	if (findMax(board) >= WIN_VALUE)
	{
		if (!queryed)
		{
			queryed = true;
			cout << "恭喜你成功合成了" << WIN_VALUE << "！是否继续游戏？(Y/n)";
			char choice = _getch();
			if (choice == 'n' || choice == 'N')return true;
			return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0)return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				int x = i + dx[k], y = j + dy[k];
				if (x < 0 || x >= 4 || y < 0 || y >= 4)continue;
				if (board[i][j] == board[x][y])return false;
			}
		}
	}
	return true;
}

void gameOver(int board[4][4], int score, int step, const char* msg)
{
	printInterface(board, score, step);
	cout << msg << endl;
	if (findMax(board) >= WIN_VALUE)
	{
		cout << "恭喜你成功合成了" << WIN_VALUE << "！" << endl;
	}
	else
	{
		cout<<"你没有成功合成"<<WIN_VALUE<<"！"<<endl;
	}
	cout << "您的分数为：" << score << " 步数为：" << step << endl;
	waitEnter();
}

bool notAbleMove(int board[4][4], int direct)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int x = i + dx[direct], y = j + dy[direct];
			if (x < 0 || x >= 4 || y < 0 || y >= 4)continue;
			if (board[i][j] != 0 && board[x][y] == 0)return false;
			if (board[i][j] != 0 && board[i][j] == board[x][y])return false;
		}
	}
	return true;
}

void moveBoard(int board[4][4], int& score, int& step, int direct)
{
	if (notAbleMove(board, direct))return;
	step++;
	bool merged[4][4] = { 0 };
	while(1)
	{
		bool flag = false;
		for (int i = direct == DOWN ? 3 : 0; direct == DOWN?i>=0:i<4; direct == DOWN ? i-- : i++)
		{
			for (int j = direct == RIGHT ? 3 : 0; direct == RIGHT ? j >= 0 : j < 4; direct == RIGHT ? j-- : j++)
			{
				int x = i + dx[direct], y = j + dy[direct];
				if (x < 0 || x >= 4 || y < 0 || y >= 4)continue;
				if (board[i][j] != 0 && board[x][y] == 0)
				{
					swap(board[i][j], board[x][y]);
					swap(merged[i][j], merged[x][y]);
					flag = true;
				}
				else if (board[i][j] != 0 && board[i][j] == board[x][y])
				{
					if (merged[i][j] || merged[x][y])continue;
					board[x][y] *= 2;
					score += board[x][y];
					board[i][j] = 0;
					merged[x][y] = 1;
					flag = true;
				}
			}
		}
		if (!flag)break;
	}
	int x = rand() % 4, y = rand() % 4;
	while (board[x][y] != 0)
	{
		x = rand() % 4;
		y = rand() % 4;
	}
	board[x][y] = rand() & 1 ? 2 : 4;
}

void initBoard(int board[4][4], mt19937& rnd)
{
	int x1 = rnd() % 4, y1 = rnd() % 4;
	int x2 = rnd() % 4, y2 = rnd() % 4;
	while (x1 == x2 && y1 == y2)
	{
		x2 = rnd() % 4;
		y2 = rnd() % 4;
	}
	board[x1][y1] = rnd() & 1 ? 2 : 4;
	board[x2][y2] = rnd() & 1 ? 2 : 4;
}

void printInterface(int board[4][4], int score, int step)
{
	system("CLS");
	//获取标准输入设备句柄
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	// 设置控制台文字颜色
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN);
	// 打印游戏界面
	cout << "            --------------------------------------------\n";
	cout << "            分数：" << setw(6) << score << "              步数：" << setw(6) << step << endl;
	cout << "            --------------------------------------------\n";
	cout << "            ********************************************\n";
	// 设置控制台文字颜色
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "                       |----|----|----|----|\n";

	for (int i = 0; i < 4; i++)
	{
		cout << "                       |";
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] != 0)
			{
				cout << setw(4) << board[i][j] << "|";
			}
			else
			{
				cout << "    |";
			}
		}
		cout << "\n                       |----|----|----|----|\n";
	}

	// 设置控制台文字颜色
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "            ********************************************\n";
	cout << "            --------------------------------------------\n";
	cout << "            ↓：下   ←：左  ↑：上  →：右  ESC键：退出\n\n";

	// 恢复控制台文字颜色为默认颜色
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}