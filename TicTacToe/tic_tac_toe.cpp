#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <queue>
#include <algorithm>
#include <ctime>
#include <deque>
#include <cmath>
#include <climits>
#include <cstring>
#include <fstream>
#include <iterator>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <cassert>

using namespace std;

//#define REDIRECT_INPUT
//#define OUTPUT_GAME_DATA
//#define TIME_MEASURERMENT
//#define DEBUG_ONE_TURN
//#define USE_UNIFORM_RANDOM

static const string INPUT_FILE_NAME = "input.txt";
static const string OUTPUT_FILE_NAME = "output.txt";
static const string EMPTY_STRING = "";
static constexpr char SPACE = ' ';
static constexpr char TAB = '\t';

static constexpr int INVALID_ID = -1;
static constexpr int INVALID_IDX = -1;
static constexpr int INVALID_NODE_DEPTH = -1;
static constexpr int TREE_ROOT_NODE_DEPTH = 1;
static constexpr int ZERO_CHAR = '0';
static constexpr int DIRECTIONS_COUNT = 8;
static constexpr int BYTE_SIZE = 8;
static constexpr int PAIR = 2;
static constexpr int TRIPLE = 3;
static constexpr int BASE_2 = 2;
static constexpr int BASE_10 = 10;
static constexpr int BASE_16 = 16;
static constexpr int STOP_INPUT = 10;
static constexpr int BOARD_DIM = 9;
static constexpr int ALL_SQUARES = BOARD_DIM * BOARD_DIM;
static constexpr int PLAYER_TOGGLE = 2;
static constexpr int MY_PLAYER_IDX = 0;
static constexpr int OPPONENT_PLAYER_IDX = 1;
static constexpr int MONTE_CARLO_ITERATIONS = 900;

static constexpr char MY_PLAYER_CHAR = 'X';
static constexpr char OPPONENT_PLAYER_CHAR = 'O';
static constexpr char EMPTY_CHAR = '_';

static constexpr size_t NODES_TO_RESERVE = 2'000'000;
static constexpr size_t MAX_CHILDREN_COUNT = 81;

static constexpr long long FIRST_TURN_MS = 1'000;
static constexpr long long TURN_MS = 100;
static constexpr long long BIAS_MS = 2;

static constexpr double WIN_VALUE = 10.0;

static constexpr unsigned PLAYER_FLAG		= 0b0000'0000'0000'0000'0000'0000'0000'0001;
static constexpr unsigned STATUS_MASK		= 0b0000'0000'0000'0000'0000'0000'0000'0110;
static constexpr unsigned MOVE_ROW_MASK		= 0b0000'0000'0000'0000'0000'0011'1111'1000;
static constexpr unsigned MOVE_COL_MASK		= 0b0000'0000'0000'0001'1111'1100'0000'0000;

static constexpr unsigned STATUS_OFFSET		= 1;
static constexpr unsigned MOVE_ROW_OFFSET	= 3;
static constexpr unsigned MOVE_COL_OFFSET	= 10;

static constexpr int X_SQUARE = 0;
static constexpr int O_SQUARE = 1;
static constexpr int SQUARE_TYPES = 2; // 'X'; 'O'
static constexpr short EMPTY_TICTACTOE_BOARD = 0; // 9 empty squares

static constexpr short FULL_BOARD_MASK = 0b0000'000'111'111'111;
static constexpr int WIN_MASKS_COUNT = 8;
static constexpr short WIN_MASKS[WIN_MASKS_COUNT] = {
	0b0000'000'000'000'111, // Top row win
	0b0000'000'000'111'000, // Middle row win
	0b0000'000'111'000'000, // Bottom row win

	0b0000'000'001'001'001, // Left column win
	0b0000'000'010'010'010, // Middle column win
	0b0000'000'100'100'100, // Right column win

	0b0000'000'100'010'001, // Main diagonal win
	0b0000'000'001'010'100, // Second diagonal win
};

enum class BoardStatus {
	INVALID = -1,
	IN_PROGRESS,
	DRAW,
	OPPONENT_WON,
	I_WON,
};

