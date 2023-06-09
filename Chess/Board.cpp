#pragma once

#include "MacrosIncludes.h";
#include "Tile.h"
#include "Board.h"

bool Board::getMove()
{
	return this->bWhiteMove;
}


Board::Board() {

	boardTiles.resize(SIZE, std::vector<Tile*>(SIZE, new Tile()));

	boardTiles[0][7] = new Tile(R);
	boardTiles[1][7] = new Tile(H);
	boardTiles[2][7] = new Tile(B);
	boardTiles[3][7] = new Tile(Q);
	boardTiles[4][7] = new Tile(K);
	boardTiles[5][7] = new Tile(B);
	boardTiles[6][7] = new Tile(H);
	boardTiles[7][7] = new Tile(R);

	boardTiles[0][0] = new Tile(R, BLK);
	boardTiles[1][0] = new Tile(H, BLK);
	boardTiles[2][0] = new Tile(B, BLK);
	boardTiles[3][0] = new Tile(Q, BLK);
	boardTiles[4][0] = new Tile(K, BLK);
	boardTiles[5][0] = new Tile(B, BLK);
	boardTiles[6][0] = new Tile(H, BLK);
	boardTiles[7][0] = new Tile(R, BLK);

	for (int i = 0; i <= 7; i++) {

		boardTiles[i][1] = new Tile(P, BLK);

	}

	for (int i = 0; i <= 7; i++) {

		boardTiles[i][6] = new Tile(P);

	}

	this->BlackKing.x = 5;
	this->WhiteKing.x = 5;

	this->WhiteKing.y = 1;
	this->BlackKing.y = 8;

	

}

std::vector<std::vector<Tile*>> Board::getBoard()
{
	return this->boardTiles;
}

void Board::translateXY(int& x, int& y) {

	//if (x == 0 || y == 0) {
	//	throw std::invalid_argument("Translating from cartesian to index. (1, 1) is first point.");
	//}

	//if (x > SIZE || y > SIZE || x < 1 || y < 1) {
	//	std::cout << "error";
	//	throw std::invalid_argument("Argument(s) out of bounds.");
	//	std::cout << "error";
	//}

	x = x - 1;
	y = SIZE - y;
}

Tile* Board::getTileAtPoint(int x, int y) {

	int xcoord = x;
	int ycoord = y;

	this->translateXY(xcoord, ycoord);
	return this->boardTiles[xcoord][ycoord];


}

void Board::setTileAtPoint(int x, int y, Tile* tile) {

	int xcoord = x;
	int ycoord = y;

	this->translateXY(xcoord, ycoord);

	this->boardTiles[xcoord][ycoord] = tile;


}

void Board::setMove(bool move) {
	this->bWhiteMove = move;
}

void Board::makeMove(str from, str to, bool underCheck) {

	if (from.length() > 2 || to.length() > 2 || from.length() <= 1 || to.length() <= 1) {
		throw (Move::ARGUMENT_ERROR);
	}


	int fromX = int(char(from[0]) - 64);
	int fromY = int(char(from[1]) - '0');
	
	int toX = int(char(to[0]) - 64);
	int toY = int(char(to[1]) - '0');

	Move move = this->validateMove(from[0], fromY, to[0], toY);

	if (move != Move::VALID) {
		throw (move);
	}

	this->bWhiteMove = !bWhiteMove;

	prevMove = Board::PrevMove(fromX, fromY, toX, toY, ((this->getTileAtPoint(toX, toY)->toString().compare(str("Blank")) == 0) ? nullptr : this->getTileAtPoint(toX, toY)));

	this->setTileAtPoint(toX, toY, this->getTileAtPoint(fromX, fromY));
	this->setTileAtPoint(fromX, fromY, new Tile());

	//pawn promo

	for (int i = 1; i <= SIZE; i++) {
		if (this->getTileAtPoint(i, SIZE)->getType() == Pieces::PAWN && this->getTileAtPoint(i, SIZE)->getColor() == Color::WHITE) {
			this->getTileAtPoint(i, SIZE)->changeTile(Pieces::QUEEN);
		}
	}

	for (int i = 1; i <= SIZE; i++) {

		if (this->getTileAtPoint(i, 1)->getType() == Pieces::PAWN && this->getTileAtPoint(i, 0)->getColor() == Color::BLACK) {
			this->getTileAtPoint(i, 1)->changeTile(Pieces::QUEEN);
		}
	}

	

}

