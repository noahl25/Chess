// Chess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once

#include <iostream>
#include <fstream>

#include "MacrosIncludes.h"
#include "Board.h"
#include "Tile.h"

Tile* strToTile(str convert) {

	if (convert.compare(str(" Blank")) == 0) {
		return new Tile();
	}

	if (convert[1] == 'R') {
		return new Tile(Pieces::ROOK, ((convert[2] == 'W') ? Color::WHITE : Color::BLACK));
	}

	if (convert[1] == 'H') {
		return new Tile(Pieces::KNIGHT, ((convert[2] == 'W') ? Color::WHITE : Color::BLACK));
	}

	if (convert[1] == 'B') {
		return new Tile(Pieces::BISHOP, ((convert[2] == 'W') ? Color::WHITE : Color::BLACK));
	}

	if (convert[1] == 'Q') {
		return new Tile(Pieces::QUEEN, ((convert[2] == 'W') ? Color::WHITE : Color::BLACK));
	}

	if (convert[1] == 'K') {
		return new Tile(Pieces::KING, ((convert[2] == 'W') ? Color::WHITE : Color::BLACK));
	}

	if (convert[1] == 'P') {
		return new Tile(Pieces::PAWN, ((convert[2] == 'W') ? Color::WHITE : Color::BLACK));
	}

	throw (0);
	//if here, something wrong w function!!!!!


}

void printInstructions() {
	system("cls");
	std::cout << "Hello!\n";
	std::cout << "To start a new game, type \'N\' into the command line!\n";
	std::cout << "Here is a list of other commands:\n";
	std::cout << " 'U'ndo undo the last move. (Doesn't work in title screen obviously!)\n";
	std::cout << " 'S'ave your current game. (Also doesn't work in title screen obviously!)\n";
	std::cout << " 'L'oad a previous game. Please make new game first before running this cmd.\n";
	std::cout << " 'Q'uit. Can be used to quit your current game or the entire application when in title screen.\n";
	std::cout << " 'I'nstructions. I think you know what this does.\n\n";
	std::cout << "When in a game, it will prompt you for a piece to move then where to move it. The coordinates are in the format:\n";
	std::cout << "A1, B3, H7, etc.\n";
	std::cout << "You wont be able to do illegal moves, so don't try to!\n";
	std::cout << "To castle, move your king to the G or C file.\n";
	std::cout << "Instead of typing in a piece to move, you may also type in a command.\n\n";
	std::cout << "Thats it! Have fun!\n";
	std::cout << "Press 'Enter' to go back.";
}

//make if nullptr check!!!!!!!!!!!!!!!!!!! im sleep now
Board* loadGame(int& move) {

	str fileName;
	std::cout << "Type file name to be loaded (no extension): ";
	std::getline(std::cin, fileName);

	std::ifstream file(fileName + ".txt");

	Board* board = new Board();

	if (file.is_open()) {

		str line;
		
		try {

			int row = 8;

			for (int j = 0; j < 8; j++) {

				std::getline(file, line);

				int col = 1;

				int prev = 0;
				str parsed;

				for (int i = 0; i < 8; i++) {

					int fst = line.find(" ", prev);
					int scnd = line.find(" ", fst + 1);

					int dist = scnd - fst;
					parsed = line.substr(fst, dist);
					prev = scnd;

					Tile* tile = strToTile(parsed);
					board->setTileAtPoint(col, row, tile);
					col++;
				}

				row--;


			
				endl;
			}

			str moveData;
			std::getline(file, moveData);

			move = (moveData[2] - '0');
			board->setMove(((moveData[1] == 'W') ? true : false));

			std::getline(file, moveData);
			board->WhiteKing.x = moveData[0] - '0';
			board->WhiteKing.y = moveData[1] - '0';

			std::getline(file, moveData);
			board->BlackKing.x = moveData[0] - '0';
			board->BlackKing.y = moveData[1] - '0';

			return board;
		}

		catch (...) {
			return nullptr;
		}

	}

	return nullptr;

}

bool saveState(Board* board, int move)  {

	str fileName;
	std::cout << "Type file name to be saved (no extension): ";
	std::getline(std::cin, fileName);
	
	std::ofstream file(fileName + ".txt");

	if (file.is_open()) {
		
		file << " ";

		for (int i = 0; i < board->getBoard().size(); i++) {
			for (int j = 0; j < board->getBoard().size(); j++) {

				file << board->getBoard()[j][i]->toString() + " ";
			}

			file << "\n";
			file << " ";
		}

		file << ((board->getMove()) ? "W" : "B");
		file << move;
		file << "\n";
		file << board->getKingLocation(WHT).x << board->getKingLocation(WHT).y;
		file << "\n";
		file << board->getKingLocation(BLK).x << board->getKingLocation(BLK).y;



		file.close();
		return true;
	}

	return false;

}

