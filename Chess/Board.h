#pragma once

#include "MacrosIncludes.h"

class Tile;

class Board {

private:

	std::vector< std::vector<Tile*> > boardTiles;
	bool bWhiteMove = true;
	bool validatePieceMove(int x, int y, int xTo, int yTo);
	bool colorCheck(int x, int y, int xTo, int yTo);
	
	
	struct PrevMove {

		int fromX;
		int fromY;

		int toX;
		int toY;

		Tile* pieceTaken;

		PrevMove(int x, int y, int tx, int ty, Tile* piece);
		PrevMove();

	} prevMove;


	inline bool isBlank(int x, int y);

public:

	struct KingLoc {
		int x;
		int y;
	} WhiteKing, BlackKing;

	std::vector< std::vector<Tile*> > getBoard();
	void translateXY(int& x, int& y);
	Tile* getTileAtPoint(int x, int y);
	void setTileAtPoint(int x, int y, Tile* tile);
	Board::KingLoc getKingLocation(Color color);

	void makeMove(str from, str to, bool underCheck);
	Move validateMove(char x, int y, char xTo, int yTo);
	void undo();

	bool underAttack(Color opp, int x, int y);
	bool underAttack(Color opp, int x, int y, int& xOut, int& yOut);

	void debug();

	int boardWidth = 3;

	bool getMove();
	void setMove(bool move);

	Board();

	~Board() {
		boardTiles.clear();
	}
	

};