#pragma once

#include "MacrosIncludes.h";


class Tile {

private:
	
	Pieces piece;
	Color color;

public:

	Tile(Pieces piece = Pieces::BLANK, Color color = Color::WHITE);
	bool changeTile(Pieces piece);
	Pieces getType();
	std::string toString();
	Color getColor();
};