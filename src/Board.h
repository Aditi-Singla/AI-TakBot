#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctype.h>
#include <climits>
#include <algorithm>

class Board{

public:
	int dimension;
	std::vector<std::string> **board;
	class Player{
	public:
		int flatStones;
		int capStones;
		Player(int flats,int caps)
		{
			this->flatStones = flats;
			this->capStones = caps;
		}
	};

	std::vector<Player> listOfPlayers;

	Board(){

	}
	Board(int n){
		this->dimension = n;
		//Default as 5*5
		listOfPlayers.push_back(Player(21,1));
		listOfPlayers.push_back(Player(21,1)); 
		board = new std::vector<std::string>*[dimension];
		for(int i=0;i<dimension;i++)
			board[i] = new std::vector<std::string>[dimension];
	}

	// Board(Board &b){
	// 	this->dimension = b.dimension;
	// 	this->listOfPlayers = NULL;
	// 	this->board = NULL;
	// }

	void makeMove(int playerNo, std::string move);
	int squareToNum(std::string sqStr);
	std::vector<std::string> getValidMoves(int currentPiece);
	int evaluate();
	void setDimension(int n);
};

struct Node
{
	Node** children;
	int no_of_children;
	int value;

	void deleteNode();
};


class Tree{

public:
	Node* tree;
	int bestMove;

	Tree(Board board, int playerNo)
	{
		tree = makeTree(board,0,playerNo);
		bestMove = -1;
	}
	Node* makeTree(Board board, int depth, int playerNo);
	void deleteTree();
	int minimax(Node* root, int depth, bool maxNode, int alpha, int beta);
};