static void printTabs(const int tabsCount, string& str) {
	for (int tabIdx = 0; tabIdx < tabsCount; ++tabIdx) {
		str += TAB;
	}
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

typedef int Coord;
const Coord INVALID_COORD = -1;

/// Y is row
/// X is column
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
	Coords operator+(const Coords& rhs);
	Coords& operator+=(const Coords& rhs);
	Coords operator-(const Coords& rhs);
	Coords& operator-=(const Coords& rhs);
	Coords operator*(const Coords& rhs);
	Coords& operator*=(const Coords& rhs);
	//Coords operator*(Coord numerical);
	//Coords& operator*=(Coord numerical);

	bool isValid() const;

	Coord distance(const Coords& coords) const;
	void roundCoords();

	friend Coord distance(const Coords& point0, const Coords& point1);
	friend ostream& operator<<(ostream& stream, const Coords& coords);
private:
	Coord rowCoord;
	Coord colCoord;
};

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords() :
	rowCoord(INVALID_COORD),
	colCoord(INVALID_COORD)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords(const Coords& rhs) :
	rowCoord(rhs.rowCoord),
	colCoord(rhs.colCoord)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords(
	Coord rowCoord,
	Coord colCoord
) :
	rowCoord(rowCoord),
	colCoord(colCoord)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator=(const Coords& rhs) {
	rowCoord = rhs.rowCoord;
	colCoord = rhs.colCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Coords::operator==(const Coords& rhs) {
	return (rowCoord == rhs.rowCoord) && (colCoord == rhs.colCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator+(const Coords& rhs) {
	return Coords(rowCoord + rhs.rowCoord, colCoord + rhs.colCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator+=(const Coords& rhs) {
	rowCoord += rhs.rowCoord;
	colCoord += rhs.colCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator-(const Coords& rhs) {
	return Coords(rowCoord - rhs.rowCoord, colCoord - rhs.colCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator-=(const Coords& rhs) {
	rowCoord -= rhs.rowCoord;
	colCoord -= rhs.colCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator*(const Coords& rhs) {
	return Coords(rowCoord * rhs.rowCoord, colCoord * rhs.colCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator*=(const Coords& rhs) {
	rowCoord *= rhs.rowCoord;
	colCoord *= rhs.colCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Coords::isValid() const {
	return INVALID_COORD != rowCoord && INVALID_COORD != colCoord;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coord Coords::distance(const Coords& coords) const {
	Coord kat0 = coords.rowCoord - rowCoord;
	Coord kat1 = coords.colCoord - colCoord;

	Coord hip = (Coord)sqrt((kat0 * kat0) + (kat1 * kat1));
	return hip;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Coords::roundCoords() {
	rowCoord = static_cast<Coord>(round(rowCoord));
	colCoord = static_cast<Coord>(round(colCoord));
}

//*************************************************************************************************************
//*************************************************************************************************************

Coord distance(const Coords& point0, const Coords& point1) {
	Coord lineXLenght = static_cast<Coord>(abs(point0.rowCoord - point1.rowCoord));
	Coord lineYLenght = static_cast<Coord>(abs(point0.colCoord - point1.colCoord));

	Coord distance = static_cast<Coord>(sqrt((lineXLenght * lineXLenght) + (lineYLenght * lineYLenght)));

	return distance;
}

//*************************************************************************************************************
//*************************************************************************************************************

ostream& operator<<(ostream& stream, const Coords& coords) {
	stream << coords.rowCoord << SPACE << coords.colCoord;
	return stream;
}

//*************************************************************************************************************
//*************************************************************************************************************

enum Direction {
	DIR_INVALID = -1,
	DIR_N = 0,
	DIR_NE,
	DIR_E,
	DIR_SE,
	DIR_S,
	DIR_SW,
	DIR_W,
	DIR_NW,
};

//*************************************************************************************************************
//*************************************************************************************************************

Coords DIRECTIONS[DIRECTIONS_COUNT] = {
	Coords(0, -1), // N
	Coords(1, -1), // NE
	Coords(1,  0), // E
	Coords(1,  1), // SE
	Coords(0,  1), // S
	Coords(-1,  1), // SW
	Coords(-1,  0), // W
	Coords(-1, -1)  // NW
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Represents the board with the played turns
class Board {
public:
	Board();
	Board(const Board& rhs);

	//void setPlayer(const int player) { this->player = player; }
	//void setMove(Coords move) { this->move = move; }
	//
	//BoardStatus getStatus() const { return status; }
	//int getPlayer() const { return player; }
	//Coords getMove() const { return move; }

	void setStatus(const BoardStatus status);
	void setPlayer(const int player);
	void setMove(const Coords move);

	BoardStatus getStatus() const;
	int getPlayer() const;
	Coords getMove() const;

	/// Initialize the board empty squares
	void init();

	/// Copy rhs board into this board
	void copy(const Board& rhs);

	/// Determine the mini board index by the given global position
	/// @param[in] pos the position on the big board
	/// @return the index of the mini board in the big board
	int getMiniBoardIdx(const Coords pos) const;

	/// Determine the mini board inner index by the given global position
	/// @param[in] pos the position on the big board
	/// @return the inner index of the mini board in the big board
	short getMiniBoardInnerIdx(const Coords pos) const;

	/// Determine the gloabal board postion by given the mini board index and inner mini board index
	/// @param[in] miniBoardIdx the index ofthe miniboard
	/// @param[in] miniBoardInnerIdx the index of the element in the mini board
	/// @return the position in the global big board
	Coords getBigBoardPosition(const int miniBoardIdx, const int miniBoardInnerIdx) const;

	/// Return the player index, played in the given position
	int getPlayerIdx(const Coords pos) const;

	/// Set the given player index in the given position
	void setPlayerIdx(const Coords pos, const int playerIdx);

	/// Check if the given move could be performed
	/// @param[in] move the coordinates on which the player plays
	/// @param[in] previousMove the coordinates on which the last player played
	/// return true if the given move could be performed
	bool validMove(const Coords move, const Coords previousMove);

	/// Appply the given move for the given player and chage the player
	/// @param[in] move the coordinates on which the player plays
	bool playMove(const Coords move);

	/// Return list of all playable coordinates
	vector<Coords> getAllPossibleMoves() const;

	/// Toggle player
	int togglePlayer(const int playerToToggle) const;

	/// Play game with random moves until end of the game is reached
	/// @return the result of the game
	int simulateRandomGame();

	Board& operator=(const Board& board);
	friend ostream& operator<<(std::ostream& stream, const Board& board);

	/// Debug the Board online
	void debug() const;

private:
	/// Return possible moves for the given mini board
	vector<Coords> getAllPossibleMovesForMiniBoard(const int miniBoardIdx) const;

	/// Return possible moves in all mini boards
	vector<Coords> getAllPossibleMovesForAllMiniBoards() const;

	/// Return true if the player wins on the given board
	bool checkForWin(const short boardToCheck) const;

	/// Return true if the given board is full
	bool boardFull(const short boardToCheck) const;

	/// Return true if the given mini board is playable
	bool playableMiniBoard(const int miniBoardIdx) const;

	/// Detemine which player wins when the board is full
	BoardStatus resolveDraw() const;

	/// Return true if the mini board could be used for play
	bool miniBoardPlayable(const int miniBoardIdx) const;

	short board[SQUARE_TYPES][BOARD_DIM]; /// Board for each player, each short representa a tictactoe board
	short bigBoard[SQUARE_TYPES]; /// Big Board for each player, each short representa a tictactoe board
	//int player; ///< The player index for which is this state
	//Coords move; ///< Move which led to this board
	//BoardStatus status; ///< Status of the board
	unsigned flags; ///< Flags and masks for the board
	short bigBoardDraw; ///< Flags indicating which mini boards ended in draw
};

//*************************************************************************************************************
//*************************************************************************************************************

Board::Board() {
	init();
}

//*************************************************************************************************************
//*************************************************************************************************************

Board::Board(const Board& rhs) {
	copy(rhs);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::setStatus(const BoardStatus status) {
	unsigned statusToSet = static_cast<unsigned>(status);
	statusToSet <<= STATUS_OFFSET;

	flags &= ~STATUS_MASK; // First zero out the current status
	flags |= statusToSet;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::setPlayer(const int player) {
	flags &= ~PLAYER_FLAG; // First zero out the player flag
	flags |= player;
}

//*************************************************************************************************************
//*************************************************************************************************************

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

//*************************************************************************************************************
//*************************************************************************************************************

BoardStatus Board::getStatus() const {
	return static_cast<BoardStatus>((STATUS_MASK & flags) >> STATUS_OFFSET);
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::getPlayer() const {
	return static_cast<int>(PLAYER_FLAG & flags);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Board::getMove() const {
	Coords res;

	res.setRowCoord((flags & MOVE_ROW_MASK) >> MOVE_ROW_OFFSET);
	res.setColCoord((flags & MOVE_COL_MASK) >> MOVE_COL_OFFSET);

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::init() {
	for (int sqType = 0; sqType < SQUARE_TYPES; ++sqType) {
		for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
			board[sqType][miniBoardIdx] = EMPTY_TICTACTOE_BOARD;
		}
	}

	for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
		bigBoard[miniBoardIdx] = EMPTY_TICTACTOE_BOARD;
	}

	//player = INVALID_IDX;
	//status = BoardStatus::IN_PROGRESS;
	flags = 0;
	setStatus(BoardStatus::IN_PROGRESS);
	bigBoardDraw = EMPTY_TICTACTOE_BOARD;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::copy(const Board& rhs) {
	//this->player = rhs.player;
	//this->move = rhs.move;
	//this->status = rhs.status;
	this->flags = rhs.flags;
	this->bigBoardDraw = rhs.bigBoardDraw;

	for (int sqTypeIdx = 0; sqTypeIdx < SQUARE_TYPES; ++sqTypeIdx) {
		for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
			this->board[sqTypeIdx][miniBoardIdx] = rhs.board[sqTypeIdx][miniBoardIdx];
		}

		this->bigBoard[sqTypeIdx] = rhs.bigBoard[sqTypeIdx];
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::getMiniBoardIdx(const Coords pos) const {
	const int bigBoardRowIdx = pos.getRowCoord() / TRIPLE;
	const int bigBoardColIdx = pos.getColCoord() / TRIPLE;
	const int miniBoardIdx = (bigBoardRowIdx * TRIPLE) + bigBoardColIdx;

	return miniBoardIdx;
}

//*************************************************************************************************************
//*************************************************************************************************************

short Board::getMiniBoardInnerIdx(const Coords pos) const {
	const int miniBoardRowIdx = pos.getRowCoord() % TRIPLE;
	const int miniBoardColIdx = pos.getColCoord() % TRIPLE;
	const short miniBoardInnerIdx = (miniBoardRowIdx * TRIPLE) + miniBoardColIdx;

	return miniBoardInnerIdx;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Board::getBigBoardPosition(const int miniBoardIdx, const int miniBoardInnerIdx) const {
	const int miniBoardRowIdx = miniBoardIdx / TRIPLE;
	const int miniBoardColIdx = miniBoardIdx % TRIPLE;

	const int miniBoardInnerRowIdx = miniBoardInnerIdx / TRIPLE;
	const int miniBoardInnerColIdx = miniBoardInnerIdx % TRIPLE;

	const int gloabalRowIdx = miniBoardInnerRowIdx + (miniBoardRowIdx * TRIPLE);
	const int gloabalColIdx = miniBoardInnerColIdx + (miniBoardColIdx * TRIPLE);

	return { gloabalRowIdx, gloabalColIdx };
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::getPlayerIdx(const Coords pos) const {
	int playerIdx = INVALID_IDX;

	const int miniBoardIdx = getMiniBoardIdx(pos);
	const short miniBoardInnerIdx = getMiniBoardInnerIdx(pos);

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

//*************************************************************************************************************
//*************************************************************************************************************

void Board::setPlayerIdx(const Coords pos, const int playerIdx) {
	const int miniBoardIdx = getMiniBoardIdx(pos);
	const short miniBoardInnerIdx = getMiniBoardInnerIdx(pos);

	board[playerIdx][miniBoardIdx] |= 1 << miniBoardInnerIdx;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Board::validMove(const Coords move, const Coords previousMove) {
	bool valid = true;

	const short miniBoardIdx = static_cast<short>(getMiniBoardIdx(move));
	const short miniBoardIdxFromPrevMove = getMiniBoardInnerIdx(previousMove);

	if (!miniBoardPlayable(miniBoardIdxFromPrevMove) || miniBoardIdx == miniBoardIdxFromPrevMove) {
		if (miniBoardPlayable(miniBoardIdx)) {
			const short miniBoardInnerIdx = getMiniBoardInnerIdx(move);

			const bool iPlayedOnSquare = board[MY_PLAYER_IDX][miniBoardIdx] & (1 << miniBoardInnerIdx);
			const bool opponentPlayedOnSquare = board[OPPONENT_PLAYER_IDX][miniBoardIdx] & (1 << miniBoardInnerIdx);
			valid = !iPlayedOnSquare && !opponentPlayedOnSquare;
		}
		else {
			valid = false;
		}
	}
	else {
		valid = false;
	}

	return valid;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Board::playMove(const Coords move) {
	bool movePlayed = false;
	const int player = getPlayer();

	//if (validMove(move)) {
		setMove(move);
		setPlayerIdx(move, player);

		const int miniBoardIdx = getMiniBoardIdx(move);
		const short miniBoard = board[player][miniBoardIdx];
		if (checkForWin(miniBoard)) {
			bigBoard[player] |= (1 << miniBoardIdx);
		}
		else if (boardFull(board[MY_PLAYER_IDX][miniBoardIdx] | board[OPPONENT_PLAYER_IDX][miniBoardIdx])) {
			bigBoardDraw |= (1 << miniBoardIdx);
		}

		if (checkForWin(bigBoard[player])) {
			setStatus((MY_PLAYER_IDX == player) ? BoardStatus::I_WON : BoardStatus::OPPONENT_WON);
		}
		else if (boardFull(bigBoard[MY_PLAYER_IDX] | bigBoard[OPPONENT_PLAYER_IDX] | bigBoardDraw)) {
			setStatus(resolveDraw());
		}

		setPlayer(togglePlayer(player));
		movePlayed = true;
	//}

	return movePlayed;
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<Coords> Board::getAllPossibleMoves() const {
	const int activeMiniBoardIdx = getMiniBoardInnerIdx(getMove()); // Current moves detemine the next mini board
	vector<Coords> miniBoardEmptyPositions = getAllPossibleMovesForMiniBoard(activeMiniBoardIdx);

	if (0 == miniBoardEmptyPositions.size()) {
		miniBoardEmptyPositions = getAllPossibleMovesForAllMiniBoards();
	}

	return miniBoardEmptyPositions;
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::simulateRandomGame() {
	//Coords previousMove = move;
	//for (int moveIdx = 0; moveIdx < ALL_SQUARES; ++moveIdx) {
	//	Coords randomMove{ moveIdx / BOARD_DIM, moveIdx % BOARD_DIM };
	//
	//	if (BoardStatus::IN_PROGRESS != status) {
	//		break;
	//	}
	//	else if (!validMove(randomMove, previousMove)) {
	//		continue;
	//	}
	//
	//	playMove(randomMove);
	//	previousMove = randomMove;
	//	moveIdx = 0;
	//}

	while (BoardStatus::IN_PROGRESS == getStatus()) {
		const vector<Coords>& allMoves = getAllPossibleMoves();
		Coords randomMove = allMoves[rand() % allMoves.size()];
	
		playMove(randomMove);
	}

	return BoardStatus::I_WON == getStatus() ? MY_PLAYER_IDX : OPPONENT_PLAYER_IDX;
}

//*************************************************************************************************************
//*************************************************************************************************************

Board& Board::operator=(const Board& rhs) {
	copy(rhs);
	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::debug() const {
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::togglePlayer(const int playerToToggle) const {
	return PLAYER_TOGGLE - (playerToToggle + 1);
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<Coords> Board::getAllPossibleMovesForMiniBoard(const int miniBoardIdx) const {
	vector<Coords> moves;
	moves.reserve(BOARD_DIM);

	if (playableMiniBoard(miniBoardIdx)) {
		const short opponentBoard = board[OPPONENT_PLAYER_IDX][miniBoardIdx];
		const short myBoard = board[MY_PLAYER_IDX][miniBoardIdx];

		for (int sqIdx = 0; sqIdx < BOARD_DIM; ++sqIdx) {
			const short squareMask = 1 << sqIdx;
			if (!(opponentBoard & squareMask) && !(myBoard & squareMask)) {
				Coords squarePosition = getBigBoardPosition(miniBoardIdx, sqIdx);
				moves.push_back(squarePosition);
			}
		}
	}

	return moves;
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<Coords> Board::getAllPossibleMovesForAllMiniBoards() const {
	vector<Coords> allMoves;
	allMoves.reserve(BOARD_DIM * BOARD_DIM);
	
	for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
		vector<Coords> miniBoardMoves = getAllPossibleMovesForMiniBoard(miniBoardIdx);
		allMoves.insert(allMoves.end(), miniBoardMoves.begin(), miniBoardMoves.end());
	}

	return allMoves;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Board::checkForWin(const short boardToCheck) const {
	bool playerWon = false;

	for (int winMaskIdx = 0; winMaskIdx < WIN_MASKS_COUNT; ++winMaskIdx) {
		if (WIN_MASKS[winMaskIdx] == (WIN_MASKS[winMaskIdx] & boardToCheck)) {
			playerWon = true;
			break;
		}
	}

	return playerWon;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Board::boardFull(const short boardToCheck) const {
	return FULL_BOARD_MASK == (FULL_BOARD_MASK & boardToCheck);
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Board::playableMiniBoard(const int miniBoardIdx) const {
	const short miniBoardMask = 1 << miniBoardIdx;
	const bool boardWon = (miniBoardMask & bigBoard[MY_PLAYER_IDX]) || (miniBoardMask & bigBoard[OPPONENT_PLAYER_IDX]);
	const bool boardDraw = miniBoardMask & bigBoardDraw;

	return !boardWon && !boardDraw;
}

//*************************************************************************************************************
//*************************************************************************************************************

BoardStatus Board::resolveDraw() const {
	int opponentMiniBoardsWon = 0;
	int mineMiniBoardsWon = 0;

	for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
		if (bigBoard[OPPONENT_PLAYER_IDX] & (1 << miniBoardIdx)) {
			++opponentMiniBoardsWon;
		}
		else {
			++mineMiniBoardsWon;
		}
	}

	return (opponentMiniBoardsWon > mineMiniBoardsWon) ? BoardStatus::OPPONENT_WON : BoardStatus::I_WON;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Board::miniBoardPlayable(const int miniBoardIdx) const {
	const bool iWonMiniBoard = bigBoard[MY_PLAYER_IDX] & (1 << miniBoardIdx);
	const bool opponentWonMiniBoard = bigBoard[OPPONENT_PLAYER_IDX] & (1 << miniBoardIdx);
	const bool drawnMiniBoard = bigBoardDraw & (1 << miniBoardIdx);

	return !iWonMiniBoard && !opponentWonMiniBoard && !drawnMiniBoard;
}

//*************************************************************************************************************
//*************************************************************************************************************

ostream& operator<<(std::ostream& stream, const Board& board) {
	for (int rowIdx = 0; rowIdx < BOARD_DIM + 1; ++rowIdx) {
		if (rowIdx > 0 && 0 == rowIdx % TRIPLE) {
			stream << endl;
		}

		for (int colIdx = 0; colIdx < BOARD_DIM + 1; ++colIdx) {
			if (0 == colIdx % TRIPLE) {
				stream << SPACE << SPACE << SPACE;
			}

			if (BOARD_DIM == rowIdx) {
				stream << colIdx;
			}
			else if (BOARD_DIM == colIdx) {
				stream << rowIdx;
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

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class State {
public:
	State(const Board& board, const int visits, const double winScore);

	void setBoard(const Board& board) { this->board = board; }
	void setVisits(const int visits) { this->visits = visits; }
	void setWinScore(const double winScore) { this->winScore = winScore; }

	const Board& getBoard() const { return board; }
	Board& getBoard() { return board; }
	int getVisits() const { return visits; }
	double getWinScore() const { return winScore; }

	/// Debug the State online
	void debug() const;

private:
	Board board; ///< The game board state, which this node represents
	int visits; ///< How many times this state is visited by the MCTS alogrithm
	double winScore; ///< Score of the state
};

//*************************************************************************************************************
//*************************************************************************************************************

State::State(const Board& board, const int visits, const double winScore) :
	board{ board },
	visits{ visits },
	winScore{ winScore }
{
}

//*************************************************************************************************************
//*************************************************************************************************************

void State::debug() const {
	//board.debug();
	//cerr << "visits: " << visits << endl;
	cerr << "winScore: " << winScore << endl;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Represents a Node in the MCST
class Node {
public:
	Node(const State& state, const int parentIdx);

	const State& getState() const { return state; }
	State& getState() { return state; }
	const vector<int>& getChildren() const { return children; }
	int getParentIdx() const { return parentIdx; }

	/// Add child node wiht the given index
	void addChild(const int childIdxNode);

	/// Return the count of the children for this node
	int getChildrenCount() const;

	/// Debug the Node online
	void debug() const;

private:
	State state; ///< Game state information (wins/visits) including the board
	vector<int> children; ///< List of children nodes' ids in the list of Nodes for the trees
	int parentIdx; ///< The index of the parent node
};

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node(const State& state, const int parentIdx) :
	state{ state },
	parentIdx{ parentIdx }
{
	children.reserve(MAX_CHILDREN_COUNT);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Node::addChild(const int childIdxNode) {
	children.push_back(childIdxNode);
}

//*************************************************************************************************************
//*************************************************************************************************************

int Node::getChildrenCount() const {
	return static_cast<int>(children.size());
}

//*************************************************************************************************************
//*************************************************************************************************************

void Node::debug() const {
	state.debug();
	
	//cerr << "Children: ";
	//for (int childIdx : children) {
	//	cerr << childIdx << SPACE;
	//}
	cerr << endl;
	cerr << endl;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Represent the search tree
class Tree {
public:
	int getNodesCount() const { return static_cast<int>(nodes.size()); }

	/// Initialize the tree with single root node
	/// @param[in] initialBoard the starting board
	void init(const Board& initialBoard);

	/// Return the node with the given index
	const Node& getNode(const int nodeIdx) const { return nodes[nodeIdx]; }
	Node& getNode(const int nodeIdx) { return nodes[nodeIdx]; }

	/// Set the starting player for the game
	void setRootPlayer(const int playerIdx);

	/// Add the given node to the tree and return its index
	int addNode(const Node& node);

	/// Print the tree to file for easy debug
	void print() const;

	/// Debug the tree online
	void debug() const;

private:
	/// Use DFS to print the tree to file
	/// @param[in] depth the current depth of the tree
	/// @param[in] nodeToExplore the current Node to explore
	/// @param[in] lastChild true if the last child must be printed
	/// @param[out] treeString the string which is updated at each depth
	void dfsPrint(const int depth, const int nodeToExplore, const bool lastChild, string& treeString) const;

	vector<Node> nodes; ///< All nodes used in the tree
};

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::init(const Board& initialBoard) {
	nodes.reserve(NODES_TO_RESERVE);

	State rootState{ initialBoard, 0, 0 };
	Node rootNode{ rootState, INVALID_IDX };
	nodes.push_back(rootNode);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::setRootPlayer(const int playerIdx) {
	nodes[0].getState().getBoard().setPlayer(playerIdx);
}

//*************************************************************************************************************
//*************************************************************************************************************

int Tree::addNode(const Node& node) {
	assert(nodes.capacity() <= NODES_TO_RESERVE);

	nodes.push_back(node);
	return static_cast<int>(nodes.size() - 1);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::print() const {
	string treeString = EMPTY_STRING;
	dfsPrint(0, 0, false, treeString);

	//cerr << treeString << endl;
	int debug = 0;
	++debug;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::debug() const {
	for (size_t nodeIdx = 0; nodeIdx < nodes.size(); ++nodeIdx) {
		cerr << "Node[" << nodeIdx << "]: " << endl;
		nodes[nodeIdx].debug();
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::dfsPrint(const int depth, const int nodeToExploreIdx, const bool lastChild, string& treeString) const {
	const Node& nodeToExplore = nodes[nodeToExploreIdx];

	printTabs(depth, treeString);
	treeString += "{\n";
	printTabs(depth + 1, treeString);
	treeString += R"("name": ")";
	treeString += to_string(nodeToExploreIdx);
	treeString += "[";
	treeString += to_string(nodeToExplore.getState().getBoard().getMove().getRowCoord());
	treeString += "; ";
	treeString += to_string(nodeToExplore.getState().getBoard().getMove().getColCoord());
	treeString += "]";
	treeString += to_string(static_cast<int>(nodeToExplore.getState().getWinScore()));
	treeString += R"(")";

	const int childrenCount = nodeToExplore.getChildrenCount();
	if (childrenCount > 0) {
		treeString += ",\n";
		printTabs(depth + 1, treeString);
		treeString += R"("children": [)";
		treeString += "\n";
		printTabs(depth + 1, treeString);

		const vector<int> children = nodeToExplore.getChildren();
		for (int i = 0; i < childrenCount; ++i) {
			dfsPrint(depth + 1, children[i], i == (childrenCount - 1), treeString);
		}

		printTabs(depth + 1, treeString);
		treeString += "]\n";
	}
	else {
		treeString += "\n";
	}

	printTabs(depth, treeString);
	treeString += "}";
	if (!lastChild && !(0 == depth)) {
		treeString += ",";
	}
	treeString += "\n";
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Run MonteCarlo tree search simulation to find the best move for the current state of the board
class MonteCarloTreeSearch {
public:
	MonteCarloTreeSearch(Board& initialBoard);

	void setOpponentMove(const Coords opponentMove) { this->opponentMove = opponentMove; }
	void setTimeLimit(long long timeLimit) { this->timeLimit = timeLimit; }

	Coords getBestMove() const { return bestMove; }
	long long getTimeLimit() const { return timeLimit; }
	int getNodesCount() const { return searchTree.getNodesCount(); }

	/// Find the best move
	/// @param[in] turnIdx the turn index
	void solve(const int turnIdx);

	/// Set the starting player for the game
	void setRootPlayer(const int playerIdx);

	/// Print the search tree to file for easy visual debug
	void printSearchTree() const;

	/// Debug the tree online
	void debug() const;

private:
	/// Starting form the current turn root, select most promising child node until a leaf node is reached
	/// @return the idx of the selected node
	int selectPromisingNode() const;

	/// Expand the search tree with adding all possible child nodes to the selected node
	/// @param[in] selectedNode the node to expand
	void expansion(const int selectedNode);

	/// Simulate game with random moves until the end
	/// @pram[in] nodeToExploreIdx the node for which to simulate the game
	/// @return the idx of the player who wins, -1 if draw
	int simulation(const int nodeToExploreIdx);

	/// Back propagate the simulation result through the parents until the root
	/// @param[in] nodeToExploreIdx the explored node
	/// @param[in] simulationResult the result form the simulation step
	void backPropagation(const int nodeToExploreIdx, const int simulationResult);

	/// Calculate the upper confidence bound for the given parmeters
	/// @param[in] nodeWinScore the score so far for the node
	/// @param[in] parentVisits the number of visits for the parent
	/// @param[in] nodeVisit the number of visits for the node
	/// @return the UCT score
	double uct(const double nodeWinScore, const int parentVisits, const int nodeVisit) const;

	/// Start the search
	/// @param[in] turnIdx the turn index
	void searchBegin(const int turnIdx);

	/// Conclude the search choosing the best move and updating the root
	/// @param[in] turnIdx the turn index
	void searchEnd(const int turnIdx);

	Tree searchTree; ///< The actual search tree for the algotrithm
	Coords opponentMove; ///< The last move for the opponent
	Coords bestMove; ///< The best move chosen from the simulation
	Board& initialBoard; ///< Current state of the board
	long long timeLimit; ///< How long to simulate
	double sqrtOf2; ///< Square root of tw, compute only once
	double maxDouble; ///< Infinity
	int turnRootNodeIdx; ///< The root node for the current turn, from which the simulations starts
};

//*************************************************************************************************************
//*************************************************************************************************************

MonteCarloTreeSearch::MonteCarloTreeSearch(Board& initialBoard) :
	initialBoard{ initialBoard },
	timeLimit{ 0 },
	turnRootNodeIdx{ 0 }
{
	sqrtOf2 = sqrt(2.0);
	maxDouble = numeric_limits<double>::max();
	searchTree.init(initialBoard);
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::solve(const int turnIdx) {
	searchBegin(turnIdx);

	int iteration = 0;
	//while (iteration < MONTE_CARLO_ITERATIONS) {

	chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while (chrono::steady_clock::now() - start < chrono::milliseconds{timeLimit}) {
		const int selectedNodeIdx = selectPromisingNode();
		const Node& selectedNode = searchTree.getNode(selectedNodeIdx);
		const State& selectedState = selectedNode.getState();
		const Board& selectedBoard = selectedState.getBoard();
		const BoardStatus selectedBoardStatus = selectedBoard.getStatus();

		if (BoardStatus::IN_PROGRESS == selectedBoard.getStatus()) {
			expansion(selectedNodeIdx);
		}
		
		int nodeToExploreIdx = selectedNodeIdx;
		if (selectedNode.getChildrenCount() > 0) {
			nodeToExploreIdx = selectedNode.getChildren()[0];
		}

		int victoriousPlayer = simulation(nodeToExploreIdx);
		backPropagation(nodeToExploreIdx, victoriousPlayer);

		++iteration;
	}

	cerr << "MCTS iterations: " << iteration << endl;
	cerr << "Nodes count: " << searchTree.getNodesCount() << endl;

	searchEnd(turnIdx);
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::setRootPlayer(const int playerIdx) {
	searchTree.setRootPlayer(playerIdx);
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::printSearchTree() const {
	searchTree.print();
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::debug() const {

}

//*************************************************************************************************************
//*************************************************************************************************************

int MonteCarloTreeSearch::selectPromisingNode() const {
	//cerr << "selectPromisingNode" << endl;
	int currentNodeIdx = turnRootNodeIdx;

	while (searchTree.getNode(currentNodeIdx).getChildrenCount() > 0) {
		const Node& currentNode = searchTree.getNode(currentNodeIdx);
		const int parentVisits = currentNode.getState().getVisits();
		const vector<int>& nodeChildren = currentNode.getChildren();

		double maxUCT = -1.0;
		for (int childIdx = 0; childIdx < currentNode.getChildrenCount(); ++childIdx) {
			const int childNodeIdx = nodeChildren[childIdx];
			const Node& childNode = searchTree.getNode(childNodeIdx);
			const State& childState = childNode.getState();
			const double childUCT = uct(childState.getWinScore(), parentVisits, childState.getVisits());

			if (childUCT > maxUCT) {
				maxUCT = childUCT;
				currentNodeIdx = childNodeIdx;
			}
		}
	}

	return currentNodeIdx;
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::expansion(const int selectedNode) {
	Node& parentNode = searchTree.getNode(selectedNode);
	const State& parentState = parentNode.getState();
	const Board& parentBoard = parentState.getBoard();
	vector<Coords> allMoves = parentBoard.getAllPossibleMoves();
	const int allMovesCount = static_cast<int>(allMoves.size());

	for (int moveIdx = 0; moveIdx < allMovesCount; ++moveIdx) {
		Board childBoard{ parentBoard };
		childBoard.playMove(allMoves[moveIdx]);

		State childState{ childBoard, 0, 0.0 };
		Node childNode{ childState, selectedNode };

		const int childNodeIdx = searchTree.addNode(childNode);
		parentNode.addChild(childNodeIdx);
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int MonteCarloTreeSearch::simulation(const int nodeToExploreIdx) {
	//cerr << "simulation" << endl;
	// Copy board, so no need to reset afterwards
	Board boardToSimulate = searchTree.getNode(nodeToExploreIdx).getState().getBoard();

	return boardToSimulate.simulateRandomGame();
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::backPropagation(const int nodeToExploreIdx, const int victoriousPlayer) {
	//cerr << "backPropagation" << endl;
	int currentNodeIdx = nodeToExploreIdx;
	while (INVALID_IDX != currentNodeIdx) {
		Node& currentNode = searchTree.getNode(currentNodeIdx);
		State& currentNodeState = currentNode.getState();
		currentNodeState.setVisits(currentNodeState.getVisits() + 1);

		int ownerPlayer = currentNodeState.getBoard().getPlayer();
		ownerPlayer = currentNodeState.getBoard().togglePlayer(ownerPlayer);
		if (ownerPlayer == victoriousPlayer) {
			currentNodeState.setWinScore(currentNodeState.getWinScore() + WIN_VALUE);
		}

		currentNodeIdx = currentNode.getParentIdx();
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

double MonteCarloTreeSearch::uct(const double nodeWinScore, const int parentVisits, const int nodeVisit) const {
	double uctValue{ maxDouble };

	if (nodeVisit > 0) {
		const double nodeVisitDouble = static_cast<double>(nodeVisit);
		const double totalVisitsDouble = static_cast<double>(parentVisits);
		const double winVisitsRatio = nodeWinScore / nodeVisitDouble;
		const double confidentRatio = sqrtOf2 * sqrt(log(totalVisitsDouble) / nodeVisitDouble); // sqrt and log may slow the program

		uctValue = winVisitsRatio + confidentRatio;
	}

	return uctValue;
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::searchBegin(const int turnIdx) {
	if (0 == turnIdx) {
		searchTree.getNode(turnRootNodeIdx).getState().setBoard(initialBoard);
	}
	else {
		const Node& currentRoot = searchTree.getNode(turnRootNodeIdx);
		const vector<int> currentRootChildren = currentRoot.getChildren();
		for (const int childIdx : currentRootChildren) {
			const Node& child = searchTree.getNode(childIdx);
			const Coords childMove = child.getState().getBoard().getMove();

			if (opponentMove == child.getState().getBoard().getMove()) {
				turnRootNodeIdx = childIdx;
			}
		}
	}

	cerr << "turnRootNodeIdx: " << turnRootNodeIdx << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::searchEnd(const int turnIdx) {
	if (0 == turnIdx && !opponentMove.isValid()) {
		// If I'm fisrt the tree is build for a play in the middle
		bestMove = { BOARD_DIM / 2, BOARD_DIM / 2 };
	}
	else {
		const vector<int>& rootChildren = searchTree.getNode(turnRootNodeIdx).getChildren();

		if (rootChildren.size() > 0) {
			int bestChildIdx = rootChildren[0]; // Score for all children may be 0.0
			double maxScore = 0.0;
			for (int childIdx = 0; childIdx < static_cast<int>(rootChildren.size()); ++childIdx) {
				const int childNodeIdx = rootChildren[childIdx];
				const double childScore = searchTree.getNode(childNodeIdx).getState().getWinScore();
				if (childScore > maxScore) {
					maxScore = childScore;
					bestChildIdx = childNodeIdx;
				}
			}

			cerr << "maxScore: " << maxScore << endl;

			const Board& bestMoveBoard = searchTree.getNode(bestChildIdx).getState().getBoard();
			bestMove = bestMoveBoard.getMove();
			turnRootNodeIdx = bestChildIdx;

			if (0 == searchTree.getNode(turnRootNodeIdx).getChildrenCount()) {
				expansion(turnRootNodeIdx);
			}
		}
	}

	//printSearchTree();
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Game {
public:
	Game();
	~Game();

	void initGame();
	void gameBegin();
	void gameEnd();
	void gameLoop();
	void getGameInput();
	void getTurnInput();
	void turnBegin();
	void makeTurn();
	void turnEnd();
	void play();

	void debug() const;

private:
	// Game specific members
	Board board; ///< The board on which the game is played
	MonteCarloTreeSearch monteCarloTreeSearch; ///< The AI algorithm
	Coords opponentMove; ///< The coordinates on which the opponent plays

	int turnsCount;
	int stopGame;
};

//*************************************************************************************************************
//*************************************************************************************************************

Game::Game() :
	board{},
	monteCarloTreeSearch{ board },
	opponentMove{},
	turnsCount{ 0 },
	stopGame{ false }
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Game::~Game() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::initGame() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameEnd() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::gameLoop() {
	while (!stopGame) {
#ifdef TIME_MEASURERMENT
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();
#endif // TIME_MEASURERMENT

		getTurnInput();
		turnBegin();
		makeTurn();
		turnEnd();

#ifdef TIME_MEASURERMENT
		chrono::steady_clock::time_point end = chrono::steady_clock::now();
		cerr << "Turn[" << turnsCount - 1 << "] execution time: " << chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " [ms]" << std::endl;
#endif // TIME_MEASURERMENT

#ifdef DEBUG_ONE_TURN
		break;
#endif // DEBUG_ONE_TURN
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::getGameInput() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::getTurnInput() {
	//cerr << "Turn: " << turnsCount << endl;

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

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnBegin() {
	if (0 == turnsCount && opponentMove.isValid()) {
		board.setPlayer(OPPONENT_PLAYER_IDX);
		board.playMove(opponentMove);
	}
	else if (0 == turnsCount) {
		board.setPlayer(MY_PLAYER_IDX);
		board.playMove({ BOARD_DIM / 2, BOARD_DIM / 2 }); // Play in the middle if I'm first
	}
	else {
		board.playMove(opponentMove);
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

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
	const Coords bestMove = monteCarloTreeSearch.getBestMove();
	cout << bestMove << endl;

	if (opponentMove.isValid()) {
		board.playMove(bestMove);
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnEnd() {
	++turnsCount;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::play() {
	initGame();
	getGameInput();
	gameBegin();
	gameLoop();
	gameEnd();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::debug() const {
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

#ifdef TESTS
#include "debug.h"
#endif // TESTS

int main(int argc, char** argv) {
#ifdef TESTS
	doctest::Context context;
	int res = context.run();
#else

#ifdef REDIRECT_INPUT
	cerr << endl << endl << endl << "!!! REDIRECT_INPUT !!!" << endl << endl << endl;
	ifstream in(INPUT_FILE_NAME);
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	ofstream out(OUTPUT_FILE_NAME);
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
#endif // REDIRECT_INPUT

	Game game;
	game.play();

#endif // TESTS

	return 0;
}

// seed=761658940
