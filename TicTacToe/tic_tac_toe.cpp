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
#define DEBUG_ONE_TURN
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
static constexpr int TRIPLE = 3;
static constexpr int BASE_2 = 2;
static constexpr int BASE_10 = 10;
static constexpr int BASE_16 = 16;
static constexpr int BOARD_DIM = 9;
static constexpr int PLAYER_TOGGLE = 3;
static constexpr int MY_PLAYER_IDX = 0;
static constexpr int OPPONENT_PLAYER_IDX = 1;

static constexpr char MY_PLAYER_CHAR = 'X';
static constexpr char OPPONENT_PLAYER_CHAR = 'O';
static constexpr char EMPTY_CHAR = '_';

static constexpr size_t NODES_TO_RESERVE = 2'000;
static constexpr size_t MAX_CHILDREN_COUNT = 81;

static constexpr long long FIRST_TURN_MS = 1'000;
static constexpr long long TURN_MS = 100;
static constexpr long long BIAS_MS = 2;

static constexpr double MAX_DOUBLE = numeric_limits<double>::max();
static constexpr double WIN_VALUE = 10.0;

static constexpr int X_SQUARE = 0;
static constexpr int O_SQUARE = 1;
static constexpr int SQUARE_TYPES = 2; // 'X'; 'O'
static constexpr short EMPTY_TICTACTOE_BOARD = 0; // 9 empty squares

static constexpr short WIN_MASKS[] = {
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
	OPPENT_WON,
	I_WON,
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
	Board();

	BoardStatus getStatus() const { return status; }

	/// Initialize the board empty squares
	void init();

	/// Return the player index, played in the given position
	int getPlayerIdx(const Coords pos) const;

	/// Set the given player index in the given position
	void setPlayerIdx(const Coords pos, const int playerIdx);

	/// Appply the given move for the given player
	/// @param[in] move the coordinates on which the player plays
	/// @param[in] the player whose turn it is
	void playMove(const Coords move, const int player);

	/// Return list of all playable coordinates
	vector<Coords> getAllPossibleMoves() const;

	/// Play game with random moves until end of the game is reached
	/// @return the result of the game
	int simulateRandomGame();

	friend ostream& operator<<(std::ostream& stream, const Board& board);

private:
	short board[SQUARE_TYPES][BOARD_DIM]; /// Board for each player, each short representa a tictactoe board
	short bigBoard[SQUARE_TYPES]; /// Big Board for each player, each short representa a tictactoe board

	BoardStatus status; ///< Status of the board
};

//*************************************************************************************************************
//*************************************************************************************************************

