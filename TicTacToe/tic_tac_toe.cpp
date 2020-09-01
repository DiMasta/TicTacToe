#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx")  //Enable AVX
#include <x86intrin.h> //AVX/SSE Extensions
#include <bits/stdc++.h> //All main STD libraries

//#define REDIRECT_INPUT
//#define OUTPUT_GAME_DATA
//#define DEBUG_ONE_TURN

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <cmath>
#include <cstring>

using namespace std;

static const string INPUT_FILE_NAME = "input.txt";
static const string OUTPUT_FILE_NAME = "output.txt";
static const string EMPTY_STRING = "";
static constexpr char SPACE = ' ';
static constexpr char TAB = '\t';
static constexpr int INVALID_IDX = -1;
static constexpr int TRIPLE = 3;
static constexpr int STOP_INPUT = 10;
static constexpr int BOARD_DIM = 9;
static constexpr int ALL_SQUARES = BOARD_DIM * BOARD_DIM;
static constexpr int PLAYER_TOGGLE = 2;
static constexpr int MY_PLAYER_IDX = 0;
static constexpr int OPPONENT_PLAYER_IDX = 1;
static constexpr int MAX_NODES = 11'000'000;
static constexpr char MY_PLAYER_CHAR = 'X';
static constexpr char OPPONENT_PLAYER_CHAR = 'O';
static constexpr char EMPTY_CHAR = '_';
static constexpr size_t NODES_TO_RESERVE = 15'000'000;
static constexpr long long FIRST_TURN_MS = 1'000;
static constexpr long long TURN_MS = 100;
static constexpr long long BIAS_MS = 2;
static constexpr float WIN_VALUE = 1.f;
static constexpr float DRAW_VALUE = 0.5f;
static constexpr unsigned short PLAYER_FLAG		= 0b0000'0000'0000'0001;
static constexpr unsigned short STATUS_MASK		= 0b0000'0000'0000'0110;
static constexpr unsigned short MOVE_ROW_MASK	= 0b0000'0000'0111'1000;
static constexpr unsigned short MOVE_COL_MASK	= 0b0000'0111'1000'0000;
static constexpr unsigned short STATUS_OFFSET = 1;
static constexpr unsigned short MOVE_ROW_OFFSET = 3;
static constexpr unsigned short MOVE_COL_OFFSET = 7;
static constexpr unsigned short ALL_POSSOBLE_FILLED_BOARDS = 512 - 1; /// 2 ^ 9
static constexpr int SQUARE_TYPES = 2;
static constexpr int FIRST_TURN_ITERATIONS = 150'000;
static constexpr int TURN_ITERATIONS = 22'000;
static constexpr short EMPTY_TICTACTOE_BOARD = 0;
static constexpr short FULL_BOARD_MASK = 0b0000'000'111'111'111;
static float SQRT_2;
static float MAX_FLOAT;

