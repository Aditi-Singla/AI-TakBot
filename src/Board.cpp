#include "Board.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctype.h>
#include <climits>
#include <algorithm>

using namespace std;

void Board::setDimension(int n)
{
		this->dimension = n;
		//Default as 5*5
		listOfPlayers.push_back(Player(21,1));
		listOfPlayers.push_back(Player(21,1)); 
		board = new vector<string>*[dimension];
		for(int i=0;i<dimension;i++)
			board[i] = new vector<string>[dimension];
}
int Board::squareToNum(string sqStr)
{
	if(sqStr.length()!=2)
		return -1;
	if(!(isalpha(sqStr[0])) or !(islower(sqStr[0])) or !(isdigit(sqStr[1])) )
		return -1;

	int row = int(sqStr[0]) - 96;
	int col = int(sqStr[1]);
	if(row<1 or row>this->dimension or col<1 or col>this->dimension)
		return -1;

	return 1;
}

void Board::makeMove(int currentPiece, string move)
{
	/* Update Shit
		-> Update the board the of this.Game object
		-> Update the GameState
	*/
	if(isalpha(move[0]))
	{
		int isPossible = this->squareToNum(move.substr(1));
		if(isPossible==-1)
		{
			std::cout<<"Incompatible Data!, Returning form Board::makeMove"<<endl;
			return;
		}
		int row = int(move.substr(1)[0])-97;
		int col = int(move.substr(1)[1])-1; 
		if(move[0]=='F' or move[0]=='S')
		{
			string s;
			s = char(currentPiece+0)+' '+move[0];
			this->board[row][col].push_back(s);
			this->listOfPlayers[currentPiece].flatStones-=1;
 		}
 		else if(move[0]=='C')
 		{
 			string s;
 			s = char(currentPiece+0)+' '+move[0];
 			this->board[row][col].push_back(s);
 			this->listOfPlayers[currentPiece].flatStones -= 1;
 		}
	}
	else if(isdigit(move[0]))
	{
		int count = int(move[0]);
		int isPossible = this->squareToNum(move.substr(1,2));
		if(isPossible==-1)
		{
			std::cout<<"Incompatible Data!, Returning form Board::makeMove, isdigit branch"<<endl;
			return;
		}
		int row = int(move.substr(1)[0])-97;
		int col = int(move.substr(1)[1])-1; 
		char direction = move[3];
		int change;
		if(direction=='+')
			change = 1;
		else if(direction=='-')
			change = -1;
		else if(direction=='>')
			change = 1;
		else if(direction=='<')
			change = -1;
		int prevSquare = this->dimension * (col - 1) + (row - 1);
		for(int i = 4;i<move.length();i++)
		{
			int nextCount = int(move[i]);
			int nextSquare = prevSquare + change;
			int currRow = (nextSquare%dimension==0?nextSquare/dimension-1:nextSquare/dimension);
			int currCol = (nextSquare%dimension==0?nextSquare/dimension-1:nextSquare%dimension-1);
			int lastIndex = this->board[currRow][currCol].size()-1;
			if( (this->board[currRow][currCol].size() >  0) and (this->board[currRow][currCol][lastIndex][1]=='S'))
				this->board[currRow][currCol][lastIndex] = this->board[currRow][currCol][lastIndex][0]+' '+'F';
				//Pull out from top of vect	or , till top-nextCount
			vector<string> initVec = this->board[row][col];
			vector<string> toAdd;
			int size = initVec.size()-1;
			for(int j = size-count;j<size-count+nextCount;j++)
			{
				toAdd.push_back(initVec[j]);
			}

			if(this->board[currRow][currCol].size()!=0)
				this->board[currRow][currCol].insert(this->board[currRow][currCol].end(),toAdd.begin(),toAdd.end());
			else
				this->board[currRow][currCol] = toAdd;

			prevSquare = nextSquare;	
			count -= nextCount;
		}	
		count = int(move[0]);
		int i = count;
		while(i--)
		{
			this->board[row][col].pop_back();
		}
		//this->board[row][col] = this->board[row][col][:-count];
	}
}