Move Board::validateMove(char x, int y, char xTo, int yTo)
{

	if (int(xTo) < int('A') || int(xTo) > int('H') ||int(x) < int('A') || int(x) > int('H') || y < 1 || y > 8 || yTo < 1 || yTo > 8) {
		return Move::INVALID_PIECE;
	}

	if (this->bWhiteMove && this->getTileAtPoint(int(x - 64), y)->getColor() == Color::BLACK) {
		return Move::INVALID_PIECE;
	}
	if (!(this->bWhiteMove) && this->getTileAtPoint(int(x - 64), y)->getColor() == Color::WHITE) {
		return Move::INVALID_PIECE;
	}

	if (this->getTileAtPoint(int(x - 64), y)->toString().compare(str("Blank")) == 0) {
		return Move::INVALID_PIECE;
	}

	if (x == xTo && y == yTo) {
		return Move::ARGUMENT_ERROR;
	}

	if (!(this->validatePieceMove(int(x - 64), y, int(xTo - 64), yTo))) {
		return Move::INVALID_MOVE;
	}


	return Move::VALID;


}

bool Board::validatePieceMove(int x, int y, int xTo, int yTo)
{
	Pieces pieceType = this->getTileAtPoint(x, y)->getType();
	Color pieceColor = this->getTileAtPoint(x, y)->getColor();

	if (pieceType == P) {

		//move forward 2 square on starting row
		if (std::abs(yTo - y) == 2 && !(std::abs(xTo - x) >= 1) && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) == 0) {
			if (pieceColor == BLK && y == 7 && this->getTileAtPoint(xTo, yTo+1)->toString().compare(str("Blank")) == 0) {
				return true;
			}

			if (pieceColor == WHT && y == 2 && this->getTileAtPoint(xTo, yTo - 1)->toString().compare(str("Blank")) == 0) {
				return true;
			}
			return false;
		}

		//move forward
		if (yTo - y == 1 && !(std::abs(xTo - x) >=1) && pieceColor == WHT && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) == 0) {
			return true;
		}

		if (yTo - y == -1 && !(std::abs(xTo - x) >= 1) && pieceColor == BLK && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) == 0) {
			return true;
		}

		//take diagonally
		if (pieceColor == WHT && yTo - y == 1 && xTo - x == -1 && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) != 0 && this->getTileAtPoint(xTo, yTo)->getColor() == BLK) {
			return true;
		}

		if (pieceColor == WHT && yTo - y == 1 && xTo - x == 1 && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) != 0 && this->getTileAtPoint(xTo, yTo)->getColor() == BLK) {
			return true;
		}

		if (pieceColor == BLK && yTo - y == -1 && xTo - x == -1 && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) != 0 && this->getTileAtPoint(xTo, yTo)->getColor() == WHT) {
			return true;
		}

		if (pieceColor == BLK && yTo - y == -1 && xTo - x == 1 && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) != 0 && this->getTileAtPoint(xTo, yTo)->getColor() == WHT) {
			return true;
		}

		return false;
	}

	if (pieceType == R) {

		if (xTo - x != 0 && yTo - y != 0) {
			return false;
		}

		if (yTo - y == 0) {

			for (int i = x+1; i < xTo; i++) {
				if (this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0) {
					return false;
				}
			}

		}

		if (xTo - x == 0) {
			for (int i = y+1; i < yTo; i++) {
				if (this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0) {
					return false;
				}
			}
		}

		if (this->getTileAtPoint(x, y)->getColor() == WHT && this->getTileAtPoint(xTo, yTo)->getColor() == WHT || this->getTileAtPoint(x, y)->getColor() == BLK && this->getTileAtPoint(xTo, yTo)->getColor() == BLK) {
			return false;
		}

		if (this->getTileAtPoint(x, y)->getColor() == WHT && this->getTileAtPoint(xTo, yTo)->getColor() == BLK) {
			return true;
		}
		if (this->getTileAtPoint(x, y)->getColor() == BLK && this->getTileAtPoint(xTo, yTo)->getColor() == WHT) {
			return true;
		}

		return true;


	}

	if (pieceType == H) {

		if (this->colorCheck(x, y, xTo, yTo)) {
			return false;
		}

		if ((2 == std::abs(xTo - x)) && (1 == std::abs(yTo - y)))
		{
			return true;
			
		}

		if ((2 == std::abs(yTo - y)) && (1 == std::abs(xTo - x)))
		{
			return true;
		}

		return false;
	}

	if (pieceType == B) {
		
		if (std::abs(xTo - x) == std::abs(yTo - y)) {
			
			//up right
			if (xTo > x && yTo > y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x + i, y + i)->toString().compare(str("Blank")) != 0) {
						return false;
					}

				}
			}

			if (xTo > x && yTo < y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x + i, y - i)->toString().compare(str("Blank")) != 0) {
						return false;
					}

				}
			}


			if (xTo < x && yTo > y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x - i, y + i)->toString().compare(str("Blank")) != 0) {
						return false;
					}
				}
			}

			if (xTo < x && yTo < y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x - i, y - i)->toString().compare(str("Blank")) != 0) {
						return false;
					}

				}
			}

			if (this->colorCheck(x, y, xTo, yTo)) {
				return false;
			}

			
			return true;
		}

		return false;

	}

	if (pieceType == K) {

		Color opp = (pieceColor == WHT) ? BLK : WHT;
		//castling

		if (pieceColor == WHT && x == 5 && (xTo == 7 || xTo == 3) && y == 1) {
			if (xTo == 7) {

				for (int i = x+1; i <= xTo; i++) {
					if (this->getTileAtPoint(i, y)->getType() != Pieces::BLANK || this->underAttack(opp, i, y) || this->getTileAtPoint(8, y)->getType() != R) {
						return false;
					}
				}

				this->getTileAtPoint(8, 1)->changeTile(Pieces::BLANK);
				this->getTileAtPoint(6, 1)->changeTile(Pieces::ROOK);
				return true;
			}
			if (xTo == 3) {
				for (int i = x - 1; i >= xTo; i--) {
					if (this->getTileAtPoint(i, y)->getType() != Pieces::BLANK || this->underAttack(opp, i, y) || this->getTileAtPoint(1, y)->getType() != R) {
						return false;
					}
				}
				this->getTileAtPoint(1, 1)->changeTile(Pieces::BLANK);
				this->getTileAtPoint(4, 1)->changeTile(Pieces::ROOK);
				return true;
			}
		}

		if (pieceColor == BLK && x == 5 && (xTo == 7 || xTo == 3) && y == 8) {
			if (xTo == 7) {
				for (int i = x+1; i <= xTo; i++) {
					if (this->getTileAtPoint(i, y)->getType() != Pieces::BLANK || this->underAttack(opp, i, y) || this->getTileAtPoint(8, y)->getType() != R) {
						return false;
					}
				}
				this->getTileAtPoint(8, 8)->changeTile(Pieces::BLANK);
				this->getTileAtPoint(6, 8)->changeTile(Pieces::ROOK);
				return true;
			}
			if (xTo == 3) {
				for (int i = x-1; i >= xTo; i--) {
					if (this->getTileAtPoint(i, y)->getType() != Pieces::BLANK || this->underAttack(opp, i, y) || this->getTileAtPoint(1, y)->getType() != R) {
						return false;
					}
				}
				this->getTileAtPoint(1, 8)->changeTile(Pieces::BLANK);
				this->getTileAtPoint(4, 8)->changeTile(Pieces::ROOK);
				return true;
			}
		}

		if (this->colorCheck(x, y, xTo, yTo)) {
			return false;
		}

		if (std::abs(x - xTo) == 1 || std::abs(y - yTo) == 1) {
			if (this->underAttack(opp, xTo, yTo)) {
				return false;
			}
			else {
				if (pieceColor == WHT) {
					this->WhiteKing.x = this->WhiteKing.x + (xTo - x);
					this->WhiteKing.y = this->WhiteKing.y + (yTo - y);
				}

				else {
					this->BlackKing.x = this->BlackKing.x + (xTo - x);
					this->BlackKing.y = this->BlackKing.y + (yTo - y);
				}

				return true;
			} 
		}

		return false;
	}


	if (pieceType == Q) {

		if (std::abs(xTo - x) == std::abs(yTo - y)) {

			//up right
			if (xTo > x && yTo > y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x + i, y + i)->toString().compare(str("Blank")) != 0) {
						return false;
					}

				}
			}

			if (xTo > x && yTo < y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x + i, y - i)->toString().compare(str("Blank")) != 0) {
						return false;
					}

				}
			}


			if (xTo < x && yTo > y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x - i, y + i)->toString().compare(str("Blank")) != 0) {
						return false;
					}
				}
			}

			if (xTo < x && yTo < y) {
				for (int i = 1; i < std::abs(xTo - x); i++) {
					if (this->getTileAtPoint(x - i, y - i)->toString().compare(str("Blank")) != 0) {
						return false;
					}

				}
			}

			if (this->colorCheck(x, y, xTo, yTo)) {
				return false;
			}


			return true;
		}

		if (x == xTo || y == yTo) {

			if (yTo - y == 0) {

				for (int i = x + 1; i < xTo; i++) {
					if (this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0) {
						return false;
					}
				}

			}

			if (xTo - x == 0) {
				for (int i = y + 1; i < yTo; i++) {
					if (this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0) {
						return false;
					}
				}
			}

			return true;

		}



		return false;
	}


	std::cout << "What happened here????????"; wait;
	return true;

}