int ALL_MOVES[ALL_POSSOBLE_FILLED_BOARDS][BOARD_DIM] = {
	{0,1,2,3,4,5,6,7,8},
	{1,2,3,4,5,6,7,8,9},
	{0,2,3,4,5,6,7,8,9},
	{2,3,4,5,6,7,8,9,9},
	{0,1,3,4,5,6,7,8,9},
	{1,3,4,5,6,7,8,9,9},
	{0,3,4,5,6,7,8,9,9},
	{3,4,5,6,7,8,9,9,9},
	{0,1,2,4,5,6,7,8,9},
	{1,2,4,5,6,7,8,9,9},
	{0,2,4,5,6,7,8,9,9},
	{2,4,5,6,7,8,9,9,9},
	{0,1,4,5,6,7,8,9,9},
	{1,4,5,6,7,8,9,9,9},
	{0,4,5,6,7,8,9,9,9},
	{4,5,6,7,8,9,9,9,9},
	{0,1,2,3,5,6,7,8,9},
	{1,2,3,5,6,7,8,9,9},
	{0,2,3,5,6,7,8,9,9},
	{2,3,5,6,7,8,9,9,9},
	{0,1,3,5,6,7,8,9,9},
	{1,3,5,6,7,8,9,9,9},
	{0,3,5,6,7,8,9,9,9},
	{3,5,6,7,8,9,9,9,9},
	{0,1,2,5,6,7,8,9,9},
	{1,2,5,6,7,8,9,9,9},
	{0,2,5,6,7,8,9,9,9},
	{2,5,6,7,8,9,9,9,9},
	{0,1,5,6,7,8,9,9,9},
	{1,5,6,7,8,9,9,9,9},
	{0,5,6,7,8,9,9,9,9},
	{5,6,7,8,9,9,9,9,9},
	{0,1,2,3,4,6,7,8,9},
	{1,2,3,4,6,7,8,9,9},
	{0,2,3,4,6,7,8,9,9},
	{2,3,4,6,7,8,9,9,9},
	{0,1,3,4,6,7,8,9,9},
	{1,3,4,6,7,8,9,9,9},
	{0,3,4,6,7,8,9,9,9},
	{3,4,6,7,8,9,9,9,9},
	{0,1,2,4,6,7,8,9,9},
	{1,2,4,6,7,8,9,9,9},
	{0,2,4,6,7,8,9,9,9},
	{2,4,6,7,8,9,9,9,9},
	{0,1,4,6,7,8,9,9,9},
	{1,4,6,7,8,9,9,9,9},
	{0,4,6,7,8,9,9,9,9},
	{4,6,7,8,9,9,9,9,9},
	{0,1,2,3,6,7,8,9,9},
	{1,2,3,6,7,8,9,9,9},
	{0,2,3,6,7,8,9,9,9},
	{2,3,6,7,8,9,9,9,9},
	{0,1,3,6,7,8,9,9,9},
	{1,3,6,7,8,9,9,9,9},
	{0,3,6,7,8,9,9,9,9},
	{3,6,7,8,9,9,9,9,9},
	{0,1,2,6,7,8,9,9,9},
	{1,2,6,7,8,9,9,9,9},
	{0,2,6,7,8,9,9,9,9},
	{2,6,7,8,9,9,9,9,9},
	{0,1,6,7,8,9,9,9,9},
	{1,6,7,8,9,9,9,9,9},
	{0,6,7,8,9,9,9,9,9},
	{6,7,8,9,9,9,9,9,9},
	{0,1,2,3,4,5,7,8,9},
	{1,2,3,4,5,7,8,9,9},
	{0,2,3,4,5,7,8,9,9},
	{2,3,4,5,7,8,9,9,9},
	{0,1,3,4,5,7,8,9,9},
	{1,3,4,5,7,8,9,9,9},
	{0,3,4,5,7,8,9,9,9},
	{3,4,5,7,8,9,9,9,9},
	{0,1,2,4,5,7,8,9,9},
	{1,2,4,5,7,8,9,9,9},
	{0,2,4,5,7,8,9,9,9},
	{2,4,5,7,8,9,9,9,9},
	{0,1,4,5,7,8,9,9,9},
	{1,4,5,7,8,9,9,9,9},
	{0,4,5,7,8,9,9,9,9},
	{4,5,7,8,9,9,9,9,9},
	{0,1,2,3,5,7,8,9,9},
	{1,2,3,5,7,8,9,9,9},
	{0,2,3,5,7,8,9,9,9},
	{2,3,5,7,8,9,9,9,9},
	{0,1,3,5,7,8,9,9,9},
	{1,3,5,7,8,9,9,9,9},
	{0,3,5,7,8,9,9,9,9},
	{3,5,7,8,9,9,9,9,9},
	{0,1,2,5,7,8,9,9,9},
	{1,2,5,7,8,9,9,9,9},
	{0,2,5,7,8,9,9,9,9},
	{2,5,7,8,9,9,9,9,9},
	{0,1,5,7,8,9,9,9,9},
	{1,5,7,8,9,9,9,9,9},
	{0,5,7,8,9,9,9,9,9},
	{5,7,8,9,9,9,9,9,9},
	{0,1,2,3,4,7,8,9,9},
	{1,2,3,4,7,8,9,9,9},
	{0,2,3,4,7,8,9,9,9},
	{2,3,4,7,8,9,9,9,9},
	{0,1,3,4,7,8,9,9,9},
	{1,3,4,7,8,9,9,9,9},
	{0,3,4,7,8,9,9,9,9},
	{3,4,7,8,9,9,9,9,9},
	{0,1,2,4,7,8,9,9,9},
	{1,2,4,7,8,9,9,9,9},
	{0,2,4,7,8,9,9,9,9},
	{2,4,7,8,9,9,9,9,9},
	{0,1,4,7,8,9,9,9,9},
	{1,4,7,8,9,9,9,9,9},
	{0,4,7,8,9,9,9,9,9},
	{4,7,8,9,9,9,9,9,9},
	{0,1,2,3,7,8,9,9,9},
	{1,2,3,7,8,9,9,9,9},
	{0,2,3,7,8,9,9,9,9},
	{2,3,7,8,9,9,9,9,9},
	{0,1,3,7,8,9,9,9,9},
	{1,3,7,8,9,9,9,9,9},
	{0,3,7,8,9,9,9,9,9},
	{3,7,8,9,9,9,9,9,9},
	{0,1,2,7,8,9,9,9,9},
	{1,2,7,8,9,9,9,9,9},
	{0,2,7,8,9,9,9,9,9},
	{2,7,8,9,9,9,9,9,9},
	{0,1,7,8,9,9,9,9,9},
	{1,7,8,9,9,9,9,9,9},
	{0,7,8,9,9,9,9,9,9},
	{7,8,9,9,9,9,9,9,9},
	{0,1,2,3,4,5,6,8,9},
	{1,2,3,4,5,6,8,9,9},
	{0,2,3,4,5,6,8,9,9},
	{2,3,4,5,6,8,9,9,9},
	{0,1,3,4,5,6,8,9,9},
	{1,3,4,5,6,8,9,9,9},
	{0,3,4,5,6,8,9,9,9},
	{3,4,5,6,8,9,9,9,9},
	{0,1,2,4,5,6,8,9,9},
	{1,2,4,5,6,8,9,9,9},
	{0,2,4,5,6,8,9,9,9},
	{2,4,5,6,8,9,9,9,9},
	{0,1,4,5,6,8,9,9,9},
	{1,4,5,6,8,9,9,9,9},
	{0,4,5,6,8,9,9,9,9},
	{4,5,6,8,9,9,9,9,9},
	{0,1,2,3,5,6,8,9,9},
	{1,2,3,5,6,8,9,9,9},
	{0,2,3,5,6,8,9,9,9},
	{2,3,5,6,8,9,9,9,9},
	{0,1,3,5,6,8,9,9,9},
	{1,3,5,6,8,9,9,9,9},
	{0,3,5,6,8,9,9,9,9},
	{3,5,6,8,9,9,9,9,9},
	{0,1,2,5,6,8,9,9,9},
	{1,2,5,6,8,9,9,9,9},
	{0,2,5,6,8,9,9,9,9},
	{2,5,6,8,9,9,9,9,9},
	{0,1,5,6,8,9,9,9,9},
	{1,5,6,8,9,9,9,9,9},
	{0,5,6,8,9,9,9,9,9},
	{5,6,8,9,9,9,9,9,9},
	{0,1,2,3,4,6,8,9,9},
	{1,2,3,4,6,8,9,9,9},
	{0,2,3,4,6,8,9,9,9},
	{2,3,4,6,8,9,9,9,9},
	{0,1,3,4,6,8,9,9,9},
	{1,3,4,6,8,9,9,9,9},
	{0,3,4,6,8,9,9,9,9},
	{3,4,6,8,9,9,9,9,9},
	{0,1,2,4,6,8,9,9,9},
	{1,2,4,6,8,9,9,9,9},
	{0,2,4,6,8,9,9,9,9},
	{2,4,6,8,9,9,9,9,9},
	{0,1,4,6,8,9,9,9,9},
	{1,4,6,8,9,9,9,9,9},
	{0,4,6,8,9,9,9,9,9},
	{4,6,8,9,9,9,9,9,9},
	{0,1,2,3,6,8,9,9,9},
	{1,2,3,6,8,9,9,9,9},
	{0,2,3,6,8,9,9,9,9},
	{2,3,6,8,9,9,9,9,9},
	{0,1,3,6,8,9,9,9,9},
	{1,3,6,8,9,9,9,9,9},
	{0,3,6,8,9,9,9,9,9},
	{3,6,8,9,9,9,9,9,9},
	{0,1,2,6,8,9,9,9,9},
	{1,2,6,8,9,9,9,9,9},
	{0,2,6,8,9,9,9,9,9},
	{2,6,8,9,9,9,9,9,9},
	{0,1,6,8,9,9,9,9,9},
	{1,6,8,9,9,9,9,9,9},
	{0,6,8,9,9,9,9,9,9},
	{6,8,9,9,9,9,9,9,9},
	{0,1,2,3,4,5,8,9,9},
	{1,2,3,4,5,8,9,9,9},
	{0,2,3,4,5,8,9,9,9},
	{2,3,4,5,8,9,9,9,9},
	{0,1,3,4,5,8,9,9,9},
	{1,3,4,5,8,9,9,9,9},
	{0,3,4,5,8,9,9,9,9},
	{3,4,5,8,9,9,9,9,9},
	{0,1,2,4,5,8,9,9,9},
	{1,2,4,5,8,9,9,9,9},
	{0,2,4,5,8,9,9,9,9},
	{2,4,5,8,9,9,9,9,9},
	{0,1,4,5,8,9,9,9,9},
	{1,4,5,8,9,9,9,9,9},
	{0,4,5,8,9,9,9,9,9},
	{4,5,8,9,9,9,9,9,9},
	{0,1,2,3,5,8,9,9,9},
	{1,2,3,5,8,9,9,9,9},
	{0,2,3,5,8,9,9,9,9},
	{2,3,5,8,9,9,9,9,9},
	{0,1,3,5,8,9,9,9,9},
	{1,3,5,8,9,9,9,9,9},
	{0,3,5,8,9,9,9,9,9},
	{3,5,8,9,9,9,9,9,9},
	{0,1,2,5,8,9,9,9,9},
	{1,2,5,8,9,9,9,9,9},
	{0,2,5,8,9,9,9,9,9},
	{2,5,8,9,9,9,9,9,9},
	{0,1,5,8,9,9,9,9,9},
	{1,5,8,9,9,9,9,9,9},
	{0,5,8,9,9,9,9,9,9},
	{5,8,9,9,9,9,9,9,9},
	{0,1,2,3,4,8,9,9,9},
	{1,2,3,4,8,9,9,9,9},
	{0,2,3,4,8,9,9,9,9},
	{2,3,4,8,9,9,9,9,9},
	{0,1,3,4,8,9,9,9,9},
	{1,3,4,8,9,9,9,9,9},
	{0,3,4,8,9,9,9,9,9},
	{3,4,8,9,9,9,9,9,9},
	{0,1,2,4,8,9,9,9,9},
	{1,2,4,8,9,9,9,9,9},
	{0,2,4,8,9,9,9,9,9},
	{2,4,8,9,9,9,9,9,9},
	{0,1,4,8,9,9,9,9,9},
	{1,4,8,9,9,9,9,9,9},
	{0,4,8,9,9,9,9,9,9},
	{4,8,9,9,9,9,9,9,9},
	{0,1,2,3,8,9,9,9,9},
	{1,2,3,8,9,9,9,9,9},
	{0,2,3,8,9,9,9,9,9},
	{2,3,8,9,9,9,9,9,9},
	{0,1,3,8,9,9,9,9,9},
	{1,3,8,9,9,9,9,9,9},
	{0,3,8,9,9,9,9,9,9},
	{3,8,9,9,9,9,9,9,9},
	{0,1,2,8,9,9,9,9,9},
	{1,2,8,9,9,9,9,9,9},
	{0,2,8,9,9,9,9,9,9},
	{2,8,9,9,9,9,9,9,9},
	{0,1,8,9,9,9,9,9,9},
	{1,8,9,9,9,9,9,9,9},
	{0,8,9,9,9,9,9,9,9},
	{8,9,9,9,9,9,9,9,9},
	{0,1,2,3,4,5,6,7,9},
	{1,2,3,4,5,6,7,9,9},
	{0,2,3,4,5,6,7,9,9},
	{2,3,4,5,6,7,9,9,9},
	{0,1,3,4,5,6,7,9,9},
	{1,3,4,5,6,7,9,9,9},
	{0,3,4,5,6,7,9,9,9},
	{3,4,5,6,7,9,9,9,9},
	{0,1,2,4,5,6,7,9,9},
	{1,2,4,5,6,7,9,9,9},
	{0,2,4,5,6,7,9,9,9},
	{2,4,5,6,7,9,9,9,9},
	{0,1,4,5,6,7,9,9,9},
	{1,4,5,6,7,9,9,9,9},
	{0,4,5,6,7,9,9,9,9},
	{4,5,6,7,9,9,9,9,9},
	{0,1,2,3,5,6,7,9,9},
	{1,2,3,5,6,7,9,9,9},
	{0,2,3,5,6,7,9,9,9},
	{2,3,5,6,7,9,9,9,9},
	{0,1,3,5,6,7,9,9,9},
	{1,3,5,6,7,9,9,9,9},
	{0,3,5,6,7,9,9,9,9},
	{3,5,6,7,9,9,9,9,9},
	{0,1,2,5,6,7,9,9,9},
	{1,2,5,6,7,9,9,9,9},
	{0,2,5,6,7,9,9,9,9},
	{2,5,6,7,9,9,9,9,9},
	{0,1,5,6,7,9,9,9,9},
	{1,5,6,7,9,9,9,9,9},
	{0,5,6,7,9,9,9,9,9},
	{5,6,7,9,9,9,9,9,9},
	{0,1,2,3,4,6,7,9,9},
	{1,2,3,4,6,7,9,9,9},
	{0,2,3,4,6,7,9,9,9},
	{2,3,4,6,7,9,9,9,9},
	{0,1,3,4,6,7,9,9,9},
	{1,3,4,6,7,9,9,9,9},
	{0,3,4,6,7,9,9,9,9},
	{3,4,6,7,9,9,9,9,9},
	{0,1,2,4,6,7,9,9,9},
	{1,2,4,6,7,9,9,9,9},
	{0,2,4,6,7,9,9,9,9},
	{2,4,6,7,9,9,9,9,9},
	{0,1,4,6,7,9,9,9,9},
	{1,4,6,7,9,9,9,9,9},
	{0,4,6,7,9,9,9,9,9},
	{4,6,7,9,9,9,9,9,9},
	{0,1,2,3,6,7,9,9,9},
	{1,2,3,6,7,9,9,9,9},
	{0,2,3,6,7,9,9,9,9},
	{2,3,6,7,9,9,9,9,9},
	{0,1,3,6,7,9,9,9,9},
	{1,3,6,7,9,9,9,9,9},
	{0,3,6,7,9,9,9,9,9},
	{3,6,7,9,9,9,9,9,9},
	{0,1,2,6,7,9,9,9,9},
	{1,2,6,7,9,9,9,9,9},
	{0,2,6,7,9,9,9,9,9},
	{2,6,7,9,9,9,9,9,9},
	{0,1,6,7,9,9,9,9,9},
	{1,6,7,9,9,9,9,9,9},
	{0,6,7,9,9,9,9,9,9},
	{6,7,9,9,9,9,9,9,9},
	{0,1,2,3,4,5,7,9,9},
	{1,2,3,4,5,7,9,9,9},
	{0,2,3,4,5,7,9,9,9},
	{2,3,4,5,7,9,9,9,9},
	{0,1,3,4,5,7,9,9,9},
	{1,3,4,5,7,9,9,9,9},
	{0,3,4,5,7,9,9,9,9},
	{3,4,5,7,9,9,9,9,9},
	{0,1,2,4,5,7,9,9,9},
	{1,2,4,5,7,9,9,9,9},
	{0,2,4,5,7,9,9,9,9},
	{2,4,5,7,9,9,9,9,9},
	{0,1,4,5,7,9,9,9,9},
	{1,4,5,7,9,9,9,9,9},
	{0,4,5,7,9,9,9,9,9},
	{4,5,7,9,9,9,9,9,9},
	{0,1,2,3,5,7,9,9,9},
	{1,2,3,5,7,9,9,9,9},
	{0,2,3,5,7,9,9,9,9},
	{2,3,5,7,9,9,9,9,9},
	{0,1,3,5,7,9,9,9,9},
	{1,3,5,7,9,9,9,9,9},
	{0,3,5,7,9,9,9,9,9},
	{3,5,7,9,9,9,9,9,9},
	{0,1,2,5,7,9,9,9,9},
	{1,2,5,7,9,9,9,9,9},
	{0,2,5,7,9,9,9,9,9},
	{2,5,7,9,9,9,9,9,9},
	{0,1,5,7,9,9,9,9,9},
	{1,5,7,9,9,9,9,9,9},
	{0,5,7,9,9,9,9,9,9},
	{5,7,9,9,9,9,9,9,9},
	{0,1,2,3,4,7,9,9,9},
	{1,2,3,4,7,9,9,9,9},
	{0,2,3,4,7,9,9,9,9},
	{2,3,4,7,9,9,9,9,9},
	{0,1,3,4,7,9,9,9,9},
	{1,3,4,7,9,9,9,9,9},
	{0,3,4,7,9,9,9,9,9},
	{3,4,7,9,9,9,9,9,9},
	{0,1,2,4,7,9,9,9,9},
	{1,2,4,7,9,9,9,9,9},
	{0,2,4,7,9,9,9,9,9},
	{2,4,7,9,9,9,9,9,9},
	{0,1,4,7,9,9,9,9,9},
	{1,4,7,9,9,9,9,9,9},
	{0,4,7,9,9,9,9,9,9},
	{4,7,9,9,9,9,9,9,9},
	{0,1,2,3,7,9,9,9,9},
	{1,2,3,7,9,9,9,9,9},
	{0,2,3,7,9,9,9,9,9},
	{2,3,7,9,9,9,9,9,9},
	{0,1,3,7,9,9,9,9,9},
	{1,3,7,9,9,9,9,9,9},
	{0,3,7,9,9,9,9,9,9},
	{3,7,9,9,9,9,9,9,9},
	{0,1,2,7,9,9,9,9,9},
	{1,2,7,9,9,9,9,9,9},
	{0,2,7,9,9,9,9,9,9},
	{2,7,9,9,9,9,9,9,9},
	{0,1,7,9,9,9,9,9,9},
	{1,7,9,9,9,9,9,9,9},
	{0,7,9,9,9,9,9,9,9},
	{7,9,9,9,9,9,9,9,9},
	{0,1,2,3,4,5,6,9,9},
	{1,2,3,4,5,6,9,9,9},
	{0,2,3,4,5,6,9,9,9},
	{2,3,4,5,6,9,9,9,9},
	{0,1,3,4,5,6,9,9,9},
	{1,3,4,5,6,9,9,9,9},
	{0,3,4,5,6,9,9,9,9},
	{3,4,5,6,9,9,9,9,9},
	{0,1,2,4,5,6,9,9,9},
	{1,2,4,5,6,9,9,9,9},
	{0,2,4,5,6,9,9,9,9},
	{2,4,5,6,9,9,9,9,9},
	{0,1,4,5,6,9,9,9,9},
	{1,4,5,6,9,9,9,9,9},
	{0,4,5,6,9,9,9,9,9},
	{4,5,6,9,9,9,9,9,9},
	{0,1,2,3,5,6,9,9,9},
	{1,2,3,5,6,9,9,9,9},
	{0,2,3,5,6,9,9,9,9},
	{2,3,5,6,9,9,9,9,9},
	{0,1,3,5,6,9,9,9,9},
	{1,3,5,6,9,9,9,9,9},
	{0,3,5,6,9,9,9,9,9},
	{3,5,6,9,9,9,9,9,9},
	{0,1,2,5,6,9,9,9,9},
	{1,2,5,6,9,9,9,9,9},
	{0,2,5,6,9,9,9,9,9},
	{2,5,6,9,9,9,9,9,9},
	{0,1,5,6,9,9,9,9,9},
	{1,5,6,9,9,9,9,9,9},
	{0,5,6,9,9,9,9,9,9},
	{5,6,9,9,9,9,9,9,9},
	{0,1,2,3,4,6,9,9,9},
	{1,2,3,4,6,9,9,9,9},
	{0,2,3,4,6,9,9,9,9},
	{2,3,4,6,9,9,9,9,9},
	{0,1,3,4,6,9,9,9,9},
	{1,3,4,6,9,9,9,9,9},
	{0,3,4,6,9,9,9,9,9},
	{3,4,6,9,9,9,9,9,9},
	{0,1,2,4,6,9,9,9,9},
	{1,2,4,6,9,9,9,9,9},
	{0,2,4,6,9,9,9,9,9},
	{2,4,6,9,9,9,9,9,9},
	{0,1,4,6,9,9,9,9,9},
	{1,4,6,9,9,9,9,9,9},
	{0,4,6,9,9,9,9,9,9},
	{4,6,9,9,9,9,9,9,9},
	{0,1,2,3,6,9,9,9,9},
	{1,2,3,6,9,9,9,9,9},
	{0,2,3,6,9,9,9,9,9},
	{2,3,6,9,9,9,9,9,9},
	{0,1,3,6,9,9,9,9,9},
	{1,3,6,9,9,9,9,9,9},
	{0,3,6,9,9,9,9,9,9},
	{3,6,9,9,9,9,9,9,9},
	{0,1,2,6,9,9,9,9,9},
	{1,2,6,9,9,9,9,9,9},
	{0,2,6,9,9,9,9,9,9},
	{2,6,9,9,9,9,9,9,9},
	{0,1,6,9,9,9,9,9,9},
	{1,6,9,9,9,9,9,9,9},
	{0,6,9,9,9,9,9,9,9},
	{6,9,9,9,9,9,9,9,9},
	{0,1,2,3,4,5,9,9,9},
	{1,2,3,4,5,9,9,9,9},
	{0,2,3,4,5,9,9,9,9},
	{2,3,4,5,9,9,9,9,9},
	{0,1,3,4,5,9,9,9,9},
	{1,3,4,5,9,9,9,9,9},
	{0,3,4,5,9,9,9,9,9},
	{3,4,5,9,9,9,9,9,9},
	{0,1,2,4,5,9,9,9,9},
	{1,2,4,5,9,9,9,9,9},
	{0,2,4,5,9,9,9,9,9},
	{2,4,5,9,9,9,9,9,9},
	{0,1,4,5,9,9,9,9,9},
	{1,4,5,9,9,9,9,9,9},
	{0,4,5,9,9,9,9,9,9},
	{4,5,9,9,9,9,9,9,9},
	{0,1,2,3,5,9,9,9,9},
	{1,2,3,5,9,9,9,9,9},
	{0,2,3,5,9,9,9,9,9},
	{2,3,5,9,9,9,9,9,9},
	{0,1,3,5,9,9,9,9,9},
	{1,3,5,9,9,9,9,9,9},
	{0,3,5,9,9,9,9,9,9},
	{3,5,9,9,9,9,9,9,9},
	{0,1,2,5,9,9,9,9,9},
	{1,2,5,9,9,9,9,9,9},
	{0,2,5,9,9,9,9,9,9},
	{2,5,9,9,9,9,9,9,9},
	{0,1,5,9,9,9,9,9,9},
	{1,5,9,9,9,9,9,9,9},
	{0,5,9,9,9,9,9,9,9},
	{5,9,9,9,9,9,9,9,9},
	{0,1,2,3,4,9,9,9,9},
	{1,2,3,4,9,9,9,9,9},
	{0,2,3,4,9,9,9,9,9},
	{2,3,4,9,9,9,9,9,9},
	{0,1,3,4,9,9,9,9,9},
	{1,3,4,9,9,9,9,9,9},
	{0,3,4,9,9,9,9,9,9},
	{3,4,9,9,9,9,9,9,9},
	{0,1,2,4,9,9,9,9,9},
	{1,2,4,9,9,9,9,9,9},
	{0,2,4,9,9,9,9,9,9},
	{2,4,9,9,9,9,9,9,9},
	{0,1,4,9,9,9,9,9,9},
	{1,4,9,9,9,9,9,9,9},
	{0,4,9,9,9,9,9,9,9},
	{4,9,9,9,9,9,9,9,9},
	{0,1,2,3,9,9,9,9,9},
	{1,2,3,9,9,9,9,9,9},
	{0,2,3,9,9,9,9,9,9},
	{2,3,9,9,9,9,9,9,9},
	{0,1,3,9,9,9,9,9,9},
	{1,3,9,9,9,9,9,9,9},
	{0,3,9,9,9,9,9,9,9},
	{3,9,9,9,9,9,9,9,9},
	{0,1,2,9,9,9,9,9,9},
	{1,2,9,9,9,9,9,9,9},
	{0,2,9,9,9,9,9,9,9},
	{2,9,9,9,9,9,9,9,9},
	{0,1,9,9,9,9,9,9,9},
	{1,9,9,9,9,9,9,9,9},
	{0,9,9,9,9,9,9,9,9}
};

