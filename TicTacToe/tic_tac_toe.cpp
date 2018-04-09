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
#include <math.h>
#include <climits>
#include <cstring>
#include <fstream>

using namespace std;

const bool OUTPUT_GAME_DATA = 1;
const bool USE_HARDCODED_INPUT = 1;

#define REDIRECT_CIN_FROM_FILE
const string INPUT_FILE_NAME = "input.txt";

const int INVALID_ID = -1;
const int INVALID_NODE_DEPTH = -1;
const int TREE_ROOT_NODE_DEPTH = 1;
const int ZERO_CHAR = '0';
const int DIRECTIONS_COUNT = 8;
const int BOARD_DIM = 3;
const int BOARD_SIZE = BOARD_DIM * BOARD_DIM;
const int INVALID_ROW_COL_IDX = -1;
const int BYTE_SIZE = 8;
const int WIN_CONDITIONS_COUNT = 8;

enum class BoardChars : char {
	ME = 'X',
	OPPONENT = 'O',
	EMPTY = 'E',
};

enum class BoardGrade : int {
	PLAYABLE = 44, // Only terminal boards' grades matter
	I_WIN = 1,
	DRAW = 0,
	OPPONENT_WINS = -1,
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

typedef unsigned short int Cells;
const int CELLS_BYTES_COUNT = sizeof(Cells) * BYTE_SIZE;
const Cells FIRST_CELL_BIT_1 = 32768; // 100 000 000 0000000
const Cells FULL_BOARD = 65408; // 111 111 111 0000000

const Cells WIN_CONDITIONS[WIN_CONDITIONS_COUNT] = {
	57344,	// 111 000 000 0000000
	7168,	// 000 111 000 0000000
	896,	// 000 000 111 0000000

	37376,	// 100 100 100 0000000
	18688,	// 010 010 010 0000000
	9344,	// 001 001 001 0000000

	34944,	// 100 010 001 0000000
	10752	// 001 010 100 0000000
};


class Board {
public:
	Board();
	~Board();

	Cells getEmptyCells() const {
		return emptyCells;
	}

	BoardChars getCell(int rowIdx, int colIdx) const;

	void setCell(int rowIdx, int colIdx, BoardChars value);
	void makeMove(Cells moveBit, BoardChars playerChar);

	int emptyCell(int rowIdx, int colIdx) const;

	BoardGrade grade() const;