//checks if tile iis same color as selected piece so you cant just move on top of your own pieces
//the thing is, i made all "tiles" have a color, despite it being empty
//so I have to work around this a little since its a little late to change it
bool Board::colorCheck(int x, int y, int xTo, int yTo)
{
	return this->getTileAtPoint(x, y)->getColor() == WHT && this->getTileAtPoint(xTo, yTo)->getColor() == WHT && this->getTileAtPoint(xTo, yTo)->toString().compare(str("Blank")) != 0 || this->getTileAtPoint(x, y)->getColor() == BLK && this->getTileAtPoint(xTo, yTo)->getColor() == BLK;
}

 Board::KingLoc Board::getKingLocation(Color color)
{
	if (color == WHT) {
		return WhiteKing;
	}

	return BlackKing;
}


 bool Board::underAttack(Color opp, int x, int y, int& xOut, int& yOut, bool isDefending)
 {
	
	Color self = ((opp == WHT) ? BLK : WHT);

	for (int i = x + 1; i <= 8; i++) {

		//if blocked by own pieces or other pieces then break
		//horizontal
		if ((this->getTileAtPoint(i, y)->getColor() == self && !(this->isBlank(i, y)) || ((this->getTileAtPoint(i, y)->getType() != R && (this->getTileAtPoint(i, y)->getType() != Q && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0))))) {
			break;
		}
		if (this->getTileAtPoint(i, y)->getColor() == opp && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(i, y)->getType() == Q || this->getTileAtPoint(i, y)->getType() == R)) {
			xOut = i;
			yOut = y;
			return true;
		}

	}

	for (int i = x - 1; i >= 1; i--) {

		if ((this->getTileAtPoint(i, y)->getColor() == self && !(this->isBlank(i, y)) || ((this->getTileAtPoint(i, y)->getType() != R && this->getTileAtPoint(i, y)->getType() != Q && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0)))) {
			break;
		}
		if (this->getTileAtPoint(i, y)->getColor() == opp && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(i, y)->getType() == Q || this->getTileAtPoint(i, y)->getType() == R)) {
			xOut = i;
			yOut = y;
			return true;
		}

	}

	//vertical
	for (int i = y + 1; i <= 8; i++) {

		if ((this->getTileAtPoint(x, i)->getColor() == self && !(this->isBlank(x, i)) || ((this->getTileAtPoint(x, i)->getType() != R && (this->getTileAtPoint(x, i)->getType() != Q && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0))))) {
			break;
		}
		if (this->getTileAtPoint(x, i)->getColor() == opp && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x, i)->getType() == Q || this->getTileAtPoint(x, i)->getType() == R)) {
			xOut = x;
			yOut = i;
			return true;
		}

	}

	for (int i = y - 1; i >= 1; i--) {


		if ((this->getTileAtPoint(x, i)->getColor() == self && !(this->isBlank(x, i)) || ((this->getTileAtPoint(x, i)->getType() != R && !(this - isBlank(x, i))) && (this->getTileAtPoint(x, i)->getType() != Q)))) {
			//std::cout << ((this->getTileAtPoint(x, i)->getType() != R && !(this - isBlank(x, i))) || (this->getTileAtPoint(x, i)->getType() != Q && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0));
			break;
		}
		if (this->getTileAtPoint(x, i)->getColor() == opp && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x, i)->getType() == Q || this->getTileAtPoint(x, i)->getType() == R)) {
			xOut = x;
			yOut = i;
			return true;
		}

	}

	//diagonal 

	for (int i = 1; x + i <= 8 && y + i <= 8; i++) {

		if (this->getTileAtPoint(x + i, y + i)->getColor() == self && !(this->isBlank(x + i, y + i)) || ((this->getTileAtPoint(x + i, y + i)->getType() != Q && !(this->isBlank(x + i, y + i))) && (this->getTileAtPoint(x + i, y + i)->getType() != B && this->getTileAtPoint(x + i, y + i)->toString().compare(str("Blank")) != 0))) {
			break;
		}
		if (this->getTileAtPoint(x + i, y + i)->getColor() == opp && this->getTileAtPoint(x + i, y + i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x + i, y + i)->getType() == Q || this->getTileAtPoint(x + i, y + i)->getType() == B)) {
			xOut = x + i;
			yOut = y + i;
			return true;
		}
	}

	for (int i = 1; x + i <= 8 && y - i >= 1; i++) {

		if ((this->getTileAtPoint(x + i, y - i)->getColor() == self && !(this->isBlank(x + i, y - i)) || ((this->getTileAtPoint(x + i, y - i)->getType() != Q && !(this->isBlank(x + i, y - i))) && (this->getTileAtPoint(x + i, y - i)->getType() != B && this->getTileAtPoint(x + i, y - i)->toString().compare(str("Blank")) != 0)))) {
			break;
		}
		if (this->getTileAtPoint(x + i, y - i)->getColor() == opp && this->getTileAtPoint(x + i, y - i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x + i, y - i)->getType() == Q || this->getTileAtPoint(x + i, y - i)->getType() == B)) {
			xOut = x + i;
			yOut = y - i;
			return true;
		}
	}

	for (int i = 1; x - i >= 1 && y - i >= 1; i++) {


		if ((this->getTileAtPoint(x - i, y - i)->getColor() == self && !(this->isBlank(x - i, y - i)) || ((this->getTileAtPoint(x - i, y - i)->getType() != Q && !(this->isBlank(x - i, y - i))) && (this->getTileAtPoint(x - i, y - i)->getType() != B && this->getTileAtPoint(x - i, y - i)->toString().compare(str("Blank")) != 0)))) {
			break;
		}
		if (this->getTileAtPoint(x - i, y - i)->getColor() == opp && this->getTileAtPoint(x - i, y - i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x - i, y - i)->getType() == Q || this->getTileAtPoint(x - i, y - i)->getType() == B)) {
			xOut = x - i;
			yOut = y - i;
			return true;
		}
	}


	for (int i = 1; x - i >= 1 && y + i <= 8; i++) {

		if ((this->getTileAtPoint(x - i, y + i)->getColor() == self && !(this->isBlank(x - i, y + i)) || (this->getTileAtPoint(x - i, y + i)->getType() != Q && !(this->isBlank(x - i, y + i))) && (this->getTileAtPoint(x - i, y + i)->getType() != B && this->getTileAtPoint(x - +i, y + i)->toString().compare(str("Blank")) != 0))) {
			break;
		}
		if (this->getTileAtPoint(x - i, y + i)->getColor() == opp && this->getTileAtPoint(x - i, y + i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x - i, y + i)->getType() == Q || this->getTileAtPoint(x - i, y + i)->getType() == B)) {
			xOut = x - i;
			yOut = y + i;
			return true;
		}
	}

	//knight

	int knightMoves[8][2] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
								   { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

	for (int i = 0; i <= 7; i++) {
		if (x + knightMoves[i][0] > 8 || y + knightMoves[i][1] > 8 || x + knightMoves[i][0] < 1 || y + knightMoves[i][1] < 1) {
			continue;
		}
		if (this->getTileAtPoint(x + knightMoves[i][0], y + knightMoves[i][1])->getColor() == opp && !(this->isBlank(x + knightMoves[i][0], y + knightMoves[i][1])) && this->getTileAtPoint(x + knightMoves[i][0], y + knightMoves[i][1])->getType() == Pieces::KNIGHT) {
			xOut = x + knightMoves[i][0];
			yOut = y + knightMoves[i][1];
			return true;
		}

	}

	//pawn

	if (opp == BLK) {

		if (!isDefending) {
			if (x + 1 <= 8 && y + 1 <= 8) {

				if (this->getTileAtPoint(x + 1, y + 1)->getColor() == opp && this->getTileAtPoint(x + 1, y + 1)->getType() == P) {
					xOut = x + 1;
					yOut = y + 1;
					return true;
				}
			}

			if (x - 1 >= 1 && y + 1 <= 8) {
				if (this->getTileAtPoint(x - 1, y + 1)->getColor() == opp && this->getTileAtPoint(x - 1, y + 1)->getType() == P) {
					xOut = x - 1;
					yOut = y + 1;
					return true;
				}
			}
		}

		if (isDefending) {

			if (y + 1 <= 8) {
				if (this->getTileAtPoint(x, y + 1)->getColor() == opp && this->getTileAtPoint(x, y + 1)->getType() == P) {
					xOut = x;
					yOut = y + 1;
					return true;
				}
			}

			if (y == 5) {
				if (this->getTileAtPoint(x, y + 2)->getColor() == opp && this->getTileAtPoint(x, y + 2)->getType() == P) {
					xOut = x;
					yOut = y + 2;
					return true;
				}


			}
		}

	}
	if (opp == WHT) {
		if (!isDefending) {
			if (x + 1 <= 8 && y - 1 >= 1) {
				if (this->getTileAtPoint(x + 1, y - 1)->getColor() == opp && this->getTileAtPoint(x + 1, y - 1)->getType() == P) {
					xOut = x + 1;
					yOut = y - 1;
					return true;
				}
			}
			if (x - 1 >= 1 && y - 1 >= 1) {
				if (this->getTileAtPoint(x - 1, y - 1)->getColor() == opp && this->getTileAtPoint(x - 1, y - 1)->getType() == P) {
					xOut = x - 1;
					yOut = y - 1;
					return true;
				}
			}
		}
		if (isDefending) {
			if (y - 1 <= 8) {
				if (this->getTileAtPoint(x, y - 1)->getColor() == opp && this->getTileAtPoint(x, y - 1)->getType() == P) {
					xOut = x;
					yOut = y - 1;
					return true;
				}
			}

			if (y == 4) {
				if (this->getTileAtPoint(x, y - 2)->getColor() == opp && this->getTileAtPoint(x, y - 2)->getType() == P) {
					xOut = x;
					yOut = y - 2;
					return true;
				}


			}
		}
	}

	return false;
}