static constexpr short NEXT_MINIBOARD[BOARD_DIM][BOARD_DIM] = { 0,1,2,0,1,2,0,1,2,3,4,5,3,4,5,3,4,5,6,7,8,6,7,8,6,7,8,0,1,2,0,1,2,0,1,2,3,4,5,3,4,5,3,4,5,6,7,8,6,7,8,6,7,8,0,1,2,0,1,2,0,1,2,3,4,5,3,4,5,3,4,5,6,7,8,6,7,8,6,7,8 };
static constexpr short CURR_MINIBOARD[BOARD_DIM][BOARD_DIM] = { 0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,3,3,3,4,4,4,5,5,5,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,6,6,6,7,7,7,8,8,8,6,6,6,7,7,7,8,8,8 };

static constexpr short WIN_BOARDS[ALL_POSSOBLE_FILLED_BOARDS] = {
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,1,1,0,0,0,0,
	0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,
	0,1,0,1,0,1,0,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,0,
	1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
	0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1
};

static constexpr short ONES_IN_BOARD[ALL_POSSOBLE_FILLED_BOARDS] = {
	0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,
	4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,
	4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,
	5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,
	4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,5,6,6,7,6,
	7,7,8,6,7,7,8,7,8,8
};

static constexpr short ZEROS_IN_BOARD[ALL_POSSOBLE_FILLED_BOARDS]		= { 9,8,8,7,8,7,7,6,8,7,7,6,7,6,6,5,8,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4,8,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,8,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,8,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,5,4,4,3,4,3,3,2,4,3,3,2,3,2,2,1,8,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,5,4,4,3,4,3,3,2,4,3,3,2,3,2,2,1,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,5,4,4,3,4,3,3,2,4,3,3,2,3,2,2,1,6,5,5,4,5,4,4,3,5,4,4,3,4,3,3,2,5,4,4,3,4,3,3,2,4,3,3,2,3,2,2,1,5,4,4,3,4,3,3,2,4,3,3,2,3,2,2,1,4,3,3,2,3,2,2,1,3,2,2,1,2,1,1 };
static constexpr short BEST_WINNING_MOVES[ALL_POSSOBLE_FILLED_BOARDS]	= { 9,9,9,2,9,1,0,3,9,6,9,2,9,1,0,6,9,8,7,2,6,1,0,6,5,5,5,2,5,6,0,6,9,9,9,2,8,1,0,8,4,4,4,2,4,1,0,4,3,3,3,2,3,8,0,8,0,6,7,2,6,6,0,6,9,3,9,2,4,1,0,3,0,1,0,2,0,1,0,4,2,2,2,2,0,1,0,3,0,2,0,2,0,1,0,5,9,3,9,2,4,1,0,3,0,4,0,2,4,4,0,4,2,3,2,2,3,3,0,3,0,2,0,2,0,8,0,8,9,9,4,2,9,1,0,4,9,6,4,2,9,1,0,4,1,1,0,2,1,1,0,6,1,1,5,2,1,1,0,6,9,9,4,2,8,1,0,4,4,4,4,4,4,1,4,4,1,1,3,2,1,1,0,8,1,1,0,2,1,1,0,6,8,3,4,2,4,1,4,4,0,8,0,2,0,1,0,4,1,8,2,2,1,1,0,8,0,8,0,2,0,1,0,8,8,3,4,2,8,8,4,4,0,4,4,4,4,4,4,4,1,3,2,2,8,8,8,8,0,8,0,2,8,8,0,8,9,4,9,2,5,1,0,4,9,4,9,2,5,1,0,4,0,1,0,2,0,1,0,5,0,5,0,2,5,5,0,5,2,2,2,2,0,1,0,4,2,4,2,2,4,4,0,4,0,2,0,2,0,1,0,3,0,2,0,2,0,6,0,6,7,3,7,2,4,4,0,4,0,4,0,2,0,4,0,4,0,2,7,2,0,1,0,7,0,2,0,2,0,5,0,5,2,2,2,2,4,4,0,4,0,4,0,2,4,4,0,4,2,2,2,2,0,3,0,3,0,2,0,2,0,1,0,7,6,4,4,4,5,1,0,4,6,6,4,4,5,6,0,4,0,1,0,2,6,1,0,6,0,6,0,6,5,6,0,6,2,2,2,2,6,1,0,4,2,4,4,4,4,4,4,4,0,1,0,2,6,1,0,6,0,6,0,2,6,6,0,6,0,3,4,4,4,4,4,4,0,4,0,4,0,4,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,2,2,2,2,4,4,4,4,0,4,4,4,4,4,4,4,2,2,2,2,0,1,0,3,0,2,0,2,0,1,0 };
static constexpr int PLAYER_SQUARES_COUNT[ALL_POSSOBLE_FILLED_BOARDS]	= { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,5,6,6,7,6,7,7,8,6,7,7,8,7,8,8 };