	Cells removeCells(int rowIdx, int colIdx, Cells cells) const;

private:
	Cells emptyCells;
	Cells playersCells;
};

//*************************************************************************************************************
//*************************************************************************************************************

Board::Board() :
	emptyCells(0),
	playersCells(0)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Board::~Board() {

}

//*************************************************************************************************************
//*************************************************************************************************************

BoardChars Board::getCell(int rowIdx, int colIdx) const {
	BoardChars cellValue = BoardChars::EMPTY;

	if (!emptyCell(rowIdx, colIdx)) {
		Cells removedCells = removeCells(rowIdx, colIdx, playersCells);

		if (removedCells & 1) {
			cellValue = BoardChars::ME;
		}
		else {
			cellValue = BoardChars::OPPONENT;
		}
	}

	return cellValue;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::setCell(int rowIdx, int colIdx, BoardChars value) {
	Cells newCellMask = FIRST_CELL_BIT_1;

	newCellMask >>= colIdx;
	newCellMask >>= rowIdx * BOARD_DIM;

	if (BoardChars::ME == value) {
		playersCells |= newCellMask;
	}

	emptyCells |= newCellMask;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Board::makeMove(Cells moveBit, BoardChars playerChar) {
	Cells cellAlreadyMarked = moveBit & emptyCells;

	if (!cellAlreadyMarked) {
		emptyCells |= moveBit;

		if (BoardChars::ME == playerChar) {
			playersCells |= moveBit;
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int Board::emptyCell(int rowIdx, int colIdx) const {
	int empty = false;

	Cells removedCells = removeCells(rowIdx, colIdx, emptyCells);

	if (removedCells & 1) {
		empty = true;
	}

	return empty;
}

//*************************************************************************************************************
//*************************************************************************************************************

BoardGrade Board::grade() const {
	BoardGrade res = BoardGrade::PLAYABLE;

	int winner = false;

	for (int winCondIdx = 0; winCondIdx < WIN_CONDITIONS_COUNT; ++winCondIdx) {
		const Cells winnerMask = WIN_CONDITIONS[winCondIdx];
		const Cells extractPlayedCells = winnerMask & emptyCells;

		if (winnerMask == extractPlayedCells) {
			const Cells notEmptyPlayersCells = playersCells & winnerMask;

			const int iWin = (notEmptyPlayersCells == winnerMask);
			const int opponentWin = (0 == notEmptyPlayersCells);

			if (iWin || opponentWin) {
				res = iWin ? BoardGrade::I_WIN : BoardGrade::OPPONENT_WINS;
				winner = true;
				break;
			}
		}
	}

	if (!winner && (FULL_BOARD == emptyCells)) {
		res = BoardGrade::DRAW;
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

Cells Board::removeCells(int rowIdx, int colIdx, Cells cells) const {
	Cells removedBottomRows = cells >> (BOARD_DIM - rowIdx);
	Cells removedBackCells = removedBottomRows >> (BOARD_DIM - colIdx);

	return removedBackCells;
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

typedef int NodeId;

class Node {
public:
	Node();
	Node(NodeId id, int nodeDepth, NodeId parentId, const Board& board);
	~Node();

	NodeId getId() const {
		return id;
	}

	int getNodeDepth() const {
		return nodeDepth;
	}

	NodeId getParentId() const {
		return parentId;
	}

	Board getBoard() const {
		return board;
	}

	Board* getBoardPtr() {
		return &board;
	}

	const Board* getBoardConstPtr() const {
		return &board;
	}

	void setId(NodeId id) { this->id = id; }
	void setNodeDepth(int nodeDepth) { this->nodeDepth = nodeDepth; }
	void setParentId(NodeId parentId) { this->parentId = parentId; }
	void setBoard(const Board& board) { this->board = board; }

private:
	NodeId id;
	int nodeDepth;
	NodeId parentId;

	Board board;
};

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node() :
	id(INVALID_ID),
	nodeDepth(INVALID_NODE_DEPTH),
	parentId(INVALID_ID),
	board()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node(NodeId id, int nodeDepth, NodeId parentId, const Board& board) :
	id(id),
	nodeDepth(nodeDepth),
	parentId(parentId),
	board(board)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Node::~Node() {

}

//*************************************************************************************************************
//*************************************************************************************************************

typedef vector<NodeId> ChildrenList;
typedef map<NodeId, ChildrenList> GraphMap;
typedef map<NodeId, Node*> IdNodeMap;
typedef vector<NodeId> NodeStack;
typedef deque<NodeId> NodeQueue;
typedef set<NodeId> NodeSet;

class Graph {
public:
	Graph();
	Graph(int nodesCount, GraphMap graph);
	~Graph();

	int getNodesCount() const {
		return nodesCount;
	}

	GraphMap getGraph() const {
		return graph;
	}

	IdNodeMap getIdNodeMap() const {
		return idNodeMap;
	}

	Node* getNode(NodeId nodeId) const {
		return idNodeMap.at(nodeId);
	}

	void setNodesCount(int nodesCount) { this->nodesCount = nodesCount; }
	void setGraph(GraphMap graph) { this->graph = graph; }
	void setIdNodeMap(IdNodeMap idNodeMap) { this->idNodeMap = idNodeMap; }

	void createEdge(NodeId x, NodeId y);
	void addEdge(NodeId parentId, NodeId childId);
	void createNode(NodeId nodeId, int nodeDepth, const Board& board);
	bool nodeCreated(NodeId nodeId) const;
	void deleteAllNodes();
	vector<NodeId> treeRootsIds() const;
	int getMaxNodeDepth() const;
	bool edgeExists(NodeId parent, NodeId child) const;
	vector<NodeId> backtrack(NodeId from, NodeId to) const;
	NodeId getFirstNodeId() const;
	NodeId getLastNodeId() const;

private:
	int nodesCount;
	GraphMap graph;

	// Map used to store all nodes, used to check if node is already created
	// and for easy accesss when deleteing memory pointed by each node
	IdNodeMap idNodeMap;
};

//*************************************************************************************************************
//*************************************************************************************************************

Graph::Graph() :
	nodesCount(0),
	graph()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Graph::Graph(int nodesCount, GraphMap graph) :
	nodesCount(nodesCount),
	graph(graph)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Graph::~Graph() {
	deleteAllNodes();
	graph.clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::deleteAllNodes() {
	for (IdNodeMap::iterator it = idNodeMap.begin(); it != idNodeMap.end(); ++it) {
		Node* node = it->second;

		if (node) {
			delete node;
			node = NULL;
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<NodeId> Graph::treeRootsIds() const {
	vector<NodeId> res;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		NodeId nodeId = nodeIt->first;

		bool isChildOfANode = false;

		for (GraphMap::const_iterator graphIt = graph.begin(); graphIt != graph.end(); ++graphIt) {
			ChildrenList childrenList = graphIt->second;
			if (find(childrenList.begin(), childrenList.end(), nodeId) != childrenList.end()) {
				isChildOfANode = true;
				break;
			}
		}

		if (!isChildOfANode) {
			res.push_back(nodeId);
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

int Graph::getMaxNodeDepth() const {
	int maxNodeDepth = INVALID_NODE_DEPTH;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		int nodeDepth = nodeIt->second->getNodeDepth();
		if (nodeDepth > maxNodeDepth) {
			maxNodeDepth = nodeDepth;
		}
	}

	return maxNodeDepth;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::edgeExists(NodeId parent, NodeId child) const {
	bool res = false;

	if (nodeCreated(parent) && nodeCreated(child)) {
		ChildrenList children = graph.at(parent); // TODO: copying do not copy use * for children
		res = find(children.begin(), children.end(), child) != children.end();
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<NodeId> Graph::backtrack(NodeId from, NodeId to) const {
	vector<NodeId> path;

	while (from != to) {
		path.push_back(from);
		from = idNodeMap.at(from)->getParentId();
	}

	path.push_back(to);

	reverse(path.begin(), path.end());

	return path;
}

//*************************************************************************************************************
//*************************************************************************************************************

NodeId Graph::getFirstNodeId() const {
	return idNodeMap.begin()->first;
}

//*************************************************************************************************************
//*************************************************************************************************************

NodeId Graph::getLastNodeId() const {
	return idNodeMap.rbegin()->first;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::addEdge(NodeId parentId, NodeId childId) {
	graph[parentId].push_back(childId);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::createNode(NodeId nodeId, int nodeDepth, const Board& board) {
	if (!nodeCreated(nodeId)) {
		Node* node = new Node(nodeId, nodeDepth, INVALID_ID, board);
		idNodeMap[node->getId()] = node;
		++nodesCount;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::nodeCreated(NodeId nodeId) const {
	return idNodeMap.end() != idNodeMap.find(nodeId);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::createEdge(NodeId x, NodeId y) {
	createNode(x, INVALID_NODE_DEPTH, Board());
	createNode(y, INVALID_NODE_DEPTH, Board());

	addEdge(x, y);
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

struct MiniMaxResult {
	MiniMaxResult() :
		bestLeaveNode(INVALID_ID),
		evaluationValue(0)
	{}

	MiniMaxResult(
		NodeId bestLeaveNode,
		int evaluationValue
	) :
		bestLeaveNode(bestLeaveNode),
		evaluationValue(evaluationValue)
	{}

	NodeId bestLeaveNode;
	int evaluationValue;
};

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class MiniMax {
public:
	MiniMax();
	~MiniMax();

	void run(const Board& initialBoard);
	Node makeChild(const Node& parent, Cells moveBit, BoardChars playerChar);
	MiniMaxResult maximize(Node& node, int alpha, int beta);
	MiniMaxResult minimize(Node& node, int alpha, int beta);

private:
	Graph miniMaxTree;
};

//*************************************************************************************************************
//*************************************************************************************************************

MiniMax::MiniMax() :
	miniMaxTree()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

MiniMax::~MiniMax() {
	miniMaxTree.deleteAllNodes();
}

//*************************************************************************************************************
//*************************************************************************************************************

void MiniMax::run(const Board& initialBoard) {
	miniMaxTree.createNode(0, 0, initialBoard);
	MiniMaxResult miniMaxRes = maximize(*miniMaxTree.getNode(0), INT_MIN, INT_MAX);


}

//*************************************************************************************************************
//*************************************************************************************************************

Node MiniMax::makeChild(const Node& parent, Cells moveBit, BoardChars playerChar) {
	NodeId childId = miniMaxTree.getLastNodeId() + 1;
	Node child = Node(childId, parent.getNodeDepth() + 1, parent.getId(), parent.getBoard());
	child.getBoardPtr()->makeMove(moveBit, playerChar);
	miniMaxTree.addEdge(parent.getId(), childId);

	return child;
}

//*************************************************************************************************************
//*************************************************************************************************************

MiniMaxResult MiniMax::maximize(Node& node, int alpha, int beta) {
	const BoardGrade grade = node.getBoardConstPtr()->grade();

	if (BoardGrade::PLAYABLE != grade) {
		// Terminal state
		return MiniMaxResult(node.getId(), static_cast<int>(grade));
	}

	MiniMaxResult res(INVALID_ID, INT_MIN);

	Cells moveBit = FIRST_CELL_BIT_1;
	for (int moveIdx = 0; moveIdx < BOARD_SIZE; ++moveIdx) {
		if (node.getBoardConstPtr()->getEmptyCells() & moveBit) {
			moveBit >>= 1;
			continue;
		}

		Node child = makeChild(node, moveBit, BoardChars::ME);
		MiniMaxResult minRes = minimize(child, alpha, beta);

		if (minRes.evaluationValue > res.evaluationValue) {
			res = minRes;
		}

		if (res.evaluationValue >= beta) {
			break;
		}

		if (res.evaluationValue > alpha) {
			alpha = res.evaluationValue;
		}

		moveBit >>= 1;
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

MiniMaxResult MiniMax::minimize(Node& node, int alpha, int beta) {
	const BoardGrade grade = node.getBoardConstPtr()->grade();

	if (BoardGrade::PLAYABLE != grade) {
		// Terminal state
		return MiniMaxResult(node.getId(), static_cast<int>(grade));
	}

	MiniMaxResult res(INVALID_ID, INT_MAX);

	Cells moveBit = FIRST_CELL_BIT_1;
	for (int moveIdx = 0; moveIdx < BOARD_SIZE; ++moveIdx) {
		if (node.getBoardConstPtr()->getEmptyCells() & moveBit) {
			moveBit >>= 1;
			continue;
		}

		Node child = makeChild(node, moveBit, BoardChars::OPPONENT);
		MiniMaxResult maxRes = maximize(child, alpha, beta);

		if (maxRes.evaluationValue < res.evaluationValue) {
			res = maxRes;
		}

		if (res.evaluationValue <= alpha) {
			break;
		}

		if (res.evaluationValue < beta) {
			beta = res.evaluationValue;
		}

		moveBit >>= 1;
	}

	return res;
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
	void gameLoop();
	void getGameInput();
	void getTurnInput();
	void turnBegin();
	void makeTurn();
	void turnEnd();
	void play();

	void debug() const;

private:
	int turnsCount;

	Board board;
	MiniMax miniMax;
};

//*************************************************************************************************************
//*************************************************************************************************************

Game::Game() :
	turnsCount(0),
	board(),
	miniMax()
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

void Game::gameLoop() {
	while (true) {
		turnBegin();
		getTurnInput();
		makeTurn();
		turnEnd();
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

	if (INVALID_ROW_COL_IDX != opponentRow && INVALID_ROW_COL_IDX != opponentCol) {
		board.setCell(opponentRow, opponentCol, BoardChars::OPPONENT);
	}

	//int validActionCount;
	//cin >> validActionCount; cin.ignore();
	//
	//for (int i = 0; i < validActionCount; i++) {
	//	int row;
	//	int col;
	//	cin >> row >> col; cin.ignore();
	//}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::turnBegin() {
}

//*************************************************************************************************************
//*************************************************************************************************************

void Game::makeTurn() {
	miniMax.run(board);

	cout << "0 0" << endl;
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

#ifdef REDIRECT_CIN_FROM_FILE
	ifstream in(INPUT_FILE_NAME);
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());
#endif

	Game game;
	game.play();
#endif // TESTS

	return 0;
}