void printLogo()
{
	std::cout << "    ======================================\n";
	std::cout << "       _____ _    _ ______  _____ _____\n";
	std::cout << "      / ____| |  | |  ____|/ ____/ ____|\n";
	std::cout << "     | |    | |__| | |__  | (___| (___ \n";
	std::cout << "     | |    |  __  |  __|  \\___ \\\\___ \\ \n";
	std::cout << "     | |____| |  | | |____ ____) |___) |\n";
	std::cout << "      \\_____|_|  |_|______|_____/_____/\n\n";
	std::cout << "    ======================================\n";
	std::cout << "------------------Scroll down!-------------------\n\n";
}

char switchColor(char w) {

	if (w == WHITE_SQUARE) {
		return BLACK_SQUARE;
	}

	return WHITE_SQUARE;

}

void drawLine(int tileWidth, bool bWhite, Board* board, int rowLocal, int row) {
	bool color = bWhite;
	for (int z = 0; z < SIZE; z++) {
		if (color) {
			if (rowLocal == 1) {
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
				if (board->getBoard()[z][row]->toString().compare(str("Blank")) == 0) {
					std::cout << WHITE_SQUARE;
					std::cout << WHITE_SQUARE;
				}
				else {
					std::cout << board->getBoard()[z][row]->toString();
				}
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
			}
			else {
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
				std::cout << WHITE_SQUARE;
			}
		}
		else {
			if (rowLocal == 1) {
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
				if (board->getBoard()[z][row]->toString().compare(str("Blank")) == 0) {
					std::cout << BLACK_SQUARE;
					std::cout << BLACK_SQUARE;
				}
				else {
					std::cout << board->getBoard()[z][row]->toString();
				}
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
			}

			else {
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
				std::cout << BLACK_SQUARE;
			}
		}
		color = !color;
	}
}

void drawBoard(Board* board) {

	bool bSwitch = true;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < board->boardWidth; j++) {
			if (j == 1) {
				std::cout << SIZE - i; std::cout << " ";
				drawLine(board->boardWidth, bSwitch, board, j, i);
			}
			else {
				std::cout << "  ";
				drawLine(board->boardWidth, bSwitch, board, j, i);
			}
			endl;
		}
		bSwitch = !bSwitch;
	}
	std::cout << "  ";
	for (int i = 65; i < 65 + SIZE; i++) {
		std::cout << "  ";
		std::cout << char(i);
		std::cout << "   ";
	}
	endl;
	endl;
}


bool stillUnderCheck(Board board, str from, str to, Color opp, Color self)
{
	board.makeMove(from, to, false);

	if (board.underAttack(opp, board.getKingLocation(self).x, board.getKingLocation(self).y)) {
		return true;
	}

	return false;

}

bool isCheckmate(Board board) {

	Color self = ((board.getMove()) ? WHT : BLK);
	Color opp = ((board.getMove()) ? BLK : WHT);

	int kingX = board.getKingLocation(self).x;
	int kingY = board.getKingLocation(self).y;

	int possibleMoves[8][2] = { {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1} };

	int xAttacker;
	int yAttacker;


	//under check and not able to be taken
	if (board.underAttack(opp, kingX, kingY, xAttacker, yAttacker) && !board.underAttack(self, xAttacker, yAttacker) ) {


		//if can move not checkmate
		for (int i = 0; i <= 7; i++) {

			if (kingX + possibleMoves[i][0] <= 8 && kingX + possibleMoves[i][0] >= 1 && kingY + possibleMoves[i][1] <= 8 && kingY + possibleMoves[i][1] >= 1) {
				if (!board.underAttack(opp, kingX + possibleMoves[i][0], kingY + possibleMoves[i][1]) && board.getTileAtPoint(kingX + possibleMoves[i][0], kingY + possibleMoves[i][1])->getColor() != self) {
					return false;
				}
			}
		}

		return true;


	}





	return false;

}

