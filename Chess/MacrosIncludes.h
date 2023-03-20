#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <stdlib.h>

#define WHITE_SQUARE char(0xDB)
#define BLACK_SQUARE char(0xFF)

#define endl std::cout << std::endl;
#define wait std::cin.ignore();

#define Q Pieces::QUEEN
#define R Pieces::ROOK
#define B Pieces::BISHOP
#define H Pieces::KNIGHT
#define P Pieces::PAWN
#define K Pieces::KING

#define WHT Color::WHITE
#define BLK Color::BLACK

#define SIZE 8

typedef std::string str;

enum class Pieces {

	QUEEN = 8,
	ROOK = 5,
	BISHOP = 4,
	KNIGHT = 3,
	PAWN = 1,
	BLANK = 0,
	KING = 100

};

enum class Color {

	BLACK = 0,
	WHITE = 1

};

enum class Move {
	INVALID_MOVE,
	INVALID_PIECE,
	ARGUMENT_ERROR,
	UNDER_CHECK,
	VALID
};

