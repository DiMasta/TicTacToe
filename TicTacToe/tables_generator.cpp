#include <iostream>
#include <map>
#include <ctime>
#include <ratio>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

static constexpr int BOARD_DIM = 9;
static constexpr int ALL_SQUARES = BOARD_DIM * BOARD_DIM;
static constexpr int INVALID_IDX = 9;
static constexpr int TRIPLE = 3;
static constexpr int SQUARE_POSSIBILITIES = 2;
static constexpr int SQUARE_EMPTY = 0;
static constexpr int SQUARE_FILLED = 1;
static constexpr int SQUARE_STATES[SQUARE_POSSIBILITIES] = { SQUARE_EMPTY, SQUARE_FILLED };
static constexpr short EMPTY_BOARD = 0b0000'000'000'000'000;
static constexpr short FULL_BOARD = 0b0000'000'111'111'111;

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

class Node {
public:
	Node() {}

private:
	short board[2][9];
	float visits;
	float winScore;
	float uctValue;
	int firstChild;
	int parentIdx;
	unsigned short flags;
	char childrenCount;
};

//****************************************************************************************************************************
//****************************************************************************************************************************

void printPossibleMoves(const short board) {
	cout << "\t{";

	int movesCount = 0;
	bool firstElementPrinted = false;
	for (int squareIdx = 0; squareIdx < BOARD_DIM; ++squareIdx) {
		const short squareMask = 1 << squareIdx;

		if (!(board & squareMask)) {
			if (firstElementPrinted) {
				cout << ",";
			}

			cout << squareIdx;

			if (!firstElementPrinted) {
				firstElementPrinted = true;
			}

			++movesCount;
		}
	}

	for (int nonMoveIdx = movesCount; nonMoveIdx < BOARD_DIM; ++nonMoveIdx) {
		cout << "," << INVALID_IDX;
	}

	cout << "}," << endl;
}

int countBits(const short board) {
	int bits = 0;

	for (int bitIdx = 0; bitIdx < BOARD_DIM; ++bitIdx) {
		bits += bool(board & (1 << bitIdx));
	}

	return bits;
}

//****************************************************************************************************************************
//****************************************************************************************************************************