vector<string> Board::getValidMoves(int currentPiece){
	vector<string> v;
	return v;
	/////If there is no empty space, game doesnot end. Moving stacks is an option, which might create an empty space
	// int square = this.squareToNum(move,1,move.size());
	// if(move[0]=='F' or move[0]=='S')
	// {
	// 	this.board[square].push_back(move[0]+" "+currentPiece);
	// 	this.listOfPlayers[currentPiece].flatStones -= 1;
	// }
	// else if(move[0]=='C')
	// {
	// 	this.board[square].push_back(move[0]+" "+currentPiece);
	// 	this.listOfPlayers[currentPiece].capStones -= 1;
	// }


	// else if(isdigit(move[0]))
	// {
	// 	int count = int(move[0]);
	// 	int square = this.squareToNum(move,1,3);
	// 	string direction = move[3];
	// 	int change;
	// 	if(direction=="+")
	// 		change = this.dimension;	
	// 	else if(direction=="-")
	// 		change = -1*this.dimension;
	// 	else if(direction==">")
	// 		change = 1;
	// 	else if(direction=="<")
	// 		change = -1;
	// 	int prevSquare = square;
	// 	for(int i=4;i<move.size();i++)
	// 	{//Ignore the syntax from this point
	// 		int nextCount = int(move[i]);
	// 		int nextSquare = prevSquare + change;
	// 		if( (this.board[nextSquare].size()>0) and (this.board[nextSquare][-1][1] == 'S'))
	// 			this.board[nextSquare][-1] = this.board[nextSquare][-1][0] + "F";	
	// 		if(nextCount-count==0)
	// 			this.board[nextSquare] += this.board[square][-count:];
	// 		else
	// 			this.board[nextSquare] += this.board[square][-count:-count+nextCount];
	// 		prevSquare = nextSquare;
	// 		count -= nextCount
	// 	}
	// 	count = int(move[0])
	// 	this.board[square] = this.board[square][:-count]
	// }
}

int Board::evaluate()
{
	return 1;
}

void Node::deleteNode()
{
	for (int i = 0; i != this->no_of_children; ++i)
		this->children[i]->deleteNode();
	delete [] this->children;
	delete this;
}

Node* Tree::makeTree(Board board, int depth, int playerNo)
{
	Node* node = new Node();
	vector<string> validMoves = board.getValidMoves(playerNo);
	node -> no_of_children = validMoves.size();
	node -> value = board.evaluate();
	if (depth > 0 && node->no_of_children > 0)
	{
		node -> children = new Node* [node->no_of_children];
		for (int i = 0; i != node->no_of_children; ++i){
			//Board boardTemp;
			board.makeMove(playerNo,validMoves[i]);
			node -> children[i] = this->makeTree(board, depth - 1,1-playerNo);
		}
	}
	else
	{
		node -> children = NULL;
	}
	return node;
}

void Tree::deleteTree()
{
	for (int i = 0; i != this->tree->no_of_children; ++i)
		this->tree->children[i]->deleteNode();
	delete [] this->tree->children;
}

int Tree::minimax(Node* root, int depth, bool maxNode, int alpha, int beta)
{
	///Assume depth of 3
	if (depth == 3)
		return (root -> value);

	if (maxNode)
	{
		int best = INT_MIN;
		for (int i=0; i < (root -> no_of_children); i++)
		{
			int value = minimax(tree->children[i], depth+1, false, alpha, beta);
			this->bestMove = i;
			best = 	max(best, value);
			alpha = std::max(alpha, best);

			if (beta <= alpha)
				break;
		}
		return best;
	}
	else
	{
		int best = INT_MAX;

		for (int i=0; i< (root->no_of_children); i++)
		{
			int value = minimax(root -> children[i], depth+1, true, alpha, beta);
			this->bestMove = i;
			best = std::min(best, value);
			beta = std::min(beta, best);

			if (beta <= alpha)
				break;
		}
		return best;
	}
}
