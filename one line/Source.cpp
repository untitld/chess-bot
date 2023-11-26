#include <iostream>
#include <vector>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fstream>


using namespace std;

void initBoard();
void printBoard();
void findMoves(int x, int y, int piece);
void Color(int color);
int getX(int coord);
int getY(int coord);
int coordToPiece(int coord);
void cs();
int takeInput(char input);
int checkPiece(bool top, int coords);
int checkPieceMove(int coords, int origCoords, int piece, bool top, bool move);
bool checkForCheck(bool top);
int pawnRetake(bool top);
bool checkForMate(bool top);

bool selectPiece = false;
int choosePiece = 2;
int pPos;
int chessboard[8][8];
vector <int> movesList;
vector <int> movePieces;

bool bot = true;

int main() {

	ofstream dumpData;
	ifstream takeData;

	srand(time(0));

	int botPieceMove;
	bool loop = true;
	const int nodesColumn = 100;
	vector <int> genome;

	int nodeSet0[nodesColumn];
	int nodeSet1[nodesColumn];
	int nodeSet2[nodesColumn];
	int nodeSet3[nodesColumn];
	int nodeSet4[nodesColumn];

	initBoard();

	int moveDecision;
	bool p1 = true;
	char input = 'l';
	char gofuckurself;
	bool moveBeingMade = true;
	if (bot) {
		while (true) {
			while (moveBeingMade) {
				if (checkForCheck(p1)) {
					if (checkForMate(p1)) {
						printBoard();
						cout << "checkmate" << endl;
						cin >> input;
					}
				}
				for (int y = 0; y < 8; y++) {
					for (int x = 0; x < 8; x++) {
						if (checkPiece(p1, (y * 100 + x)) == 2) {
							movePieces.push_back(y * 100 + x);
						}
					}
				}
				choosePiece = movePieces[rand() % movePieces.size()];
				findMoves(getX(choosePiece), getY(choosePiece), coordToPiece(choosePiece));
				botPieceMove = movesList[rand() % movesList.size()];
				if (checkPieceMove(botPieceMove, choosePiece, coordToPiece(choosePiece), p1, true) == 1) {
					moveBeingMade = false;
				}
				while (movePieces.size() > 0) {
					movePieces.pop_back();
				}
			}
			moveBeingMade = true;
			if (p1) {
				p1 = false;
			}
			else {
				p1 = true;
			}
			while (movesList.size() > 0) {
				movesList.pop_back();
			}
		}
	}
	else {
		while (loop) {
			input = 'l';
			while (moveBeingMade) {
				while (input != 'p') { 
					cs();
					printBoard();
					if (p1) {
						cout << "player 1 turn" << endl;
					}
					else {
						cout << "player 2 turn" << endl;
					}
					if (checkForCheck(p1)) {
						if (checkForMate(p1)) {
							cout << "checkmate" << endl;
							cin >> input;
						}
						cout << "YOU ARE IN CHECK" << endl;
					}
					else {
					}
					cout << "use wasd to move around selection and hit 'p' to select" << endl;
					input = _getch();
					if (takeInput(input) == 1) {
						switch (checkPiece(p1, pPos)) {
						case 0:
							cout << "thats the other players piece!!" << endl;
							input = 'l';
							gofuckurself = _getch();
							break;
						case 1:
							cout << "this piece has no moves!" << endl;
							input = 'l';
							gofuckurself = _getch();
							break;
						case 2:
							choosePiece = pPos;
							break;
						case 3:
							cout << "thats not even a chess piece!" << endl;
							input = 'l';
							gofuckurself = _getch();
							break;
						}
					}
				}
				input = 'l';
				findMoves(getX(choosePiece), getY(choosePiece), coordToPiece(choosePiece));
				while (input != 'p') {
					selectPiece = true;
					cs();
					printBoard();
					cout << "select where you want to move the piece to" << endl;
					input = _getch();
					if (takeInput(input) == 1) {
						switch (checkPieceMove(pPos, choosePiece, coordToPiece(choosePiece), p1, true)) {
						case 0:
							cout << "not a valid move!" << endl;
							gofuckurself = _getch();
							break;
						case 1:
							moveBeingMade = false;
							break;
						case 2:
							cout << "you cant move your king into check!" << endl;
							gofuckurself = _getch();
						}
					}
				}
				selectPiece = false;
				input = 'l';
			}
			if (p1) {
				p1 = false;
			}
			else {
				p1 = true;
			}
			moveBeingMade = true;
			while (movesList.size() > 0) {
				movesList.pop_back();
			}
		}
	}

	return 0;
}