Board::Board() :
	status{ BoardStatus::IN_PROGRESS }
{
	init();
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
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::getPlayerIdx(const Coords pos) const {
	int playerIdx = INVALID_IDX;

	const int bigBoardRowIdx = pos.getYCoord() / TRIPLE;
	const int bigBoardColIdx = pos.getXCoord() / TRIPLE;
	const int miniBoardIdx = (bigBoardRowIdx * TRIPLE) + bigBoardColIdx;

	const int miniBoardRowIdx = pos.getYCoord() % TRIPLE;
	const int miniBoardColIdx = pos.getXCoord() % TRIPLE;
	const short miniBoardInnerIdx = (miniBoardRowIdx * TRIPLE) + miniBoardColIdx;

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

}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::playMove(const Coords move, const int player) {
	setPlayerIdx(move, player);
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<Coords> Board::getAllPossibleMoves() const {
	return vector<Coords>();
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::simulateRandomGame() {
	return 0;
}

//*************************************************************************************************************
//*************************************************************************************************************

ostream& operator<<(std::ostream& stream, const Board& board) {
	for (int rowIdx = 0; rowIdx < BOARD_DIM; ++rowIdx) {
		if (rowIdx > 0 && 0 == rowIdx % TRIPLE) {
			stream << endl;
		}

		for (int colIdx = 0; colIdx < BOARD_DIM; ++colIdx) {
			if (colIdx > 0 && 0 == colIdx % TRIPLE) {
				stream << SPACE;
			}

			const int playerIdx = board.getPlayerIdx({ rowIdx, colIdx });

			switch (playerIdx) {
			case MY_PLAYER_IDX: { stream << MY_PLAYER_CHAR; break; }
			case OPPONENT_PLAYER_IDX: { stream << OPPONENT_PLAYER_CHAR; break; }
			default: { stream << EMPTY_CHAR; break; }
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
	State(const Board& board, const int player, const int visits, const double winScore);

	void setBoard(const Board& board) { this->board = board; }
	void setMove(Coords move) { this->move = move; }
	void setPlayer(const int player) { this->player = player; }
	void setVisits(const int visits) { this->visits = visits; }
	void setWinScore(const double winScore) { this->winScore = winScore; }

	const Board& getBoard() const { return board; }
	Board& getBoard() { return board; }
	Coords getMove() { return move; }
	int getPlayer() const { return player; }
	int getVisits() const { return visits; }
	double getWinScore() const { return winScore; }

private:
	Board board; ///< The game board state, which this node represents
	Coords move; ///< Move which led to this state
	int player; ///< The player index for which is this state
	int visits; ///< How many times this state is visited by the MCTS alogrithm
	double winScore; ///< Score of the state
};

//*************************************************************************************************************
//*************************************************************************************************************

State::State(const Board& board, const int player, const int visits, const double winScore) :
	board{ board },
	player{ player },
	visits{ visits },
	winScore{ winScore }
{
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

private:
	State state; ///< Game state information (wins/visits) including the board
	vector<int> children; ///< List of children nodes' ids in the list of Nodes for the trees
	int parentIdx; ///< The index of the parent node
	//int idx; ///< Unique id for the Node, may be removed after the game is solved
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

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/// Represent the search tree
class Tree {
public:
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

private:
	vector<Node> nodes; ///< All nodes used in the tree
};

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::init(const Board& initialBoard) {
	nodes.reserve(NODES_TO_RESERVE);

	State rootState{ initialBoard, 0, 0, 0 };
	Node rootNode{ rootState, INVALID_IDX };
	nodes.push_back(rootNode);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Tree::setRootPlayer(const int playerIdx) {
	nodes[0].getState().setPlayer(playerIdx);
}

//*************************************************************************************************************
//*************************************************************************************************************

int Tree::addNode(const Node& node) {
	nodes.push_back(node);
	return static_cast<int>(nodes.size() - 1);
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

	/// Find the best move
	void solve();

	/// Set the starting player for the game
	void setRootPlayer(const int playerIdx);

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
	/// @param[in] nodeWinScore
	/// @param[in] totalVisits
	/// @param[in] nodeVisit
	/// @return the UCT score
	double uct(const double nodeWinScore, const int totalVisits, const int nodeVisit) const;

	/// Conclude the search choosing the best move and updating the root
	void searchEnd();

	Tree searchTree; ///< The actual search tree for the algotrithm
	Coords opponentMove; ///< The last move for the opponent
	Coords bestMove; ///< The best move chosen from the simulation
	Board& initialBoard; ///< Current state of the board
	long long timeLimit; ///< How long to simulate
	double sqrtOf2; ///< Square root of tw, compute only once
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
	searchTree.init(initialBoard);
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
			const int selectedNodeIdx = selectPromisingNode();
			const Node& selectedNode = searchTree.getNode(selectedNodeIdx);
			if (BoardStatus::IN_PROGRESS == selectedNode.getState().getBoard().getStatus()) {
				expansion(selectedNodeIdx);
			}

			int nodeToExploreIdx = selectedNodeIdx;
			if (selectedNode.getChildrenCount() > 0) {
				nodeToExploreIdx = rand() % selectedNode.getChildrenCount();
			}

			int victoriousPlayer = simulation(nodeToExploreIdx);
			backPropagation(nodeToExploreIdx, victoriousPlayer);
		}

		searchEnd();
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::setRootPlayer(const int playerIdx) {
	searchTree.setRootPlayer(playerIdx);
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

void MonteCarloTreeSearch::expansion(const int selectedNode) {
	Node& parentNode = searchTree.getNode(selectedNode);
	const State& parentState = parentNode.getState();
	const Board& parentBoard = parentState.getBoard();
	vector<Coords> allMoves = parentBoard.getAllPossibleMoves();

	const int nextPlayer = PLAYER_TOGGLE - parentState.getPlayer() + 1;
	for (int moveIdx = 0; moveIdx < static_cast<int>(allMoves.size()); ++moveIdx) {
		Board childBoard{ parentBoard };
		childBoard.playMove(allMoves[moveIdx], nextPlayer);

		State childState{ childBoard, nextPlayer, 0, 0.0 };
		Node childNode{ childState, selectedNode };

		const int childNodeIdx = searchTree.addNode(childNode);
		parentNode.addChild(childNodeIdx);
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int MonteCarloTreeSearch::simulation(const int nodeToExploreIdx) {
	return searchTree.getNode(nodeToExploreIdx).getState().getBoard().simulateRandomGame();;
}

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::backPropagation(const int nodeToExploreIdx, const int victoriousPlayer) {
	int currentNodeIdx = nodeToExploreIdx;
	while (INVALID_IDX != currentNodeIdx) {
		Node& currentNode = searchTree.getNode(currentNodeIdx);
		State& currentNodeState = currentNode.getState();
		currentNodeState.setVisits(currentNodeState.getVisits() + 1);

		if (currentNodeState.getPlayer() == victoriousPlayer) {
			currentNodeState.setWinScore(currentNodeState.getWinScore() + WIN_VALUE);
		}

		currentNodeIdx = currentNode.getParentIdx();
	}
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

//*************************************************************************************************************
//*************************************************************************************************************

void MonteCarloTreeSearch::searchEnd() {
	const vector<int>& rootChildren = searchTree.getNode(turnRootNodeIdx).getChildren();

	int bestChildIdx = INVALID_IDX;
	double maxScore = 0.0;
	for (int childIdx = 0; childIdx < static_cast<int>(rootChildren.size()); ++childIdx) {
		const int childNodeIdx = rootChildren[childIdx];
		const double childScore = searchTree.getNode(childNodeIdx).getState().getWinScore();
		if (childScore > maxScore) {
			maxScore = childScore;
			bestChildIdx = childNodeIdx;
		}
	}

	bestMove = searchTree.getNode(bestChildIdx).getState().getMove();
	turnRootNodeIdx = bestChildIdx;
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
	cerr << board << endl;

	if (0 == turnsCount && opponentMove.isValid()) {
		monteCarloTreeSearch.setRootPlayer(OPPONENT_PLAYER_IDX);
	}
	else if (0 == turnsCount) {
		monteCarloTreeSearch.setRootPlayer(MY_PLAYER_IDX);
	}

	board.playMove(opponentMove, OPPONENT_PLAYER_IDX);

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