enum class BoardStatus {
	INVALID = -1,
	IN_PROGRESS,
	DRAW,
	OPPONENT_WON,
	I_WON,
};

float invSqrt(float x) {
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x * (1.5f - xhalf * x*x);
	return x;
}

static unsigned int g_seed;

inline void fast_srand(int seed) {
	g_seed = seed;
}

inline int fast_rand(void) {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

typedef char Coord;
const Coord INVALID_COORD = -1;

class Coords {
public:
	Coords();
	Coords(const Coords& rhs);
	Coords(Coord rowCoord, Coord colCoord);
	Coord getRowCoord() const { return rowCoord; }
	Coord getColCoord() const { return colCoord; }
	void setRowCoord(Coord rowCoord) { this->rowCoord = rowCoord; }
	void setColCoord(Coord colCoord) { this->colCoord = colCoord; }
	Coords& operator=(const Coords& rhs);
	bool operator==(const Coords& rhs);
	bool isValid() const;
	friend ostream& operator<<(ostream& stream, const Coords& coords);

	short getNextMiniBoard() const {
		return NEXT_MINIBOARD[rowCoord][colCoord];
	}

	short getCurrMiniBoard() const {
		return CURR_MINIBOARD[rowCoord][colCoord];
	}

private:
	Coord rowCoord;
	Coord colCoord;
};

Coords::Coords() :
	rowCoord{ INVALID_COORD },
	colCoord{ INVALID_COORD }
{}

Coords::Coords(const Coords& rhs) :
	rowCoord{ rhs.rowCoord },
	colCoord{ rhs.colCoord }
{}

Coords::Coords(
	Coord rowCoord,
	Coord colCoord
) :
	rowCoord{ rowCoord },
	colCoord{ colCoord }
{}

Coords& Coords::operator=(const Coords& rhs) {
	rowCoord = rhs.rowCoord;
	colCoord = rhs.colCoord;
	return *this;
}

bool Coords::operator==(const Coords& rhs) {
	return (rowCoord == rhs.rowCoord) && (colCoord == rhs.colCoord);
}

bool Coords::isValid() const {
	return INVALID_COORD != rowCoord && INVALID_COORD != colCoord;
}

ostream& operator<<(ostream& stream, const Coords& coords) {
	stream << static_cast<int>(coords.rowCoord) << SPACE << static_cast<int>(coords.colCoord);
	return stream;
}

Coords BIG_BOARD_POSITIONS[BOARD_DIM][BOARD_DIM] = {
	Coords{ 0, 0 },
	Coords{ 0, 1 },
	Coords{ 0, 2 },
	Coords{ 1, 0 },
	Coords{ 1, 1 },
	Coords{ 1, 2 },
	Coords{ 2, 0 },
	Coords{ 2, 1 },
	Coords{ 2, 2 },
	Coords{ 0, 3 },
	Coords{ 0, 4 },
	Coords{ 0, 5 },
	Coords{ 1, 3 },
	Coords{ 1, 4 },
	Coords{ 1, 5 },
	Coords{ 2, 3 },
	Coords{ 2, 4 },
	Coords{ 2, 5 },
	Coords{ 0, 6 },
	Coords{ 0, 7 },
	Coords{ 0, 8 },
	Coords{ 1, 6 },
	Coords{ 1, 7 },
	Coords{ 1, 8 },
	Coords{ 2, 6 },
	Coords{ 2, 7 },
	Coords{ 2, 8 },
	Coords{ 3, 0 },
	Coords{ 3, 1 },
	Coords{ 3, 2 },
	Coords{ 4, 0 },
	Coords{ 4, 1 },
	Coords{ 4, 2 },
	Coords{ 5, 0 },
	Coords{ 5, 1 },
	Coords{ 5, 2 },
	Coords{ 3, 3 },
	Coords{ 3, 4 },
	Coords{ 3, 5 },
	Coords{ 4, 3 },
	Coords{ 4, 4 },
	Coords{ 4, 5 },
	Coords{ 5, 3 },
	Coords{ 5, 4 },
	Coords{ 5, 5 },
	Coords{ 3, 6 },
	Coords{ 3, 7 },
	Coords{ 3, 8 },
	Coords{ 4, 6 },
	Coords{ 4, 7 },
	Coords{ 4, 8 },
	Coords{ 5, 6 },
	Coords{ 5, 7 },
	Coords{ 5, 8 },
	Coords{ 6, 0 },
	Coords{ 6, 1 },
	Coords{ 6, 2 },
	Coords{ 7, 0 },
	Coords{ 7, 1 },
	Coords{ 7, 2 },
	Coords{ 8, 0 },
	Coords{ 8, 1 },
	Coords{ 8, 2 },
	Coords{ 6, 3 },
	Coords{ 6, 4 },
	Coords{ 6, 5 },
	Coords{ 7, 3 },
	Coords{ 7, 4 },
	Coords{ 7, 5 },
	Coords{ 8, 3 },
	Coords{ 8, 4 },
	Coords{ 8, 5 },
	Coords{ 6, 6 },
	Coords{ 6, 7 },
	Coords{ 6, 8 },
	Coords{ 7, 6 },
	Coords{ 7, 7 },
	Coords{ 7, 8 },
	Coords{ 8, 6 },
	Coords{ 8, 7 },
	Coords{ 8, 8 }
};

class Board {
public:
	Board();
	Board(const Board& rhs);

	void setStatus(const BoardStatus status);
	void setPlayer(const int player);
	void setMove(const Coords move);

	BoardStatus getStatus() const;
	int getPlayer() const;
	Coords getMove() const;

	void init();
	void copy(const Board& rhs);
	int getPlayerIdx(const Coords pos) const;
	void setPlayerIdx(const Coords pos, const short miniBoardIdx, const int playerIdx);
	void playMove(const Coords move, short (&bigBoard)[SQUARE_TYPES], short& bigBoardDraw);
	void playMove(const Coords moveToPlay, short (&bigBoard)[SQUARE_TYPES], short& bigBoardDraw, Coords& prevMove, int& player);
	Coords getRandomMove(const Coords move, const short (&bigBoard)[SQUARE_TYPES], short bigBoardDraw) const;
	Coords getRandomMoveForBoard(const int miniBoardIdx, const short board) const;
	void getAllPossibleMoves(Coords (&allMoves)[ALL_SQUARES], int& allMovesCount) const;
	int togglePlayer(const int playerToToggle) const;
	int simulateRandomGame();
	void constructBigBoard(short(&bigBoard)[SQUARE_TYPES], short& bigBoardDraw);
	int terminal(const short(&bigBoard)[SQUARE_TYPES]);

	friend ostream& operator<<(std::ostream& stream, const Board& board);

private:
	bool notPlayableMiniBoard(const int miniBoardIdx) const;
	BoardStatus resolveDraw(const short (&bigBoard)[SQUARE_TYPES]) const;
	short getBigBoardMask() const;

	short board[SQUARE_TYPES][BOARD_DIM];
	unsigned short flags;
};

Board::Board() {
	init();
}

Board::Board(const Board& rhs) {
	copy(rhs);
}

void Board::setStatus(const BoardStatus status) {
	unsigned statusToSet = static_cast<unsigned>(status);
	statusToSet <<= STATUS_OFFSET;

	flags &= ~STATUS_MASK; // First zero out the current status
	flags |= statusToSet;
}

void Board::setPlayer(const int player) {
	flags &= ~PLAYER_FLAG; // First zero out the player flag
	flags |= player;
}

void Board::setMove(Coords move) {
	unsigned rowToSet = static_cast<unsigned>(move.getRowCoord());
	unsigned colToSet = static_cast<unsigned>(move.getColCoord());

	rowToSet <<= MOVE_ROW_OFFSET;
	colToSet <<= MOVE_COL_OFFSET;

	flags &= ~MOVE_ROW_MASK; // Zero out the row
	flags &= ~MOVE_COL_MASK; // Zero out the col

	flags |= rowToSet;
	flags |= colToSet;
}

BoardStatus Board::getStatus() const {
	return static_cast<BoardStatus>((STATUS_MASK & flags) >> STATUS_OFFSET);
}

int Board::getPlayer() const {
	return static_cast<int>(PLAYER_FLAG & flags);
}

Coords Board::getMove() const {
	Coords res;
	res.setRowCoord((flags & MOVE_ROW_MASK) >> MOVE_ROW_OFFSET);
	res.setColCoord((flags & MOVE_COL_MASK) >> MOVE_COL_OFFSET);
	return res;
}

void Board::init() {
	for (int sqType = 0; sqType < SQUARE_TYPES; ++sqType) {
		for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
			board[sqType][miniBoardIdx] = EMPTY_TICTACTOE_BOARD;
		}
	}

	flags = 0;
	setStatus(BoardStatus::IN_PROGRESS);
}