bool Board::underAttack(Color opp, int x, int y)
{
	Color self = ((opp == WHT) ? BLK : WHT);

	for (int i = x + 1; i <= 8; i++) {

		//if blocked by own pieces or other pieces then break
		//horizontal
		if ((this->getTileAtPoint(i, y)->getColor() == self && !(this->isBlank(i, y)) || ((this->getTileAtPoint(i, y)->getType() != R && (this->getTileAtPoint(i, y)->getType() != Q && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0))))) {
			break;
		}
		if (this->getTileAtPoint(i, y)->getColor() == opp && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0  && (this->getTileAtPoint(i, y)->getType() == Q || this->getTileAtPoint(i, y)->getType() == R)) {
			return true;
		}

	}

	for (int i = x - 1; i >= 1; i--) {

		if ((this->getTileAtPoint(i, y)->getColor() == self && !(this->isBlank(i, y)) || ((this->getTileAtPoint(i, y)->getType() != R && this->getTileAtPoint(i, y)->getType() != Q && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0)))) {
			break;
		}
		if (this->getTileAtPoint(i, y)->getColor() == opp && this->getTileAtPoint(i, y)->toString().compare(str("Blank")) != 0  && (this->getTileAtPoint(i, y)->getType() == Q || this->getTileAtPoint(i, y)->getType() == R)) {
			return true;
		}

	}

	//vertical
	for (int i = y + 1; i <= 8; i++) {

		if ((this->getTileAtPoint(x, i)->getColor() == self && !(this->isBlank(x, i)) || ((this->getTileAtPoint(x, i)->getType() != R && (this->getTileAtPoint(x, i)->getType() != Q && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0))))) {
			break;
		}
		if (this->getTileAtPoint(x, i)->getColor() == opp && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x, i)->getType() == Q || this->getTileAtPoint(x, i)->getType() == R)) {
			return true;
		}

	}

	for (int i = y - 1; i >= 1; i--) {


		if ((this->getTileAtPoint(x, i)->getColor() == self && !(this->isBlank(x, i)) || ((this->getTileAtPoint(x, i)->getType() != R && !(this - isBlank(x, i))) && (this->getTileAtPoint(x, i)->getType() != Q)))) {
			//std::cout << ((this->getTileAtPoint(x, i)->getType() != R && !(this - isBlank(x, i))) || (this->getTileAtPoint(x, i)->getType() != Q && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0));
			break;
		}
		if (this->getTileAtPoint(x, i)->getColor() == opp && this->getTileAtPoint(x, i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x, i)->getType() == Q || this->getTileAtPoint(x, i)->getType() == R)) {
			return true;
		}

	}

	//diagonal 
	
	for (int i = 1; x + i <= 8 && y + i <= 8; i++) {

		if (this->getTileAtPoint(x + i, y + i)->getColor() == self && !(this->isBlank(x + i, y + i)) || ((this->getTileAtPoint(x + i, y + i)->getType() != Q && !(this->isBlank(x + i, y + i))) && (this->getTileAtPoint(x + i, y + i)->getType() != B && this->getTileAtPoint(x + i, y + i)->toString().compare(str("Blank")) != 0))) {
			break;
		}
		if (this->getTileAtPoint(x + i, y + i)->getColor() == opp && this->getTileAtPoint(x + i, y + i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x + i, y + i)->getType() == Q || this->getTileAtPoint(x + i, y + i)->getType() == B)) {
			return true;
		}
	}

	for (int i = 1; x + i <= 8 && y - i >= 1; i++) {

		if ((this->getTileAtPoint(x + i, y - i)->getColor() == self && !(this->isBlank(x + i, y - i)) || ((this->getTileAtPoint(x + i, y - i)->getType() != Q && !(this->isBlank(x + i, y - i))) && (this->getTileAtPoint(x + i, y - i)->getType() != B && this->getTileAtPoint(x + i, y - i)->toString().compare(str("Blank")) != 0)))) {
			break;
		}
		if (this->getTileAtPoint(x + i, y -i)->getColor() == opp && this->getTileAtPoint(x + i, y - i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x + i, y - i)->getType() == Q || this->getTileAtPoint(x + i, y - i)->getType() == B)) {
			return true;
		}
	}
	
	for (int i = 1; x - i >= 1 && y - i >= 1; i++) {
		

		if ((this->getTileAtPoint(x - i, y - i)->getColor() == self && !(this->isBlank(x - i, y - i)) || ((this->getTileAtPoint(x - i, y - i)->getType() != Q && !(this->isBlank(x - i, y - i))) && (this->getTileAtPoint(x - i, y - i)->getType() != B && this->getTileAtPoint(x - i, y - i)->toString().compare(str("Blank")) != 0)))) {
			break;
		}
		if (this->getTileAtPoint(x - i, y - i)->getColor() == opp && this->getTileAtPoint(x - i, y - i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x - i, y - i)->getType() == Q || this->getTileAtPoint(x - i, y - i)->getType() == B)) {
			return true;
		}
	}

	
	for (int i = 1; x - i >= 1 && y + i <= 8; i++) {

		if ((this->getTileAtPoint(x - i, y + i)->getColor() == self && !(this->isBlank(x - i, y + i)) || (this->getTileAtPoint(x - i, y + i)->getType() != Q && !(this->isBlank(x - i, y + i))) && (this->getTileAtPoint(x - i, y + i)->getType() != B && this->getTileAtPoint(x- + i, y + i)->toString().compare(str("Blank")) != 0))) {
			break;
		}
		if (this->getTileAtPoint(x - i, y + i)->getColor() == opp && this->getTileAtPoint(x - i, y + i)->toString().compare(str("Blank")) != 0 && (this->getTileAtPoint(x - i, y + i)->getType() == Q || this->getTileAtPoint(x - i, y + i)->getType() == B)) {
			return true;
		}
	}

	//knight
	
	int knightMoves[8][2] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
								   { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

	for (int i = 0; i <= 7; i++) {
		if (x + knightMoves[i][0] > 8 || y + knightMoves[i][1] > 8 || x + knightMoves[i][0] < 1 || y + knightMoves[i][1] < 1) {
			continue;
		}
		if (this->getTileAtPoint(x + knightMoves[i][0], y + knightMoves[i][1])->getColor() == opp && !(this->isBlank(x + knightMoves[i][0], y + knightMoves[i][1])) && this->getTileAtPoint(x + knightMoves[i][0], y + knightMoves[i][1])->getType() == Pieces::KNIGHT) {
			return true;
		}

	}

	//pawn

	if (opp == BLK) {
		if (x + 1 <= 8 && y + 1 <= 8) {
			if (this->getTileAtPoint(x + 1, y + 1)->getColor() == opp && this->getTileAtPoint(x + 1, y + 1)->getType() == P) {
				return true;
			}
		}

		if (x - 1 >= 1 && y + 1 <= 8) {
			if (this->getTileAtPoint(x - 1, y + 1)->getColor() == opp && this->getTileAtPoint(x - 1, y + 1)->getType() == P) {
				return true;
			}
		}
	}
	if (opp == WHT) {
		if (x + 1 <= 8 && y - 1 >= 1) {
			if (this->getTileAtPoint(x + 1, y - 1)->getColor() == opp && this->getTileAtPoint(x + 1, y - 1)->getType() == P) {
				return true;
			}
		}
		if (x - 1 >= 1 && y - 1 >= 1) {
			if (this->getTileAtPoint(x - 1, y - 1)->getColor() == opp && this->getTileAtPoint(x - 1, y - 1)->getType() == P) {
				return true;
			}
		}
	}

	return false;

}

