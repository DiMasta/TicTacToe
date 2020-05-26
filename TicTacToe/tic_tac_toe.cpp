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

using namespace std;

#define REDIRECT_INPUT
//#define OUTPUT_GAME_DATA
//#define TIME_MEASURERMENT
//#define DEBUG_ONE_TURN
//#define USE_UNIFORM_RANDOM

static const string INPUT_FILE_NAME = "input.txt";
static const string OUTPUT_FILE_NAME = "output.txt";
static const string EMPTY_STRING = "";
static const string SPACE = " ";

static constexpr int INVALID_ID = -1;
static constexpr int INVALID_IDX = -1;
static constexpr int INVALID_NODE_DEPTH = -1;
static constexpr int TREE_ROOT_NODE_DEPTH = 1;
static constexpr int ZERO_CHAR = '0';
static constexpr int DIRECTIONS_COUNT = 8;
static constexpr int BYTE_SIZE = 8;
static constexpr int PAIR = 2;
static constexpr int BASE_2 = 2;
static constexpr int BASE_10 = 10;
static constexpr int BASE_16 = 16;
static constexpr int BOARD_DIM = 9;
static constexpr int MY_PLAYER_IDX = 1;
static constexpr int OPPONENT_PLAYER_IDX = 2;

static constexpr long long FIRST_TURN_MS = 1000;
static constexpr long long TURN_MS = 100;
static constexpr long long BIAS_MS = 2;

static constexpr double MAX_DOUBLE = numeric_limits<double>::max();

const float FLOAT_MAX_RAND = static_cast<float>(RAND_MAX);

enum class Player {
	OPPONENT,
	ME,
};

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
	Coords(Coord xCoord, Coord yCoord);

	Coord getXCoord() const {
		return xCoord;
	}

	Coord getYCoord() const {
		return yCoord;
	}

	void setXCoord(Coord xCoord) { this->xCoord = xCoord; }
	void setYCoord(Coord yCoord) { this->yCoord = yCoord; }

	Coords& operator=(const Coords& other);
	bool operator==(const Coords& other);
	Coords operator+(const Coords& other);
	Coords& operator+=(const Coords& other);
	Coords operator-(const Coords& other);
	Coords& operator-=(const Coords& other);
	Coords operator*(const Coords& other);
	Coords& operator*=(const Coords& other);
	//Coords operator*(Coord numerical);
	//Coords& operator*=(Coord numerical);

	bool isValid() const;
	void debug() const;
	void print() const;

	Coord distance(const Coords& coords) const;
	void roundCoords();

	friend Coord distance(const Coords& point0, const Coords& point1);
