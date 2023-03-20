#pragma once

#include "MacrosIncludes.h"

class Tile;

class Board {

private:

	std::vector< std::vector<Tile*> > boardTiles;
	bool bWhiteMove = true;
	bool validatePieceMove(Board* board, int x, int y, int xTo, int yTo);
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

public:

	std::vector< std::vector<Tile*> > getBoard();
	void translateXY(int& x, int& y);
	Tile* getTileAtPoint(int x, int y);
	void setTileAtPoint(int x, int y, Tile* tile);
	void getKingLocation(Color color, int& x, int& y);

	void makeMove(str from, str to);
	Move validateMove(Board* board, char x, int y, char xTo, int yTo);
	void undo();

	bool underAttack(Color opp, int x, int y);

	void debug();

	int boardWidth = 3;

	bool getMove();
	void setMove(bool move);

	Board();

};