void Board::copy(const Board& rhs) {
	this->flags = rhs.flags;
	memcpy(&board[0][0], &rhs.board[0][0], 36);
}

int Board::getPlayerIdx(const Coords pos) const {
	int playerIdx = INVALID_IDX;
	const short miniBoardIdx = pos.getCurrMiniBoard();
	const short miniBoardInnerIdx = pos.getNextMiniBoard();
	const short miniBoardXes = board[0][miniBoardIdx];
	const short miniBoardOs = board[1][miniBoardIdx];
	if (miniBoardXes & (1 << miniBoardInnerIdx)) {
		playerIdx = MY_PLAYER_IDX;
	}
	else if (miniBoardOs & (1 << miniBoardInnerIdx)) {
		playerIdx = OPPONENT_PLAYER_IDX;
	}

	return playerIdx;
}

void Board::setPlayerIdx(const Coords pos, const short miniBoardIdx, const int playerIdx) {
	const short miniBoardInnerIdx = pos.getNextMiniBoard();
	board[playerIdx][miniBoardIdx] |= 1 << miniBoardInnerIdx;
}

void Board::playMove(const Coords move, short(&bigBoard)[SQUARE_TYPES], short& bigBoardDraw) {
	const int player = getPlayer();
	const short miniBoardIdx = move.getCurrMiniBoard();

	setMove(move);
	setPlayerIdx(move, miniBoardIdx, player);
	const short miniBoard = board[player][miniBoardIdx];

	const int boardFull = FULL_BOARD_MASK == (board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx]);

	bigBoard[player] |= WIN_BOARDS[miniBoard] << miniBoardIdx;
	bigBoardDraw |= boardFull << miniBoardIdx;

	if (WIN_BOARDS[bigBoard[player]]) {
		setStatus((MY_PLAYER_IDX == player) ? BoardStatus::I_WON : BoardStatus::OPPONENT_WON);
	}
	else if (FULL_BOARD_MASK == (bigBoard[MY_PLAYER_IDX] | bigBoard[OPPONENT_PLAYER_IDX] | bigBoardDraw)) {
		setStatus(resolveDraw(bigBoard));
	}

	setPlayer(togglePlayer(player));
}

void Board::playMove(const Coords moveToPlay, short(&bigBoard)[SQUARE_TYPES], short& bigBoardDraw, Coords& prevMove, int& player) {
	const short miniBoardIdx = moveToPlay.getCurrMiniBoard();

	prevMove = moveToPlay;
	setPlayerIdx(moveToPlay, miniBoardIdx, player);
	const short miniBoard = board[player][miniBoardIdx];

	const int boardFull = FULL_BOARD_MASK == (board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx]);

	bigBoard[player] |= WIN_BOARDS[miniBoard] << miniBoardIdx;
	bigBoardDraw |= boardFull << miniBoardIdx;

	if (WIN_BOARDS[bigBoard[player]]) {
		setStatus((MY_PLAYER_IDX == player) ? BoardStatus::I_WON : BoardStatus::OPPONENT_WON);
	}
	else if (FULL_BOARD_MASK == (bigBoard[MY_PLAYER_IDX] | bigBoard[OPPONENT_PLAYER_IDX] | bigBoardDraw)) {
		setStatus(resolveDraw(bigBoard));
	}

	player = togglePlayer(player);
}

Coords Board::getRandomMove(const Coords move, const short(&bigBoard)[SQUARE_TYPES], short bigBoardDraw) const {
	int miniBoardIdx = move.getNextMiniBoard();

	if (notPlayableMiniBoard(miniBoardIdx)) {
		const short boardMask = bigBoard[MY_PLAYER_IDX] | bigBoard[OPPONENT_PLAYER_IDX] | bigBoardDraw;
		miniBoardIdx = ALL_MOVES[boardMask][fast_rand() % ZEROS_IN_BOARD[boardMask]];
	}

	return getRandomMoveForBoard(miniBoardIdx, board[0][miniBoardIdx] | board[1][miniBoardIdx]);
}

