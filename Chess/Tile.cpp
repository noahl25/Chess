#pragma once

#include "Tile.h"

Tile::Tile(Pieces piece, Color color) {
	
	this->piece = piece;
	this->color = color;

}

bool Tile::changeTile(Pieces piece) {
	
	this->piece = piece;
	return true;

}

std::string Tile::toString() {
	
	switch (this->piece) {

	case Pieces::QUEEN:
		return str("Q") + ((this->color == WHT) ? str("W") : str("B"));
	case Pieces::BISHOP:
		return str("B") + ((this->color == WHT) ? str("W") : str("B"));
	case Pieces::KNIGHT:
		return str("H") + ((this->color == WHT) ? str("W") : str("B"));
	case Pieces::ROOK:
		return str("R") + ((this->color == WHT) ? str("W") : str("B"));
	case Pieces::PAWN:
		return str("P") + ((this->color == WHT) ? str("W") : str("B"));
	case Pieces::BLANK:
		return str("Blank");
	case Pieces::KING:
		return str("K") + ((this->color == WHT) ? str("W") : str("B"));
	default:
		return std::string("null");


	}

}

Pieces Tile::getType()
{
	return this->piece;
}

Color Tile::getColor()
{
	return this->color;
}