int checkPieceMove(int coords, int origCoords, int piece, bool top, bool move){
	int tempPiece;
	for (int i = 0; i < movesList.size(); i++) {
		if (coords == movesList[i]) {
			tempPiece = chessboard[getY(coords)][getX(coords)];
			chessboard[getY(coords)][getX(coords)] = piece;
			chessboard[getY(origCoords)][getX(origCoords)] = 0;
			if (checkForCheck(top)) {
				chessboard[getY(coords)][getX(coords)] = tempPiece;
				chessboard[getY(origCoords)][getX(origCoords)] = piece;
				return 2;
			}
			else {
				if (!move) {
					cout << "no move" << endl;
					chessboard[getY(coords)][getX(coords)] = tempPiece;
					chessboard[getY(origCoords)][getX(origCoords)] = piece;
				}
				else {
					if (piece == 1 || piece == 11) {
						if (getY(coords) == 0 || getY(coords) == 7) {
							chessboard[getY(coords)][getX(coords)] = pawnRetake(top);
						}
					}
				}
				return 1;
			}
		}
	}
	return 0;
}

int pawnRetake(bool top) {
	if (bot) {
		return (rand() % 4) + 2;
	}
	else {
		cout << "promote pawn to what piece" << endl << endl;
		cout << "(2) rook" << endl;
		cout << "(3) knight" << endl;
		cout << "(4) bishop" << endl;
		cout << "(5) queen" << endl;
		while (true) {
			char input = _getch();
			if (input == '2') {
				return 2;
			}
			if (input == '3') {
				return 3;
			}
			if (input == '4') {
				return 4;
			}
			if (input == '5') {
				return 5;
			}
		}
	}

}

int checkPiece(bool top, int coords) {
	if (top) {
		if (chessboard[getY(coords)][getX(coords)] > 10) {
			return 0;
		}
		else if (chessboard[getY(coords)][getX(coords)] == 0) {
			return 3;
		}
		else {
			findMoves(getX(coords), getY(coords), coordToPiece(coords));
			if (movesList.size() == 0) {
				return 1;
			}
			return 2;
		}

	}
	else {
		if (chessboard[getY(coords)][getX(coords)] < 10 && chessboard[getY(coords)][getX(coords)] != 0) {
			return 0;
		}
		else if (chessboard[getY(coords)][getX(coords)] == 0) {
			return 3;
		}
		else {
			findMoves(getX(coords), getY(coords), coordToPiece(coords));
			if (movesList.size() == 0) {
				return 1;
			}
			return 2;
		}
	}
}

int takeInput(char input) {
	int pPosy = (pPos - (pPos % 100)) / 100;
	int pPosx = pPos % 100;
	if (input == 'w') {
		if (pPosy - 1 >= 0) {
			pPos -= 100;
		}
	}
	if (input == 's') {
		if (pPosy + 1 < 8) {
			pPos += 100;
		}
	}
	if (input == 'a') {
		if (pPosx - 1 >= 0) {
			pPos -= 1;
		}
	}
	if (input == 'd') {
		if (pPosx + 1 < 8) {
			pPos += 1;
		}
	}
	if (input == 'p') {
		return 1;
	}
	return 0;
}

void Color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int coordToPiece(int coord) {
	return chessboard[getY(coord)][getX(coord)];
}

void cs() {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
}

//to grab the x coord from regular coord and likewise y below it
int getX(int coord) {
	return coord % 100;
}

int getY(int coord) {
	return (coord - (coord % 100)) / 100;
}