int Board::simulateRandomGame() {
	//cerr << *this << endl;

	short bigBoard[SQUARE_TYPES] = { 0, 0 };
	short bigBoardDraw = 0;
	constructBigBoard(bigBoard, bigBoardDraw);
	Coords move = getMove();
	int player = getPlayer();

	while (BoardStatus::IN_PROGRESS == getStatus()) {
		playMove(getRandomMove(move, bigBoard, bigBoardDraw), bigBoard, bigBoardDraw, move, player);

		//cerr << *this << endl;
	}

	int res = INVALID_IDX;
	const BoardStatus status = getStatus();
	if (BoardStatus::I_WON == status) {
		res = MY_PLAYER_IDX;
	}
	else if (BoardStatus::OPPONENT_WON == status) {
		res = OPPONENT_PLAYER_IDX;
	}

	return res;
}

Coords Board::getRandomMoveForBoard(const int miniBoardIdx, const short board) const {
	//Coords winnigMove;
	//
	//for (int moveIdx = 0; moveIdx < BOARD_DIM; ++moveIdx) {
	//	const int miniBoardMove = ALL_MOVES[board][moveIdx];
	//	if (BOARD_DIM != miniBoardMove) {
	//		const Coords coordsToCheck = BIG_BOARD_POSITIONS[miniBoardIdx][moveIdx];
	//		Board boardToCheck{ *this };
	//		short bigBoard[SQUARE_TYPES] = { 0, 0 };
	//		short bigBoardDraw = 0;
	//		boardToCheck.constructBigBoard(bigBoard, bigBoardDraw);
	//		boardToCheck.playMove(coordsToCheck, bigBoard, bigBoardDraw);
	//
	//		const bool myPlayerWon = (BoardStatus::I_WON == boardToCheck.getStatus()) && (MY_PLAYER_IDX == getPlayer());
	//		const bool opponentWon = (BoardStatus::OPPONENT_WON == boardToCheck.getStatus()) && (OPPONENT_PLAYER_IDX == getPlayer());
	//
	//		if (myPlayerWon || opponentWon) {
	//			winnigMove = coordsToCheck;
	//			break;
	//		}
	//	}
	//}
	//
	//if (winnigMove.isValid()) {
	//	return winnigMove;
	//}
	//else
	if (BOARD_DIM != BEST_WINNING_MOVES[board]) {
		return BIG_BOARD_POSITIONS[miniBoardIdx][BEST_WINNING_MOVES[board]];
	}
	else {
		return BIG_BOARD_POSITIONS[miniBoardIdx][ALL_MOVES[board][fast_rand() % ZEROS_IN_BOARD[board]]];
	}
}

void Board::getAllPossibleMoves(Coords (&allMoves)[ALL_SQUARES], int& allMovesCount) const {
	allMovesCount = 0;

	const int miniBoardIdx = getMove().getNextMiniBoard(); // Current moves detemine the next mini board
	if (!notPlayableMiniBoard(miniBoardIdx)) {
		const short boardMask = board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx];
		for (int idx = 0; idx < ZEROS_IN_BOARD[boardMask]; ++idx) {
			const int moveIdx = ALL_MOVES[boardMask][idx];
			allMoves[allMovesCount] = BIG_BOARD_POSITIONS[miniBoardIdx][moveIdx];
			++allMovesCount;
		}
	}
	else {
		const short bigBoardMask = getBigBoardMask();
		for (int idx = 0; idx < ZEROS_IN_BOARD[bigBoardMask]; ++idx) {
			const int miniBoardIdx = ALL_MOVES[bigBoardMask][idx];

			const short boardMask = board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx];
			for (int idx = 0; idx < ZEROS_IN_BOARD[boardMask]; ++idx) {
				const int moveIdx = ALL_MOVES[boardMask][idx];
				allMoves[allMovesCount] = BIG_BOARD_POSITIONS[miniBoardIdx][moveIdx];
				++allMovesCount;
			}
		}
	}
}

int Board::togglePlayer(const int playerToToggle) const {
	return PLAYER_TOGGLE - (playerToToggle + 1);
}

bool Board::notPlayableMiniBoard(const int miniBoardIdx) const {
	const bool myPlayerWins = WIN_BOARDS[board[MY_PLAYER_IDX][miniBoardIdx]];
	const bool opponentPlayerWins = WIN_BOARDS[board[OPPONENT_PLAYER_IDX][miniBoardIdx]];
	const bool draw = FULL_BOARD_MASK == (board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx]);

	return myPlayerWins || opponentPlayerWins || draw;
}

BoardStatus Board::resolveDraw(const short(&bigBoard)[SQUARE_TYPES]) const {
	int mineMiniBoardsWon = PLAYER_SQUARES_COUNT[bigBoard[MY_PLAYER_IDX]];
	int opponentMiniBoardsWon = PLAYER_SQUARES_COUNT[bigBoard[OPPONENT_PLAYER_IDX]];

	BoardStatus status = BoardStatus::DRAW;

	if (opponentMiniBoardsWon > mineMiniBoardsWon) {
		status = BoardStatus::OPPONENT_WON;
	}
	else if (opponentMiniBoardsWon < mineMiniBoardsWon) {
		status = BoardStatus::I_WON;
	}

	return status;
}

short Board::getBigBoardMask() const {
	short playerBoardToCheck = 0;
	short opponentBoardToCheck = 0;
	short drawBoards = 0;

	for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
		playerBoardToCheck |= WIN_BOARDS[board[MY_PLAYER_IDX][miniBoardIdx]] << miniBoardIdx;
		opponentBoardToCheck |= WIN_BOARDS[board[OPPONENT_PLAYER_IDX][miniBoardIdx]] << miniBoardIdx;

		short wholeBoard = board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx];
		drawBoards |= (static_cast<int>(FULL_BOARD_MASK == wholeBoard) << miniBoardIdx);
	}

	return playerBoardToCheck | opponentBoardToCheck | drawBoards;
}

void Board::constructBigBoard(short (&bigBoard)[SQUARE_TYPES], short& bigBoardDraw) {
	for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
		bigBoard[MY_PLAYER_IDX] |= WIN_BOARDS[board[MY_PLAYER_IDX][miniBoardIdx]] << miniBoardIdx;
		bigBoard[OPPONENT_PLAYER_IDX] |= WIN_BOARDS[board[OPPONENT_PLAYER_IDX][miniBoardIdx]] << miniBoardIdx;

		short wholeBoard = board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx];
		bigBoardDraw |= (static_cast<int>(FULL_BOARD_MASK == wholeBoard) << miniBoardIdx);
	}
}

int Board::terminal(const short(&bigBoard)[SQUARE_TYPES]) {
	int res = INVALID_IDX;

	if (abs(ONES_IN_BOARD[bigBoard[0]] - ONES_IN_BOARD[bigBoard[1]]) > 1) {
		res = ONES_IN_BOARD[bigBoard[0]] > ONES_IN_BOARD[bigBoard[1]] ? 0 : 1;
	}

	return res;
}

ostream& operator<<(std::ostream& stream, const Board& board) {
	for (Coord rowIdx = 0; rowIdx < BOARD_DIM + 1; ++rowIdx) {
		if (rowIdx > 0 && 0 == rowIdx % TRIPLE) {
			stream << endl;
		}

		for (Coord colIdx = 0; colIdx < BOARD_DIM + 1; ++colIdx) {
			if (0 == colIdx % TRIPLE) {
				stream << SPACE << SPACE << SPACE;
			}

			if (BOARD_DIM == rowIdx) {
				stream << static_cast<int>(colIdx);
			}
			else if (BOARD_DIM == colIdx) {
				stream << static_cast<int>(rowIdx);
			}
			else {
				const int playerIdx = board.getPlayerIdx({ rowIdx, colIdx });

				switch (playerIdx) {
					case MY_PLAYER_IDX: { stream << MY_PLAYER_CHAR; break; }
					case OPPONENT_PLAYER_IDX: { stream << OPPONENT_PLAYER_CHAR; break; }
					default: { stream << EMPTY_CHAR; break; }
				}
			}
		}

		stream << endl;
	}
	return stream;
}

class Node {
public:
	Node(const Board& board, const float visits, const float winScore, const int parentIdx);
	void setBoard(const Board& board) { this->board = board; }
	void setVisits(const float visits) { this->visits = visits; }
	void setWinScore(const float winScore) { this->winScore = winScore; }
	const Board& getBoard() const { return board; }
	Board& getBoard() { return board; }
	float getVisits() const { return visits; }
	float getWinScore() const { return winScore; }
	float getUCTValue() const { return uctValue; }
	int getFirstChild() const { return firstChild; }
	int getParentIdx() const { return parentIdx; }
	int getChildrenCount() const { return childrenCount; }

	void addChild(const int childIdxNode);
	void uct(const float parentVisits);
	void incrementVisits();

private:
	Board board;
	float visits;
	float winScore;
	float uctValue;
	int firstChild;
	int parentIdx;
	char childrenCount;
};