int main() {
	//cout << "int ALL_MOVES[ALL_POSSOBLE_FILLED_BOARDS][BOARD_DIM] = {" << endl;
	//
	//for (short board = EMPTY_BOARD; board < FULL_BOARD; ++board) {
	//	printPossibleMoves(board);
	//}
	//
	//cout << "};" << endl;

	//*********************************************************************************************

	//cout << "short nextMiniBoard[BOARD_DIM][BOARD_DIM] = { ";
	//
	//for (int rowIdx = 0; rowIdx < BOARD_DIM; ++rowIdx) {
	//	for (int colIdx = 0; colIdx < BOARD_DIM; ++colIdx) {
	//		const int miniBoardRowIdx = rowIdx % TRIPLE;
	//		const int miniBoardColIdx = colIdx % TRIPLE;
	//		const short miniBoardInnerIdx = (miniBoardRowIdx * TRIPLE) + miniBoardColIdx;
	//
	//		//cout << "[" << rowIdx << ", " << colIdx << "] = " << miniBoardInnerIdx << endl;
	//		cout << miniBoardInnerIdx << ",";
	//	}
	//}
	//
	//cout << " };" << endl;

	//*********************************************************************************************

	//cout << "short currMiniBoard[BOARD_DIM][BOARD_DIM] = { ";
	//
	//for (int rowIdx = 0; rowIdx < BOARD_DIM; ++rowIdx) {
	//	for (int colIdx = 0; colIdx < BOARD_DIM; ++colIdx) {
	//		const int bigBoardRowIdx = rowIdx / TRIPLE;
	//		const int bigBoardColIdx = colIdx / TRIPLE;
	//		const int miniBoardIdx = (bigBoardRowIdx * TRIPLE) + bigBoardColIdx;
	//
	//		//cout << "[" << rowIdx << ", " << colIdx << "] = " << miniBoardIdx << endl;
	//		cout << miniBoardIdx << ",";
	//	}
	//}
	//
	//cout << " };" << endl;

	//*********************************************************************************************

	//cout << "bool WIN_BOARDS[ALL_POSSOBLE_FILLED_BOARDS] = {" << endl;
	//cout << "\t";
	//
	//for (short board = EMPTY_BOARD; board < FULL_BOARD; ++board) {
	//	bool playerWon = false;
	//
	//	for (int winMaskIdx = 0; winMaskIdx < WIN_MASKS_COUNT; ++winMaskIdx) {
	//		if (WIN_MASKS[winMaskIdx] == (WIN_MASKS[winMaskIdx] & board)) {
	//			playerWon = true;
	//			break;
	//		}
	//	}
	//
	//	if (0 != board && 0 == board % 100) {
	//		cout << endl << "\t";
	//	}
	//
	//	cout << playerWon << ",";
	//}
	//
	//cout << endl << "};" << endl;

	//*********************************************************************************************

	//cout << "Coords BIG_BOARD_POSITIONS[BOARD_DIM][BOARD_DIM] = {" << endl;
	//
	//for (int miniBoardIdx = 0; miniBoardIdx < BOARD_DIM; ++miniBoardIdx) {
	//	for (int miniBoardInnerIdx = 0; miniBoardInnerIdx < BOARD_DIM; ++miniBoardInnerIdx) {
	//		const int miniBoardRowIdx = miniBoardIdx / TRIPLE;
	//		const int miniBoardColIdx = miniBoardIdx % TRIPLE;
	//		const int miniBoardInnerRowIdx = miniBoardInnerIdx / TRIPLE;
	//		const int miniBoardInnerColIdx = miniBoardInnerIdx % TRIPLE;
	//		const int gloabalRowIdx = miniBoardInnerRowIdx + (miniBoardRowIdx * TRIPLE);
	//		const int gloabalColIdx = miniBoardInnerColIdx + (miniBoardColIdx * TRIPLE);
	//		
	//		//cout << "\t[" << miniBoardIdx << ", " << miniBoardInnerIdx << "] = " << "{ " << gloabalRowIdx << ", " << gloabalColIdx << " }," << endl;
	//		cout << "\t{ " << gloabalRowIdx << ", " << gloabalColIdx << " }," << endl;
	//	}
	//}
	//
	//cout << endl << "};" << endl;

	//*********************************************************************************************

	//cout << "int PLAYER_SQUARES_COUNT[ALL_POSSOBLE_FILLED_BOARDS] = {";
	//
	//for (short board = EMPTY_BOARD; board < FULL_BOARD; ++board) {
	//	int bitsCount = 0;
	//	for (int squareIdx = 0; squareIdx < BOARD_DIM; ++squareIdx) {
	//		if (board & (1 << squareIdx)) {
	//			++bitsCount;
	//		}
	//	}
	//
	//	cout << bitsCount << ",";
	//}

	//*********************************************************************************************

	//cout << "short BEST_WINNING_MOVES[ALL_POSSOBLE_FILLED_BOARDS] = {";
	//
	//for (short board = EMPTY_BOARD; board < FULL_BOARD; ++board) {
	//	int maxWinsForBoard = 0;
	//	int bestMove = 9; // Invalid
	//
	//	for (int squareIdx = 0; squareIdx < BOARD_DIM; ++squareIdx) {
	//		const short squareMask = (1 << squareIdx);
	//		if (!(board & squareMask)) {
	//			const short boardToTest = board | squareMask;
	//
	//			int wins = 0;
	//
	//			for (int winMaskIdx = 0; winMaskIdx < WIN_MASKS_COUNT; ++winMaskIdx) {
	//				if (WIN_MASKS[winMaskIdx] == (WIN_MASKS[winMaskIdx] & boardToTest)) {
	//					++wins;
	//				}
	//			}
	//
	//			if (wins > maxWinsForBoard) {
	//				maxWinsForBoard = wins;
	//				bestMove = squareIdx;
	//			}
	//		}
	//	}
	//
	//	cout << bestMove << ",";
	//}
	//
	//cout << "};" << endl;

	//*********************************************************************************************

	//cout << "short ONES_IN_BOARD[ALL_POSSOBLE_FILLED_BOARDS] = {" << endl;
	//
	//for (short board = EMPTY_BOARD; board < FULL_BOARD; ++board) {
	//	cout << countBits(board) << ",";
	//
	//	if (0 != board && 0 == board % 100) {
	//		cout << endl << "\t";
	//	}
	//}
	//
	//cout << "};" << endl;

	//*********************************************************************************************

	//cout << "short ZEROS_IN_BOARD[ALL_POSSOBLE_FILLED_BOARDS] = { ";
	//
	//for (short board = EMPTY_BOARD; board < FULL_BOARD; ++board) {
	//	cout << BOARD_DIM - countBits(board) << ",";
	//}
	//
	//cout << " };" << endl;

	//*********************************************************************************************

	//cout << "static const Coords BIG_BOARD_POSITION_FROM_IDX[ALL_SQUARES] = {" << endl;
	//
	//for (int idx = 0; idx < ALL_SQUARES; ++idx) {
	//	cout << "\tCoords{ " << idx / BOARD_DIM << ", " << idx % BOARD_DIM << " }," <<endl;
	//}
	//
	//cout << " };" << endl;

	//*********************************************************************************************

	cout << "static const char IDX_FROM_COORDS[BOARD_DIM][BOARD_DIM] = {" << endl;

	for (int rowIdx = 0; rowIdx < BOARD_DIM; ++rowIdx) {
		cout << "\t";
		for (int colIdx = 0; colIdx < BOARD_DIM; ++colIdx) {
			cout << (rowIdx * BOARD_DIM) + colIdx << ", ";
		}
		cout << endl;
	}

	cout << " };" << endl;

	return 0;
}