private:
	Coord xCoord;
	Coord yCoord;
};

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords() :
	xCoord(INVALID_COORD),
	yCoord(INVALID_COORD)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords::Coords(
	Coord xCoord,
	Coord yCoord
) :
	xCoord(xCoord),
	yCoord(yCoord)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator=(const Coords& other) {
	if (this != &other) {
		xCoord = other.xCoord;
		yCoord = other.yCoord;
	}

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Coords::operator==(const Coords& other) {
	return (xCoord == other.xCoord) && (yCoord == other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator+(const Coords& other) {
	return Coords(xCoord + other.xCoord, yCoord + other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator+=(const Coords& other) {
	xCoord += other.xCoord;
	yCoord += other.yCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator-(const Coords& other) {
	return Coords(xCoord - other.xCoord, yCoord - other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator-=(const Coords& other) {
	xCoord -= other.xCoord;
	yCoord -= other.yCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords Coords::operator*(const Coords& other) {
	return Coords(xCoord * other.xCoord, yCoord * other.yCoord);
}

//*************************************************************************************************************
//*************************************************************************************************************

Coords& Coords::operator*=(const Coords& other) {
	xCoord *= other.xCoord;
	yCoord *= other.yCoord;

	return *this;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Coords::isValid() const {
	return INVALID_COORD != xCoord && INVALID_COORD != yCoord;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Coords::debug() const {
	cerr << "Position: X=" << xCoord << ", Y=" << yCoord << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Coords::print() const {
	cout << yCoord << SPACE << xCoord << endl;
}

//*************************************************************************************************************
//*************************************************************************************************************

Coord Coords::distance(const Coords& coords) const {
	Coord kat0 = coords.xCoord - xCoord;
	Coord kat1 = coords.yCoord - yCoord;

	Coord hip = (Coord)sqrt((kat0 * kat0) + (kat1 * kat1));
	return hip;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Coords::roundCoords() {
	xCoord = static_cast<Coord>(round(xCoord));
	yCoord = static_cast<Coord>(round(yCoord));
}

//*************************************************************************************************************
//*************************************************************************************************************

Coord distance(const Coords& point0, const Coords& point1) {
	Coord lineXLenght = static_cast<Coord>(abs(point0.xCoord - point1.xCoord));
	Coord lineYLenght = static_cast<Coord>(abs(point0.yCoord - point1.yCoord));

	Coord distance = static_cast<Coord>(sqrt((lineXLenght * lineXLenght) + (lineYLenght * lineYLenght)));

	return distance;
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

	/// Appply the given move for the given player
	/// @param[in] move the coordinates on which the player plays
	/// @param[in] the player whose turn it is
	void playMove(const Coords move, const Player player);

private:

};

//*************************************************************************************************************
//*************************************************************************************************************

void Board::playMove(const Coords move, const Player player) {

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class State {
public:
	void setPlayer(const int player) { this->player = player; }
	void setVisits(const int visits) { this->visits = visits; }
	void setWinScore(const double winScore) { this->winScore = winScore; }

	int getPlayer() const { return player; }
	int getVisits() const { return visits; }
	double getWinScore() const { return winScore; }

private:
	Board board; ///< The game board state, which this node represents
	int player; ///< The player index for which is this state
	int visits; ///< How many times this state is visited by the MCTS alogrithm
	double winScore; ///< Score of the state
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Represents a Node in the MCST
class Node {
public:
	const State& getState() const { return state; }
	const vector<int>& getChildren() const { return children; }

	/// Return the count of the children for this node
	int getChildrenCount() const;

private:
	State state; ///< Game state information (wins/visits) including the board
	vector<int> children; ///< List of children nodes' ids in the list of Nodes for the trees
	int parentIdx; ///< The index of the parent node
	int idx; ///< Unique id for the Node, may be removed after the game is solved
};

//*************************************************************************************************************
//*************************************************************************************************************

int Node::getChildrenCount() const {
	return static_cast<int>(children.size());
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Represent the search tree
class Tree {
public:
	/// Return the node with the given index
	const Node& getNode(const int nodeIdx) const;

private:
	vector<Node> nodes; ///< All nodes used in the tree
};

//*************************************************************************************************************
//*************************************************************************************************************

const Node& Tree::getNode(const int nodeIdx) const {
	return nodes[nodeIdx];
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Run MonteCarlo tree search simulation to find the best move for the current state of the board
class MonteCarloTreeSearch {
public:
	MonteCarloTreeSearch(Board& turnOriginalBoard);

	void setOpponentMove(const Coords opponentMove) { this->opponentMove = opponentMove; }
	void setTimeLimit(long long timeLimit) { this->timeLimit = timeLimit; }

	Coords getBestMove() const { return bestMove; }
	long long getTimeLimit() const { return timeLimit; }

	/// Find the best move
	void solve();

private:
	/// Starting form the current turn root, select most promising child node until a leaf node is reached
	/// @return the idx of the selected node
	int selectPromisingNode() const;

	// void expansion();
	// void simulation();
	// void update();

	/// Calculate the upper confidence bound for the given parmeters
	/// @param[in] nodeWinScore
	/// @param[in] totalVisits
	/// @param[in] nodeVisit
	/// @return the UCT score
	double uct(const double nodeWinScore, const int totalVisits, const int nodeVisit) const;

	Tree searchTree; ///< The actual search tree for the algotrithm
	Coords opponentMove; ///< The last move for the opponent
	Coords bestMove; ///< The best move chosen from the simulation
	Board& turnOriginalBoard; ///< Current state of the board
	long long timeLimit; ///< How long to simulate
	double sqrtOf2; ///< Square root of tw, compute only once
	int turnRootNodeIdx; ///< The root node for the current turn, from which the simulations starts
};

//*************************************************************************************************************
//*************************************************************************************************************

MonteCarloTreeSearch::MonteCarloTreeSearch(Board& turnOriginalBoard) :
	turnOriginalBoard{ turnOriginalBoard },
	timeLimit{ 0 }
{
	sqrtOf2 = sqrt(2.0);
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::solve() {
	if (!opponentMove.isValid()) {
		// The board is empty, could play anywhere, play in the middle
		bestMove.setXCoord(BOARD_DIM / 2);
		bestMove.setYCoord(BOARD_DIM / 2);
	}
	else {
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();
		while (chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - begin).count() < timeLimit) {
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int MonteCarloTreeSearch::selectPromisingNode() const {
	int currentNodeIdx = turnRootNodeIdx;

	while (searchTree.getNode(currentNodeIdx).getChildrenCount() > 0) {
		const Node& currentNode = searchTree.getNode(currentNodeIdx);
		const int parentVisits = currentNode.getState().getVisits();
		const vector<int>& nodeChildren = currentNode.getChildren();

		double maxUCT = 0.0;
		for (int childIdx = 0; childIdx < static_cast<int>(nodeChildren.size()); ++childIdx) {
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

double MonteCarloTreeSearch::uct(const double nodeWinScore, const int totalVisits, const int nodeVisit) const {
	double uctValue{ MAX_DOUBLE };

	if (nodeVisit > 0) {
		const double nodeVisitDouble = static_cast<double>(nodeVisit);
		const double totalVisitsDouble = static_cast<double>(totalVisits);
		const double winVisitsRatio = nodeWinScore / nodeVisitDouble;
		const double confidentRatio = sqrtOf2 * sqrt(log(totalVisitsDouble) / nodeVisitDouble); // sqrt and log may slow the program

		uctValue = winVisitsRatio + confidentRatio;
	}

	return uctValue;
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
#endif // TIME_MEASURERM

		getTurnInput();
		turnBegin();
		makeTurn();
		turnEnd();

#ifdef TIME_MEASURERMENT
		chrono::steady_clock::time_point end = chrono::steady_clock::now();
		cerr << "Turn[" << turnsCount - 1 << "] execution time: " << chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " [ms]" << std::endl;
#endif // TIME_MEASURERM

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
	int opponentRow;
	int opponentCol;
	cin >> opponentRow >> opponentCol; cin.ignore();

#ifdef OUTPUT_GAME_DATA
	cerr << opponentRow << SPACE << opponentCol << endl;
#endif // OUTPUT_GAME_DATA

	opponentMove.setXCoord(opponentCol);
	opponentMove.setYCoord(opponentRow);
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
	board.playMove(opponentMove, Player::OPPONENT);

	if (0 == turnsCount) {
		monteCarloTreeSearch.setTimeLimit(FIRST_TURN_MS - BIAS_MS);
	}
	else {
		monteCarloTreeSearch.setTimeLimit(TURN_MS - BIAS_MS);
	}

	monteCarloTreeSearch.solve();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
	monteCarloTreeSearch.getBestMove().print();
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