Node::Node(const Board& board, const float visits, const float winScore, const int parentIdx) :
	board{ board },
	visits{ visits },
	winScore{ winScore },
	uctValue{ 3.40282e+38f },
	firstChild{ INVALID_IDX},
	parentIdx{ parentIdx },
	childrenCount{ 0 }
{}

void Node::addChild(const int childIdxNode) {
	if (0 == childrenCount) {
		firstChild = childIdxNode;
	}
	++childrenCount;
}

void Node::uct(const float parentVisits) {
	if (visits > 0.f) {
		const float winVisitsRatio = winScore / visits;
		//const float confidentRatio = 1.41421f * (1.f / invSqrt(logf(parentVisits) / visits));
		const float confidentRatio = 1.41421f * (sqrtf(logf(parentVisits) / visits));

		uctValue = winVisitsRatio + confidentRatio;
	}
}

void Node::incrementVisits() {
	visits += 1.f;
}

class Tree {
public:
	int getNodesCount() const { return nodesCount; }
	const Node& getNode(const int nodeIdx) const { return nodes[nodeIdx]; }
	Node& getNode(const int nodeIdx) { return nodes[nodeIdx]; }

	void init(const Board& initialBoard);
	void setRootPlayer(const int playerIdx);
	int addNode(const Node& node);

	void debug() const;

private:
	vector<Node> nodes;
	int nodesCount;
};

void Tree::init(const Board& initialBoard) {
	const bool nodesAlreadyReserved = nodes.size() > 1;

	nodes.clear();

	if (!nodesAlreadyReserved) {
		nodes.reserve(NODES_TO_RESERVE);
	}

	Node rootNode{ initialBoard, 0.f, 0.f, INVALID_IDX };
	nodes.emplace_back(rootNode);
	nodesCount = 1;
}

void Tree::setRootPlayer(const int playerIdx) {
	nodes[0].getBoard().setPlayer(playerIdx);
}

int Tree::addNode(const Node& node) {
	nodes.emplace_back(node);
	++nodesCount;

	return nodesCount - 1;
}

void Tree::debug() const {
	for (int nodeIdx = 0; nodeIdx < nodesCount; ++nodeIdx) {
		cout << "Node[" << nodeIdx << "] = {" << endl;
		cout << "\tmove = (" << static_cast<int>(nodes[nodeIdx].getBoard().getMove().getRowCoord());
		cout << ", ";
		cout << static_cast<int>(nodes[nodeIdx].getBoard().getMove().getColCoord()) << ")" << endl;
		cout << "\tvisits = " << nodes[nodeIdx].getVisits() << endl;
		cout << "\twinScore = " << nodes[nodeIdx].getWinScore() << endl;
		cout << "\tfirstChild = " << nodes[nodeIdx].getFirstChild() << endl;
		cout << "\tparentIdx = " << nodes[nodeIdx].getParentIdx() << endl;
		cout << "\tchildrenCoun = " << nodes[nodeIdx].getChildrenCount() << endl;
		cout << "};" << endl;
		cout << endl;
	}
}

class MonteCarloTreeSearch {
public:
	MonteCarloTreeSearch(Board& initialBoard);
	void setOpponentMove(const Coords opponentMove) { this->opponentMove = opponentMove; }
	void setTimeLimit(long long timeLimit) { this->timeLimit = timeLimit; }
	Coords getBestMove() const { return bestMove; }
	int getNodesCount() const { return searchTree.getNodesCount(); }
	void solve(const int turnIdx);
	void setRootPlayer(const int playerIdx);

	void debug() const;

private:
	int preporcessMoves(const Board& parentBoard, Coords(&allMoves)[ALL_SQUARES], const int allMovesCount) const;
	int selectPromisingNode();
	void expansion(const int selectedNode, Coords(&allMoves)[ALL_SQUARES], int& allMovesCount);
	int simulation(const int nodeToExploreIdx);
	void backPropagation(const int nodeToExploreIdx, const int simulationResult);
	void searchBegin(const int turnIdx);
	void searchEnd(const int turnIdx, Coords(&allMoves)[ALL_SQUARES], int& allMovesCount);

private:
	Tree searchTree;
	Coords opponentMove;
	Coords bestMove;
	Board& initialBoard;
	long long timeLimit;
	int turnRootNodeIdx;
};

MonteCarloTreeSearch::MonteCarloTreeSearch(Board& initialBoard) :
	initialBoard{ initialBoard },
	timeLimit{ 0 },
	turnRootNodeIdx{ 0 }
{
	searchTree.init(initialBoard);
}

void MonteCarloTreeSearch::solve(const int turnIdx) {
	searchBegin(turnIdx);

	Coords allMoves[ALL_SQUARES]; // Reuse array
	int allMovesCount;

	int iteration = 0;

	chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	const chrono::steady_clock::time_point loopEnd = start + chrono::milliseconds{ timeLimit };
	
	for (chrono::steady_clock::time_point now = start; now < loopEnd; now = std::chrono::steady_clock::now()) {
	//while (iteration < 5) {
		int selectedNodeIdx = selectPromisingNode();
		const Node& selectedNode = searchTree.getNode(selectedNodeIdx);

		if (BoardStatus::IN_PROGRESS == selectedNode.getBoard().getStatus()) {
			expansion(selectedNodeIdx, allMoves, allMovesCount);
			selectedNodeIdx = selectedNode.getFirstChild() + (fast_rand() % selectedNode.getChildrenCount());
			searchTree.getNode(selectedNodeIdx).incrementVisits();
		}

		int victoriousPlayer = simulation(selectedNodeIdx);
		backPropagation(selectedNodeIdx, victoriousPlayer);

		++iteration;
	}

	cerr << "MCTS iterations: " << iteration << endl;
	cerr << "Nodes count: " << searchTree.getNodesCount() << endl << endl;

	searchEnd(turnIdx, allMoves, allMovesCount);
}

void MonteCarloTreeSearch::setRootPlayer(const int playerIdx) {
	searchTree.setRootPlayer(playerIdx);
}

void MonteCarloTreeSearch::debug() const {
	cout << "SEARCH_END" << endl;
	searchTree.debug();
}

int MonteCarloTreeSearch::preporcessMoves(const Board& parentBoard, Coords(&allMoves)[ALL_SQUARES], const int allMovesCount) const {
	int winningMoveIdx = INVALID_IDX;

	for (int moveIdx = 0; moveIdx < allMovesCount; ++moveIdx) {
		Board boardToCheck{ parentBoard };

		short bigBoard[SQUARE_TYPES] = { 0, 0 };
		short bigBoardDraw = 0;
		boardToCheck.constructBigBoard(bigBoard, bigBoardDraw);
		boardToCheck.playMove(allMoves[moveIdx], bigBoard, bigBoardDraw);

		const bool myPlayerWon = (BoardStatus::I_WON == boardToCheck.getStatus()) && (MY_PLAYER_IDX == parentBoard.getPlayer());
		const bool opponentWon = (BoardStatus::OPPONENT_WON == boardToCheck.getStatus()) && (OPPONENT_PLAYER_IDX == parentBoard.getPlayer());

		if (myPlayerWon || opponentWon) {
			winningMoveIdx = moveIdx;
			break;
		}
	}

	return winningMoveIdx;
}

int MonteCarloTreeSearch::selectPromisingNode() {
	int currentNodeIdx = turnRootNodeIdx;

	while (searchTree.getNode(currentNodeIdx).getChildrenCount() > 0) {
		Node& currentNode = searchTree.getNode(currentNodeIdx);
		currentNode.incrementVisits();
		const float parentVisits = currentNode.getVisits();
		const int nodeFirstChild = currentNode.getFirstChild();

		float maxUCT = -1.0;
		for (int childIdx = 0; childIdx < currentNode.getChildrenCount(); ++childIdx) {
			const int childNodeIdx = nodeFirstChild + childIdx;
			const float childUCT = searchTree.getNode(childNodeIdx).getUCTValue();

			if (childUCT > maxUCT) {
				maxUCT = childUCT;
				currentNodeIdx = childNodeIdx;
			}
		}
	}

	searchTree.getNode(currentNodeIdx).incrementVisits();
	return currentNodeIdx;
}

void MonteCarloTreeSearch::expansion(const int selectedNode, Coords(&allMoves)[ALL_SQUARES], int& allMovesCount) {
	Node& parentNode = searchTree.getNode(selectedNode);
	const Board& parentBoard = parentNode.getBoard();
	parentBoard.getAllPossibleMoves(allMoves, allMovesCount);
	
	const int winningMoveIdx = preporcessMoves(parentBoard, allMoves, allMovesCount);
	
	if (INVALID_IDX != winningMoveIdx) {
		Board childBoard{ parentBoard };

		short bigBoard[SQUARE_TYPES] = { 0, 0 };
		short bigBoardDraw = 0;
		childBoard.constructBigBoard(bigBoard, bigBoardDraw);
		childBoard.playMove(allMoves[winningMoveIdx], bigBoard, bigBoardDraw);

		const Node childNode{ childBoard, 0.f, 0.f, selectedNode };
		parentNode.addChild(searchTree.addNode(childNode));
	}
	else {
		for (int moveIdx = 0; moveIdx < allMovesCount; ++moveIdx) {
			Board childBoard{ parentBoard };

			short bigBoard[SQUARE_TYPES] = { 0, 0 };
			short bigBoardDraw = 0;
			childBoard.constructBigBoard(bigBoard, bigBoardDraw);
			childBoard.playMove(allMoves[moveIdx], bigBoard, bigBoardDraw);

			const Node childNode{ childBoard, 0.f, 0.f, selectedNode };
			parentNode.addChild(searchTree.addNode(childNode));
		}
	}
}