int main() {

	str trash;
	str errorone = "";
	
	while (true) {

		char command;
		printLogo();
		//temp
		Board* temp = new Board();
		drawBoard(temp);
		delete temp;
		//game board
		Board* board = new Board();

		std::cout << errorone;
		std::cout << "Commands: (N)ew Game, (U)ndo, (S)ave Game, (L)oad Game, \n(Q)uit, (I)nstructions"; endl;
		std::cout << "Cmd: ";
		std::cin >> command;

		if (command == 'Q') {
			system("cls");
			endl;
			std::cout << "--Thanks for playing!--"; endl;
			exit(0);
		}
		if (command == 'I') { 
			printInstructions();
			std::getline(std::cin, trash);
			trash.clear();
			errorone = "";
			wait;
			system("cls");
			continue;
		}
		if (command != 'N') {
			errorone = "Invalid command!\n";
			system("cls");
			std::getline(std::cin, trash);
			trash.clear();
			continue;
		}

		errorone.clear();

		std::getline(std::cin, trash);
		trash.clear();

		str error = "";
		str whiteMove = "";
		str blackMove = "";

		int move = 1;
		bool saved = false;

		//main game loop
		while (true) {

			system("cls");
			endl;
			drawBoard(board);
			if (isCheckmate(*board)) {
				endl;
				std::cout << "CHECKMATE!";
				endl; endl;
				std::cout << ((board->getMove()) ? str("Black") : str("White")) + str(" Wins!");
				endl;
				std::cout << "Press enter to exit to menu.";
				wait;
				break;

			}
			std::cout << error;
			str moveFrom;
			str moveTo;
			std::cout << str("Current turn: ") + ((board->getMove()) ? str("WHITE.") : str("BLACK.")); endl; endl;
			std::cout << "----------------------------------------\n";
			std::cout << "Previous moves:\n";
			std::cout << "White: " + whiteMove; endl;
			std::cout << "Black: " + blackMove; endl;
			std::cout << "----------------------------------------"; endl; endl;
			std::cout << "Piece? (or cmd) ";
			std::getline(std::cin, moveFrom);

			if (moveFrom.length() == 1 && moveFrom[0] > 64 && moveFrom[0] < 91) {
				if (moveFrom.compare(str("U")) == 0 && move != 1) {
					board->undo();
					error = "";
					move--;
					continue;
				}
				if (DEBUG) {
					if (moveFrom.compare(str("E")) == 0 && move != 1) {
						error = "";
						board->setMove(!board->getMove());
						continue;
					}
				}
				if (moveFrom.compare(str("U")) == 0 && move == 1) {
					error = "Cannot undo!\n";
					continue;
				}
				if (moveFrom.compare(str("L")) == 0) {

					Board* temp = loadGame(move);

					if (temp == nullptr) {
						error = "Could not load!\n";
						continue;
					}

					whiteMove = "";
					blackMove = "";
					move = 1;
					board = temp;
					
					continue;
				}
				if (moveFrom.compare(str("I")) == 0) {
					printInstructions();
					std::getline(std::cin, trash);
					trash.clear();
					system("cls");
					continue;
				}
				if (moveFrom.compare(str("Q")) == 0) {
					system("cls");
					if (!saved && move != 1) {
						std::cout << "Game not saved! Would you like to save? (Y, N) ";
						str ans; 
						std::getline(std::cin, ans);
						if (ans.compare("Y") == 0 || ans.compare("y") == 0) {
							saveState(board, move);
						}

					}
					break;
				}

				if (moveFrom.compare(str("S")) == 0) {
					if (!saveState(board, move)) {
						error = "Could not save!\n";
					}
					else {
						error = "Saved!\n";
						saved = true;
					}
					continue;
				}
				else {
					error = "Invalid command.\n";
					continue;
				}
			}
			std::cout << "Where? ";
			std::getline(std::cin, moveTo);

			Color opp = board->getMove() ? BLK : WHT;
			Color self = board->getMove() ? WHT : BLK;

			bool underCheck = board->underAttack(opp, board->getKingLocation(self).x, board->getKingLocation(self).y);

			try {
		
				if (stillUnderCheck(*board, moveFrom, moveTo, opp, self)) {
					error = "Under check. Invalid move. \n";
					continue;
				}
				
				board->makeMove(moveFrom, moveTo, board->underAttack(opp, board->getKingLocation(self).x, board->getKingLocation(self).y));
			}

			catch (Move move) {
				if (move == Move::INVALID_MOVE) {
					error = "Invalid move.\n";
				}
				if (move == Move::INVALID_PIECE) {
					error = "Invalid Piece.\n";
				}
				if (move == Move::ARGUMENT_ERROR) {
					error = "Please type in coords in the form: A1, E5, H7, etc. \n--Make sure from and to are not the same--\n";
				}
				continue;
			}

			if (!board->getMove()) {
				whiteMove = board->getTileAtPoint(int(moveTo[0] - 64), int(moveTo[1]) - '0')->toString() + ", " + moveFrom + "-" + moveTo;
			}
			else {
				blackMove = board->getTileAtPoint(int(moveTo[0] - 64), int(moveTo[1]) - '0')->toString() + ", " + moveFrom + "-" + moveTo;
			}

			move++;
			error = "";

		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