void Board::undo()
{
	this->setTileAtPoint(this->prevMove.fromX, this->prevMove.fromY, this->getTileAtPoint(this->prevMove.toX, this->prevMove.toY));
	if (this->prevMove.pieceTaken != nullptr) {
		this->setTileAtPoint(this->prevMove.toX, this->prevMove.toY, this->prevMove.pieceTaken);
	}
	else {
		this->setTileAtPoint(this->prevMove.toX, this->prevMove.toY, new Tile(Pieces::BLANK, Color::WHITE));
		
	}
	this->bWhiteMove = !bWhiteMove;

}

inline bool Board::isBlank(int x, int y) {
	return this->getTileAtPoint(x, y)->toString().compare(str("Blank")) == 0;
}

void Board::debug()
{
	for (int x = 0; x < this->getBoard().size(); x++) {
		for (int j = 0; j < this->getBoard().size(); j++) {
			std::cout << this->getBoard()[x][j]->toString();
		}
		endl;
	}


	std::cin.ignore();
}

Board::PrevMove::PrevMove(int x, int y, int tx, int ty, Tile* piece)
{
	this->fromX = x;
	this->fromY = y;

	this->toX = tx;
	this->toY = ty;
	this->pieceTaken = piece;
}

Board::PrevMove::PrevMove()
{
	this->fromX = 0;
	this->toX = 0;
	this->toY = 0;
	this->fromY = 0;
	this->pieceTaken = nullptr;
}