int MonteCarloTreeSearch::simulation(const int nodeToExploreIdx) {
	Board boardToSimulate = searchTree.getNode(nodeToExploreIdx).getBoard();
	return boardToSimulate.simulateRandomGame();
}

void MonteCarloTreeSearch::backPropagation(const int nodeToExploreIdx, const int victoriousPlayer) {
	int currentNodeIdx = nodeToExploreIdx;
	while (turnRootNodeIdx != currentNodeIdx) {
		Node& currentNode = searchTree.getNode(currentNodeIdx);

		int ownerPlayer = currentNode.getBoard().getPlayer();
		ownerPlayer = currentNode.getBoard().togglePlayer(ownerPlayer);
		if (ownerPlayer == victoriousPlayer) {
			currentNode.setWinScore(currentNode.getWinScore() + WIN_VALUE);
		}
		else if (ownerPlayer == INVALID_IDX) {
			//currentNode.setWinScore(currentNode.getWinScore() + DRAW_VALUE);
		}

		currentNode.uct(searchTree.getNode(currentNode.getParentIdx()).getVisits());
		currentNodeIdx = currentNode.getParentIdx();
	}
}

void MonteCarloTreeSearch::searchBegin(const int turnIdx) {
	if (0 == turnIdx || searchTree.getNodesCount() > MAX_NODES) {
		searchTree.init(initialBoard);
		searchTree.getNode(turnRootNodeIdx).setBoard(initialBoard);
		turnRootNodeIdx = 0;
	}
	else {
		const Node& currentRoot = searchTree.getNode(turnRootNodeIdx);
		const int currentRootFirstChild = currentRoot.getFirstChild();

		cerr << "searchBegin" << endl;
		cerr << "CURRENT turnRootNodeIdx: " << turnRootNodeIdx << SPACE << "(" << currentRoot.getBoard().getMove() << ")" << endl;
		cerr << "children:" << endl;

		for (int childIdx = 0; childIdx < currentRoot.getChildrenCount(); ++childIdx) {
			const Node& child = searchTree.getNode(currentRootFirstChild + childIdx);

			const Board& childBoard = child.getBoard();
			cerr << "[" << currentRootFirstChild + childIdx << ", (" << childBoard.getMove() << ")];\t";
			cerr << "UCT: " << child.getUCTValue() << ";\t";
			cerr << "Visits: " << child.getVisits() << ";\t";
			cerr << "WinScore: " << child.getWinScore() << endl;

			if (opponentMove == child.getBoard().getMove()) {
				turnRootNodeIdx = currentRootFirstChild + childIdx;
			}
		}

		cerr << "NEW turnRootNodeIdx: " << turnRootNodeIdx << SPACE << "(" << searchTree.getNode(turnRootNodeIdx).getBoard().getMove() << ")" << endl;
		cerr << endl;
		cerr << endl;
	}

	//cout << "turnRootNodeIdx = " << turnRootNodeIdx << endl;
}

void MonteCarloTreeSearch::searchEnd(const int turnIdx, Coords(&allMoves)[ALL_SQUARES], int& allMovesCount) {
	if (0 == turnIdx && !opponentMove.isValid()) {
		bestMove = { BOARD_DIM / 2, BOARD_DIM / 2 };
	}
	else {
		cerr << "searchEnd" << endl;
		cerr << "CURRENT turnRootNodeIdx: " << turnRootNodeIdx << SPACE << "(" << searchTree.getNode(turnRootNodeIdx).getBoard().getMove() << ")" << endl;
		cerr << "children:" << endl;

		const int rootFirstChild = searchTree.getNode(turnRootNodeIdx).getFirstChild();
		const int rootChildrenCount = searchTree.getNode(turnRootNodeIdx).getChildrenCount();

		if (rootChildrenCount > 0) {
			int bestChildIdx = rootFirstChild; // Score for all children may be 0.0
			float maxScore = 0.0;
			for (int childIdx = 0; childIdx < rootChildrenCount; ++childIdx) {
				const int childNodeIdx = rootFirstChild + childIdx;
				const float childScore = searchTree.getNode(childNodeIdx).getWinScore();
				if (childScore > maxScore) {
					maxScore = childScore;
					bestChildIdx = childNodeIdx;
				}

				const Node& childNode = searchTree.getNode(childNodeIdx);
				cerr << "[" << childNodeIdx << ", (" << childNode.getBoard().getMove() << ")];\t";
				cerr << "UCT: " << childNode.getUCTValue() << ";\t";
				cerr << "Visits: " << childNode.getVisits() << ";\t";
				cerr << "WinScore: " << childNode.getWinScore() << endl;
			}

			bestMove = searchTree.getNode(bestChildIdx).getBoard().getMove();
			turnRootNodeIdx = bestChildIdx;

			if (0 == searchTree.getNode(turnRootNodeIdx).getChildrenCount()) {
				expansion(turnRootNodeIdx, allMoves, allMovesCount);
			}

			cerr << "NEW turnRootNodeIdx: " << turnRootNodeIdx << SPACE << "(" << searchTree.getNode(turnRootNodeIdx).getBoard().getMove() << ")" << endl;
			cerr << endl;
			cerr << endl;
		}
	}

	//debug();
}

class Game {
public:
	Game();
	void initGame();
	void gameLoop();
	void getTurnInput();
	void turnBegin();
	void makeTurn();
	void turnEnd();
	void play();
private:
	Board board;
	MonteCarloTreeSearch monteCarloTreeSearch;
	Coords opponentMove;
	int turnsCount;
	int stopGame;
};

Game::Game() :
	board{},
	monteCarloTreeSearch{ board },
	opponentMove{},
	turnsCount{ 0 },
	stopGame{ false }
{}

void Game::initGame() {
	fast_srand(444);
	SQRT_2 = sqrtf(2.f);
	MAX_FLOAT = numeric_limits<float>::max();
}

void Game::gameLoop() {
	while (!stopGame) {
		getTurnInput();
		turnBegin();
		makeTurn();
		turnEnd();

#ifdef DEBUG_ONE_TURN
		break;
#endif // DEBUG_ONE_TURN
	}
}

void Game::getTurnInput() {
	int opponentRow;
	int opponentCol;
	cin >> opponentRow >> opponentCol; cin.ignore();

#ifdef REDIRECT_INPUT
	if (STOP_INPUT == opponentRow && STOP_INPUT == opponentCol) {
		exit(0);
	}
#endif // REDIRECT_INPUT

#ifdef OUTPUT_GAME_DATA
	cerr << opponentRow << SPACE << opponentCol << endl;
#endif // OUTPUT_GAME_DATA

	opponentMove.setRowCoord(opponentRow);
	opponentMove.setColCoord(opponentCol);
	monteCarloTreeSearch.setOpponentMove(opponentMove);

	int validActionCount;
	cin >> validActionCount; cin.ignore();

#ifdef OUTPUT_GAME_DATA
	cerr << validActionCount << endl;
#endif // OUTPUT_GAME_DATA

	for (int i = 0; i < validActionCount; i++) {
		int row;
		int col;
		cin >> row >> col; cin.ignore();

#ifdef OUTPUT_GAME_DATA
		cerr << row << SPACE << col << endl;
#endif // OUTPUT_GAME_DATA
	}
}

void Game::turnBegin() {
	//cout << "TURN: " << turnsCount << endl;

	fast_srand(fast_rand());

	short bigBoard[SQUARE_TYPES] = { 0, 0 };
	short bigBoardDraw = 0;
	board.constructBigBoard(bigBoard, bigBoardDraw);

	if (0 == turnsCount && opponentMove.isValid()) {
		board.setPlayer(OPPONENT_PLAYER_IDX);
		board.playMove(opponentMove, bigBoard, bigBoardDraw);
	}
	else if (0 == turnsCount) {
		board.setPlayer(MY_PLAYER_IDX);
		board.playMove({ BOARD_DIM / 2, BOARD_DIM / 2 }, bigBoard, bigBoardDraw);
	}
	else {
		board.playMove(opponentMove, bigBoard, bigBoardDraw);
	}

	//cerr << board << endl;

	if (0 == turnsCount) {
		monteCarloTreeSearch.setTimeLimit(FIRST_TURN_MS - BIAS_MS);
	}
	else {
		monteCarloTreeSearch.setTimeLimit(TURN_MS - BIAS_MS);
	}

	monteCarloTreeSearch.solve(turnsCount);
}

void Game::makeTurn() {
	const Coords bestMove = monteCarloTreeSearch.getBestMove();
	cout << bestMove << endl;

	short bigBoard[SQUARE_TYPES] = { 0, 0 };
	short bigBoardDraw = 0;
	board.constructBigBoard(bigBoard, bigBoardDraw);

	if (opponentMove.isValid()) {
		board.playMove(bestMove, bigBoard, bigBoardDraw);
	}
}

void Game::turnEnd() {
	++turnsCount;
}

void Game::play() {
	initGame();
	gameLoop();
}

int main() {
#ifdef REDIRECT_INPUT
	ifstream in(INPUT_FILE_NAME);
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	ofstream out(OUTPUT_FILE_NAME);
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
#endif // REDIRECT_INPUT

	Game game;
	game.play();

	return 0;
}