void printBoard() {
	cout << "    12345678" << endl << endl;
	for (int y = 0; y < 8; y++) {
		cout << y + 1 << "   ";
		for (int x = 0; x < 8; x++) {
			for (int i = 0; i < movesList.size(); i++) {
				if ((y * 100 + x) == movesList[i]) {
					Color(4);
				}
			}
			if (selectPiece) {
				if ((y * 100 + x) == choosePiece) {
					Color(14);
				}
			}
			if ((y * 100 + x) == pPos) {
				Color(2);
			}
			if (chessboard[y][x] > 10) {
				Color(8);
				for (int i = 0; i < movesList.size(); i++) {
					if ((y * 100 + x) == movesList[i]) {
						Color(4);
					}
				}
				if (selectPiece) {
					if ((y * 100 + x) == choosePiece) {
						Color(14);
					}
				}
				if ((y * 100 + x) == pPos) {
					Color(2);
				}
				cout << chessboard[y][x] - 10;
			}
			else if (chessboard[y][x] == 0){
				cout << ".";
			}
			else {
				cout << chessboard[y][x];
			}
			if (x == 7) {
				cout << endl;
			}
			Color(7);
		}
	}
	cout << endl;
}

 void initBoard() {
	/*
	0: empty space
	1: pawn
	2: rook
	3: knight
	4: bishop
	5: queen
	6: king
	having a 1 in front means its the lower half of the baord
	*/
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			switch (y) {
			case 0:
				switch (x) {
				case 0:
					chessboard[y][x] = 2;
					break;
				case 1:
					chessboard[y][x] = 3;
					break;
				case 2:
					chessboard[y][x] = 4;
					break;
				case 3:
					chessboard[y][x] = 5;
					break;
				case 4:
					chessboard[y][x] = 6;
					break;
				case 5:
					chessboard[y][x] = 4;
					break;
				case 6:
					chessboard[y][x] = 3;
					break;
				case 7:
					chessboard[y][x] = 2;
					break;
				}
				break;
			case 1:
				chessboard[y][x] = 1;
				break;
			case 2:
				chessboard[y][x] = 0;
				break;
			case 3:
				chessboard[y][x] = 0;
				break;
			case 4:
				chessboard[y][x] = 0;
				break;
			case 5:
				chessboard[y][x] = 0;
				break;
			case 6:
				chessboard[y][x] = 11;
				break;
			case 7:
				switch (x) {
				case 0:
					chessboard[y][x] = 12;
					break;
				case 1:
					chessboard[y][x] = 13;
					break;
				case 2:
					chessboard[y][x] = 14;
					break;
				case 3:
					chessboard[y][x] = 15;
					break;
				case 4:
					chessboard[y][x] = 16;
					break;
				case 5:
					chessboard[y][x] = 14;
					break;
				case 6:
					chessboard[y][x] = 13;
					break;
				case 7:
					chessboard[y][x] = 12;
					break;
				}
				break;
			}
		}
	}
}

 void findMoves(int x, int y, int piece) {
	/*
	0: empty space
	1: pawn
	2: rook
	3: knight
	4: bishop
	5: queen
	6: king
	having a 1 in front means its the lower half of the baord
	*/
	 int obj;
	 bool clearPath = true;
	 while (movesList.size() > 0) {
		 movesList.pop_back();
	 }
	 bool top = true;
	 if (piece == 100) {
		 piece -= 91;
	 }
	 else {
		 if (piece > 10) {
			 top = false;
			 piece -= 10;
		 }
	 }
	 switch (piece) {
	 case 1:
		 //forward/backwards
		 if (top) {
			 if (chessboard[y + 1][x] == 0) {
				 movesList.push_back((y + 1) * 100 + x);
			 }
			 if (y == 1) {
				 if (chessboard[y + 2][x] == 0) {
					 movesList.push_back((y + 2) * 100 + x);
				 }
			 }
		 }
		 else {
			 if (chessboard[y - 1][x] == 0) {
				 movesList.push_back((y - 1) * 100 + x);
			 }
			 if (y == 6) {
				 if (chessboard[y - 2][x] == 0) {
					 movesList.push_back((y - 2) * 100 + x);
				 }
			 }
		 }
		 //upleft
		 if (x - 1 >= 0 && y - 1 >= 0) {
			 if (!top){
				 if (chessboard[y - 1][x - 1] < 10 && chessboard[y - 1][x - 1] != 0) {
					 movesList.push_back((y - 1) * 100 + (x - 1));
				 }
			 }
		 }
		 //upright
		 if (x + 1 < 8 && y - 1 >= 0) {
			 if (!top){
				 if (chessboard[y - 1][x + 1] < 10 && chessboard[y - 1][x + 1] != 0) {
					 movesList.push_back((y - 1) * 100 + (x + 1));
				 }
			 }
		 }
		 //downleft
		 if (x - 1 >= 0 && y + 1 < 8) {
			 if (top) {
				 if (chessboard[y + 1][x - 1] > 10) {
					 movesList.push_back((y + 1) * 100 + (x - 1));
				 }
			 }
		 }
		 //downright
		 if (x + 1 < 8 && y + 1 < 8) {
			 if (top) {
				 if (chessboard[y + 1][x + 1] > 10) {
					 movesList.push_back((y + 1) * 100 + (x + 1));
				 }
			 }
		 }
		 break;
	 case 2:
		 //up
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (y - i >= 0) {
					 obj = chessboard[y - i][x];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back((y - i) * 100 + x);
						 }
						 if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back((y - i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back((y - i) * 100 + x);
						 }
						 if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back((y - i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //down
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (y + i < 8) {
					 obj = chessboard[y + i][x];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back((y + i) * 100 + x);
						 }
						 if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back((y + i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back((y + i) * 100 + x);
						 }
						 if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back((y + i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //left
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (x - i >= 0) {
					 obj = chessboard[y][x - i];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x - i));
						 }
						 if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back(y * 100 + (x - i));
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x - i));
						 }
						 if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back(y * 100 + (x - i));
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //right
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (x + i < 8) {
					 obj = chessboard[y][x + i];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x + i));
						 }
						 if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back(y * 100 + (x + i));
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x + i));
						 }
						 if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back(y * 100 + (x + i));
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 break;
	 case 3:
		 //up
		 if (y - 2 >= 0 && x + 1 < 8 && x - 1 >= 0) {
			 if (top) {
				 if (chessboard[y - 2][x + 1] == 0 || chessboard[y - 2][x + 1] > 10) {
					 movesList.push_back((y - 2) * 100 + (x + 1));
				 }
				 if (chessboard[y - 2][x - 1] == 0 || chessboard[y - 2][x - 1] > 10) {
					 movesList.push_back((y - 2) * 100 + (x - 1));
				 }
			 }
			 else {
				 if (chessboard[y - 2][x + 1] < 10) {
					 movesList.push_back((y - 2) * 100 + (x + 1));
				 }
				 if (chessboard[y - 2][x - 1] < 10) {
					 movesList.push_back((y - 2) * 100 + (x - 1));
				 }
			 }
		 }
		 //down
		 if (y + 2 < 8 && x + 1 < 8 && x - 1 >= 0) {
			 if (top) {
				 if (chessboard[y + 2][x + 1] == 0 || chessboard[y + 2][x + 1] > 10) {
					 movesList.push_back((y + 2) * 100 + (x + 1));
				 }
				 if (chessboard[y + 2][x - 1] == 0 || chessboard[y + 2][x - 1] > 10) {
					 movesList.push_back((y + 2) * 100 + (x - 1));
				 }
			 }
			 else {
				 if (chessboard[y + 2][x + 1] < 10) {
					 movesList.push_back((y + 2) * 100 + (x + 1));
				 }
				 if (chessboard[y + 2][x - 1] < 10) {
					 movesList.push_back((y + 2) * 100 + (x - 1));
				 }
			 }
		 }
		 //left
		 if (x - 2 >= 0 && y + 1 < 8 && y - 1 >= 0) {
			 if (top) {
				 if (chessboard[y - 1][x - 2] == 0 || chessboard[y - 1][x - 2] > 10) {
					 movesList.push_back((y - 1) * 100 + (x - 2));
				 }
				 if (chessboard[y + 1][x - 2] == 0 || chessboard[y + 1][x - 2] > 10) {
					 movesList.push_back((y + 1) * 100 + (x - 2));
				 }
			 }
			 else {
				 if (chessboard[y - 1][x - 2] < 10) {
					 movesList.push_back((y - 1) * 100 + (x - 2));
				 }
				 if (chessboard[y + 1][x - 2] < 10) {
					 movesList.push_back((y + 1) * 100 + (x - 2));
				 }
			 }
		 }
		 //right
		 if (x + 2 < 8 && y + 1 < 8 && y - 1 >= 0) {
			 if (top) {
				 if (chessboard[y - 1][x + 2] == 0 || chessboard[y - 1][x + 2] > 10) {
					 movesList.push_back((y - 1) * 100 + (x + 2));
				 }
				 if (chessboard[y + 1][x + 2] == 0 || chessboard[y + 1][x + 2] > 10) {
					 movesList.push_back((y + 1) * 100 + (x + 2));
				 }
			 }
			 else {
				 if (chessboard[y - 1][x + 2] < 10) {
					 movesList.push_back((y - 1) * 100 + (x + 2));
				 }
				 if (chessboard[y + 1][x + 2] < 10) {
					 movesList.push_back((y + 1) * 100 + (x + 2));
				 }
			 }
		 }
		 break;
	 case 4:
		 //upleft
		 for (int i = 1; i < 8; i++) {
			 if (x - i >= 0 && y - i >= 0) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y - i][x - i] == 0) {
							 movesList.push_back((y - i) * 100 + (x - i));
						 }
						 else if (chessboard[y - i][x - i] > 10) {
							 movesList.push_back((y - i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y - i][x - i] == 0) {
							 movesList.push_back((y - i) * 100 + (x - i));
						 }
						 else if (chessboard[y - i][x - i] < 10) {
							 movesList.push_back((y - i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //upright
		 for (int i = 1; i < 8; i++) {
			 if (x + i < 8 && y - i >= 0) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y - i][x + i] == 0) {
							 movesList.push_back((y - i) * 100 + (x + i));
						 }
						 else if (chessboard[y - i][x + i] > 10) {
							 movesList.push_back((y - i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y - i][x + i] == 0) {
							 movesList.push_back((y - i) * 100 + (x + i));
						 }
						 else if (chessboard[y - i][x + i] < 10) {
							 movesList.push_back((y - i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //downleft
		 for (int i = 1; i < 8; i++) {
			 if (x - i >= 0 && y + i < 8) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y + i][x - i] == 0) {
							 movesList.push_back((y + i) * 100 + (x - i));
						 }
						 else if (chessboard[y + i][x - i] > 10) {
							 movesList.push_back((y + i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y + i][x - i] == 0) {
							 movesList.push_back((y + i) * 100 + (x - i));
						 }
						 else if (chessboard[y + i][x - i] < 10) {
							 movesList.push_back((y + i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //downright
		 for (int i = 1; i < 8; i++) {
			 if (x + i < 8 && y + i < 8) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y + i][x + i] == 0) {
							 movesList.push_back((y + i) * 100 + (x + i));
						 }
						 else if (chessboard[y + i][x + i] > 10) {
							 movesList.push_back((y + i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y + i][x + i] == 0) {
							 movesList.push_back((y + i) * 100 + (x + i));
						 }
						 else if (chessboard[y + i][x + i] < 10) {
							 movesList.push_back((y + i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 break;
	 case 5:
		 //up
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (y - i >= 0) {
					 obj = chessboard[y - i][x];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back((y - i) * 100 + x);
						 } else if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back((y - i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back((y - i) * 100 + x);
						 } else if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back((y - i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //down
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (y + i < 8) {
					 obj = chessboard[y + i][x];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back((y + i) * 100 + x);
						 } else if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back((y + i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back((y + i) * 100 + x);
						 } else if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back((y + i) * 100 + x);
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //left
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (x - i >= 0) {
					 obj = chessboard[y][x - i];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x - i));
						 } else if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back(y * 100 + (x - i));
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x - i));
						 } else if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back(y * 100 + (x - i));
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //right
		 for (int i = 1; i < 8; i++) {
			 if (clearPath) {
				 if (x + i < 8) {
					 obj = chessboard[y][x + i];
					 //top
					 if (top) {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x + i));
						 } else if (obj > 0) {
							 if (obj > 10) {
								 movesList.push_back(y * 100 + (x + i));
							 }
							 clearPath = false;
						 }
					 }
					 //bottom
					 else {
						 if (obj == 0) {
							 movesList.push_back(y * 100 + (x + i));
						 } else if (obj > 0) {
							 if (obj < 10) {
								 movesList.push_back(y * 100 + (x + i));
							 }
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //upleft
		 for (int i = 1; i < 8; i++) {
			 if (x - i >= 0 && y - i >= 0) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y - i][x - i] == 0) {
							 movesList.push_back((y - i) * 100 + (x - i));
						 }
						 else if (chessboard[y - i][x - i] > 10) {
							 movesList.push_back((y - i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y - i][x - i] == 0) {
							 movesList.push_back((y - i) * 100 + (x - i));
						 }
						 else if (chessboard[y - i][x - i] < 10) {
							 movesList.push_back((y - i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //upright
		 for (int i = 1; i < 8; i++) {
			 if (x + i < 8 && y - i >= 0) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y - i][x + i] == 0) {
							 movesList.push_back((y - i) * 100 + (x + i));
						 }
						 else if (chessboard[y - i][x + i] > 10) {
							 movesList.push_back((y - i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y - i][x + i] == 0) {
							 movesList.push_back((y - i) * 100 + (x + i));
						 }
						 else if (chessboard[y - i][x + i] < 10) {
							 movesList.push_back((y - i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //downleft
		 for (int i = 1; i < 8; i++) {
			 if (x - i >= 0 && y + i < 8) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y + i][x - i] == 0) {
							 movesList.push_back((y + i) * 100 + (x - i));
						 }
						 else if (chessboard[y + i][x - i] > 10) {
							 movesList.push_back((y + i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y + i][x - i] == 0) {
							 movesList.push_back((y + i) * 100 + (x - i));
						 }
						 else if (chessboard[y + i][x - i] < 10) {
							 movesList.push_back((y + i) * 100 + (x - i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 //downright
		 for (int i = 1; i < 8; i++) {
			 if (x + i < 8 && y + i < 8) {
				 if (clearPath) {
					 if (top) {
						 if (chessboard[y + i][x + i] == 0) {
							 movesList.push_back((y + i) * 100 + (x + i));
						 }
						 else if (chessboard[y + i][x + i] > 10) {
							 movesList.push_back((y + i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
					 else {
						 if (chessboard[y + i][x + i] == 0) {
							 movesList.push_back((y + i) * 100 + (x + i));
						 }
						 else if (chessboard[y + i][x + i] < 10) {
							 movesList.push_back((y + i) * 100 + (x + i));
							 clearPath = false;
						 }
						 else {
							 clearPath = false;
						 }
					 }
				 }
			 }
		 }
		 clearPath = true;
		 break;
	 case 6:
		 //up
		 if (y - 1 >= 0) {
			 if (top) {
				 if (chessboard[y - 1][x] == 0 || chessboard[y - 1][x] > 10) {
					 movesList.push_back((y - 1) * 100 + x);
				 }
			 }
			 else {
				 if (chessboard[y - 1][x] < 10) {
					 movesList.push_back((y - 1) * 100 + x);
				 }
			 }
		 }
		 //down
		 if (y + 1 < 8) {
			 if (top) {
				 if (chessboard[y + 1][x] == 0 || chessboard[y + 1][x] > 10) {
					 movesList.push_back((y + 1) * 100 + x);
				 }
			 }
			 else {
				 if (chessboard[y + 1][x] < 10) {
					 movesList.push_back((y + 1) * 100 + x);
				 }
			 }
		 }
		 //left
		 if (x - 1 >= 0) {
			 if (top) {
				 if (chessboard[y][x - 1] == 0 || chessboard[y][x - 1] > 10) {
					 movesList.push_back(y * 100 + (x - 1));
				 }
			 }
			 else {
				 if (chessboard[y][x - 1] < 10) {
					 movesList.push_back(y * 100 + (x - 1));
				 }
			 }
		 }
		 //right
		 if (x + 1 < 8) {
			 if (top) {
				 if (chessboard[y][x + 1] == 0 || chessboard[y][x + 1] > 10) {
					 movesList.push_back(y * 100 + (x + 1));
				 }
			 }
			 else {
				 if (chessboard[y][x + 1] < 10) {
					 movesList.push_back(y * 100 + (x + 1));
				 }
			 }
		 }
		 //upleft
		 if (x - 1 >= 0 && y - 1 >= 0) {
			 if (top) {
				 if (chessboard[y - 1][x - 1] == 0 || chessboard[y - 1][x - 1] > 10) {
					 movesList.push_back((y - 1) * 100 + (x - 1));
				 }
			 }
			 else {
				 if (chessboard[y - 1][x - 1] < 10) {
					 movesList.push_back((y - 1) * 100 + (x - 1));
				 }
			 }
		 }
		 //upright
		 if (x + 1 < 8 && y - 1 >= 0) {
			 if (top) {
				 if (chessboard[y - 1][x + 1] == 0 || chessboard[y - 1][x + 1] > 10) {
					 movesList.push_back((y - 1) * 100 + (x + 1));
				 }
			 }
			 else {
				 if (chessboard[y - 1][x + 1] < 10) {
					 movesList.push_back((y - 1) * 100 + (x + 1));
				 }
			 }
		 }
		 //downleft
		 if (x - 1 >= 0 && y + 1 < 8) {
			 if (top) {
				 if (chessboard[y + 1][x - 1] == 0 || chessboard[y + 1][x - 1] > 10) {
					 movesList.push_back((y + 1) * 100 + (x - 1));
				 }
			 }
			 else {
				 if (chessboard[y + 1][x - 1] < 10) {
					 movesList.push_back((y + 1) * 100 + (x - 1));
				 }
			 }
		 }
		 //downright
		 if (x + 1 < 8 && y + 1 < 8) {
			 if (top) {
				 if (chessboard[y + 1][x + 1] == 0 || chessboard[y + 1][x + 1] > 10) {
					 movesList.push_back((y + 1) * 100 + (x + 1));
				 }
			 }
			 else {
				 if (chessboard[y + 1][x + 1] < 10) {
					 movesList.push_back((y + 1) * 100 + (x + 1));
				 }
			 }
		 }
		 break;
	 }
 }

 bool checkForCheck(bool top) {
	 for (int y = 0; y < 8; y++) {
		 for (int x = 0; x < 8; x++) {
			 if (chessboard[y][x] != 0) {
				 findMoves(x, y, chessboard[y][x]);
				 for (int i = 0; i < movesList.size(); i++) {
					 if (top) {
						 if (chessboard[getY(movesList[i])][getX(movesList[i])] == 6) {
							 return true;
						 }
					 }
					 else if (chessboard[getY(movesList[i])][getX(movesList[i])] == 16) {
						 return true;
					 }
				 }
			 }
		 }
	 }
	 while (movesList.size() > 0) {
		 movesList.pop_back();
	 }
	 return false;
 }

 bool checkForMate(bool top) {
	 for (int y = 0; y < 8; y++) {
		 for (int x = 0; x < 8; x++) {
			 if (checkPiece(top, (y * 100 + x)) == 2) {
				 movePieces.push_back(y * 100 + x);
			 }
		 }
	 }
	 for (int y = 0; y < movePieces.size(); y++) {
		 findMoves(getX(movePieces[y]), getY(movePieces[y]), coordToPiece(movePieces[y]));
		 for (int x = 0; x < movesList.size(); x++) {
			 if (checkPieceMove(movesList[x], movePieces[y], coordToPiece(movePieces[y]), top, false) == 1) {
				 while (movePieces.size() > 0) {
					 movePieces.pop_back();
				 }
				 while (movesList.size() > 0) {
					 movesList.pop_back();
				 }
				 return false;
			 }
			 else {
			 }
		 }
	 }
	 checkForCheck(top);
	 return true;
 }