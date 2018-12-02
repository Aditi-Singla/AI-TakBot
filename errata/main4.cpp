#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctype.h>
#include <climits>
#include <algorithm>
#include <string>
#include <stdlib.h>
//#include "Board.h"
#define MAX 10000
#define MIN -10000

using namespace std;
//Debugger functions 
void split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
    }
}

void print(vector<string> elems)
{
	for(int i=0;i<elems.size();i++)
		cerr << elems[i] << " ";

	// cerr<<endl;
}
class Board{

public:
	int dimension;
	//vector<string> board[dimension];
	vector<string> **board;
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

	vector<Player> listOfPlayers;

	Board(){

	}
	Board(const Board &obj)
	{	
		dimension = obj.dimension;
		board = new vector<string>*[dimension];
		for(int i=0;i<dimension;i++)
			board[i] = new vector<string>[dimension];
		**board = **obj.board;
		listOfPlayers = obj.listOfPlayers;
	}
	Board(const Board &obj)
	{	
		dimension = obj.dimension;
		board = new vector<string>*[dimension];
		for(int i=0;i<dimension;i++)
		{
			board[i] = new vector<string>[dimension];
			for(int j=0;j<dimension;j++)
			{
				board[i][j] = obj.board[i][j];
			}
		}
		listOfPlayers = obj.listOfPlayers;
	}
	~Board(void){
		for (int i=0; i<dimension; i++)
    		delete[] board[i];

		delete[] board;
		// cerr << "Delete mem ";
	}
	~Board()
	{
		for (int i=0; i<dimension; i++)
    		delete[] board[i];

		delete[] board;
	}
	void makeMove(int playerNo, string move);
	int squareToNum(string sqStr);
	int evaluate();
	void setDimension(int n);
	void printBoard();
	vector<string> getValidMoves(int currentPiece);
	vector<string> getValidAdds(int currentPiece);
	vector<string> getValidStackMoves(int currentPiece);
	vector<string> getMove(int direction,int i, int j, int height, bool isTopCapStone, string str, int count);
};
void Board::printBoard(){
	for (int i=0; i<this->dimension; i++){
		for (int j=0; j<this->dimension; j++){
			print(this->board[i][j]);
			cerr << " , ";
		}
		cerr << endl;
	}		
}

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
	if(sqStr.length()!=2){
		return -1;
	}
	if(!(isalpha(sqStr[0])) or !(islower(sqStr[0])) or !(isdigit(sqStr[1]))){
		return -1;
	}

	int col = int(sqStr[0]) - 96;
	int row = int(sqStr[1]) - 48;
	if(row<1 or row>this->dimension or col<1 or col>this->dimension){
		// cerr<<"Problem ";
		// cerr<<"row: "<<row<<" col: "<<col<<endl;
		return -1;
	}

	// cerr<<"Ok ";
	// cerr<<"row: "<<row<<" col: "<<col<<endl;
	return 1;
}

void Board::makeMove(int currentPiece, string move)
{
	/* Update Shit
		-> Update the board the of this.Game object
		-> Update the GameState
	*/
	// cerr<<"Move selected: "<<move<<endl;
	if(isalpha(move[0]))
	{
		int isPossible = this->squareToNum(move.substr(1));
		if(isPossible==-1)
		{
			cerr<<"Incompatible Data!, Returning form Board::makeMove"<<endl;
			return;
		}
		int col = int(move.substr(1)[0])-97;
		int row = int(move.substr(1)[1])-49; 
		// cerr<<"Computed Row: "<<row<<" Computed Col: "<<col<<endl;
		if(move[0]=='F' or move[0]=='S')
		{
			string s="";
			s += char(currentPiece+49);
			// cerr<<"S:"<<s<<endl;
			s += " ";
			// cerr<<"S:"<<s<<endl;
			s += move[0];
			// cerr<<"S:"<<s<<endl;
			// cerr<<"Adding "<<s<<" to "<<row<<" "<<col<<endl;
			this->board[row][col].push_back(s);
			this->listOfPlayers[currentPiece].flatStones-=1;
 		}
 		else if(move[0]=='C')
 		{
 			string s="";
			s += char(currentPiece+49);
			// cerr<<"S:"<<s<<endl;
			s += " ";
			// cerr<<"S:"<<s<<endl;
			s += move[0];
			// cerr<<"S:"<<s<<endl;
			// cerr<<"Adding "<<s<<" to "<<row<<" "<<col<<endl;
			this->board[row][col].push_back(s);
 			// cerr<<"Adding "<<s<<" to "<<row<<" "<<col<<endl;
 			this->listOfPlayers[currentPiece].capStones -= 1;
 		}
	}
	else if(isdigit(move[0]))
	{
		int count = int(move[0])-48;
		int isPossible = this->squareToNum(move.substr(1,2));
		if(isPossible==-1)
		{
			cerr<<"Incompatible Data!, Returning form Board::makeMove, isdigit branch"<<endl;
			return;
		}
		int col = int(move.substr(1)[0])-97;
		int row = int(move.substr(1)[1])-49; 
		char direction = move[3];
		int change;
		if(direction=='+')
			change = this->dimension;
		else if(direction=='-')
			change = -1 * this->dimension;
		else if(direction=='>')
			change = 1;
		else if(direction=='<')
			change = -1;
		int prevSquare = col + this->dimension*(row) + 1;
		for(int i = 4;i<move.length();i++)
		{
			int nextCount = int(move[i])-48;
			int nextSquare = prevSquare + change;
			// cerr<<"prevSquare: "<<prevSquare<<" NextSquare: "<<nextSquare<<" Change: "<<change<<endl;
			int currRow = (nextSquare%dimension==0?nextSquare/dimension-1:nextSquare/dimension);
			int currCol = (nextSquare%dimension==0?dimension-1:nextSquare%dimension-1);
			// cerr<<"currRow: "<<currRow<<" currCol: "<<currCol<<endl;
			int lastIndex = this->board[currRow][currCol].size()-1;
			// cerr<<"lastIndex: "<<lastIndex<<endl;
			if( (this->board[currRow][currCol].size() >  0) and (this->board[currRow][currCol][lastIndex][1]=='S'))
				this->board[currRow][currCol][lastIndex] = this->board[currRow][currCol][lastIndex][0]+' '+'F';
				//Pull out from top of vect	or , till top-nextCount
			// cerr<<"I am here"<<endl;
			vector<string> initVec = this->board[row][col];
			vector<string> toAdd;
			int size = initVec.size();
			// cerr<<"initVec size "<<size<<endl;
			// cerr<<"count: "<<count<<" size: "<<size<<endl;
			// cerr<<"initVec: ";
			// print(initVec);
			for(int j = size-count;j<size-count+nextCount;j++)
			{
				// cerr<<j<<endl;
				toAdd.push_back(initVec[j]);
				// print(toAdd);
			}
			// cerr<<"I am here now"<<endl;
			if(this->board[currRow][currCol].size()!=0)
				this->board[currRow][currCol].insert(this->board[currRow][currCol].end(),toAdd.begin(),toAdd.end());
			else{
				// this->board[currRow][currCol] = toAdd
				for (int k=0; k<toAdd.size(); k++){
					this->board[currRow][currCol].push_back(toAdd[k]);
				}				
			}

			// cerr<<"I am here as well"<<endl;
			prevSquare = nextSquare;	
			count -= nextCount;
		}	
		count = int(move[0])-48;
		int i = count;
		while(i--)
		{
			this->board[row][col].pop_back();
		}
		// print(this->board[row][col]);
		//this->board[row][col] = this->board[row][col][:-count];
	}
}

vector<string> Board::getValidMoves(int currentPiece){
	vector<string> v = this->getValidAdds(currentPiece);
	vector<string> v1 = this->getValidStackMoves(currentPiece);
	if (v.empty()){
		return v1;
	}
	else{
		v.insert(v.end(),v1.begin(),v1.end());
		return v;
	}
}

vector<string> Board::getValidAdds(int currentPiece){
	vector<string> list;
	char c;
	int flats = this->listOfPlayers[currentPiece].flatStones;
	int caps = this->listOfPlayers[currentPiece].capStones;
	for (int i=0; i<this->dimension; i++){
		for (int j=0; j<this->dimension; j++){
			if (this->board[i][j].empty()){
				if (caps>0){
					string s = "C";
					s += char('a'+j);
					s += std::to_string(i+1);
					list.push_back(s);
				}
				if (flats>0){
					string s = "F";
					s += char('a'+j);
					s += std::to_string(i+1);
					list.push_back(s);
					s = "S";
					s += char('a'+j);
					s += std::to_string(i+1);
					list.push_back(s);
				}
			}
			else
				continue;
		}
	}
	return list;
}
vector<string> Board::getValidStackMoves(int currentPiece){
	vector<string> list;
	for (int i=0; i<this->dimension; i++){
		for (int j=0; j<this->dimension; j++){
			if (this->board[i][j].empty() || this->board[i][j].back()[0]!=char('0'+(currentPiece+1))){   /////assuming the pieces are placed as (F/S/C)(0/1)
				continue;
			}
			else{
				int heightStack = this->board[i][j].size();
				bool isTopCapStone = false;
				if (heightStack > this->dimension)
					heightStack = this->dimension;
				if (this->board[i][j].back()[2]=='C')
					isTopCapStone = true;
				string arr[4] = {"<",">","-","+"};
				for (int k = 0; k<4; k++){	
					string st = "";
					st += char('a'+j);
					st += std::to_string(i+1)+arr[k];
					vector<string> v1 = this->getMove(k,i,j,heightStack,isTopCapStone,st,0);
					if (list.empty())
						list = v1;
					else
						list.insert(list.end(),v1.begin(),v1.end());
				}	
			}
		}
	}
	return list;
}

//////To be tested and verified again
vector<string> Board::getMove(int direction,int i, int j, int height, bool isTopCapStone, string str, int count){
	//////	0 : "<"		1 : ">"		2 : "-" 	3: "+"
	int arr1[4] = {j,this->dimension-j-1,i,this->dimension-i-1};   /////To check the extremes
	int row[4] = {i,i,i-1,i+1};				////Row No. of adjacent square in the given direction
	int col[4] = {j-1,j+1,j,j};				////Column No. of adjacent square in the given direction
	vector<string> v;
	if (height==0 || arr1[direction]==0){
		if (count==0)
			return v;
		else{
			v.push_back(std::to_string(count) + str);
			return v;
		}
	}
	int nextRow = row[direction];
	int nextCol = col[direction];
	if (!this->board[nextRow][nextCol].empty() && (this->board[nextRow][nextCol].back()[2] == 'C')){
		return v;
	}
	else if (!this->board[nextRow][nextCol].empty() && (this->board[nextRow][nextCol].back()[2] == 'S')){
		if (isTopCapStone){
			v.push_back(std::to_string(count+1)+str+"1");
			return v;
		}
		else{
			if (count==0)
				return v;
			else{
				v.push_back(std::to_string(count)+str);
				return v;
			}
		}
	}
	else{	////FlatStone or empty
		for (int k=1; k<=height; k++){
			vector<string> v1 = this->getMove(direction,nextRow,nextCol,height-k,isTopCapStone,str+std::to_string(k),count+k);
			if (v1.empty()){
				continue;
			}
			else{
				if (v.empty())
					v = v1;
				else
					v.insert(v.end(),v1.begin(),v1.end());
			}
		}
		return v;
	}	
}


int Board::evaluate()
{
	return 1;
}


struct Node
{
	Node** children;
	int no_of_children;
	int value;

	void deleteNode();
};


void Node::deleteNode()
{
	for (int i = 0; i != this->no_of_children; ++i)
		this->children[i]->deleteNode();
	delete [] this->children;
	delete this;
}

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

class Game{

	int dimension;
	int totSquares;
	int currTurnNo;
	int maxNoOfMovablePieces;
	int maxUp;
	int maxDown;
	char maxRight;
	int noOfMoves;
	Board board;
	
public:
	Game()
	{

	}
	Game(int n)
	{
		this->dimension = n;
		this->	totSquares = n*n;
		this->currTurnNo = 0;
		this->maxNoOfMovablePieces = this->dimension;
		this->maxUp = this->dimension;
		this->maxDown = 1;
		this->maxRight = char(int('a'+n-1));
		this->noOfMoves = 0;
		this->board.setDimension(this->dimension);
	}

	string getBestMove();
	void makeMove(string move);
	void setDim(int n);
};
void Game::setDim(int n)
{
		this->dimension = n;
		this->totSquares = n*n;
		this->currTurnNo = 0;
		this->maxNoOfMovablePieces = this->dimension;
		this->maxUp = this->dimension;
		this->maxDown = 1;
		this->maxRight = char(int('a'+n-1));
		this->noOfMoves = 0;
		this->board.setDimension(this->dimension);
}
string Game::getBestMove(){
	int currentPiece;
	if(this->currTurnNo==0)
		this->noOfMoves+=1;
	if(this->noOfMoves!=1)
		currentPiece = this->currTurnNo;
	else
		currentPiece = 1 - this->currTurnNo;
	// Tree* minmaxTree = new Tree(this->board, currentPiece);
	// int highestValue = minmaxTree -> minimax(minmaxTree->tree, 0, true, INT_MIN, INT_MAX);
	// int bestMoveIndex = minmaxTree -> bestMove;
	// cerr<<"ALL nkbkk: ";
	vector<string> v = this->board.getValidMoves(currentPiece);
	cerr<<"ALL VALID MOVES!!!!!!!!!!!!!!!!: ";
	print(v);
	cerr << endl;
	int bestMoveIndex = rand()%v.size();
	string bestMove = v[bestMoveIndex];
	// minmaxTree -> deleteTree();
	// delete minmaxTree;
	return bestMove;
}

void Game::makeMove(string move)
{
	int currentPiece;
	if(this->currTurnNo==0)
		this->noOfMoves+=1;
	if(this->noOfMoves!=1)
		currentPiece = this->currTurnNo;
	else
		currentPiece = 1 - this->currTurnNo;

	this->board.makeMove(currentPiece, move);
	this->currTurnNo = 1 - this->currTurnNo;
}


class AIPlayer{
	int sizeOfBoard;
	int timeLeft;
	int playerNo;
	Game game;
	
    public:
		AIPlayer(string start)
		{
			//TODO:Initialize the params 
			string data;
			cin >> data;
			this->playerNo = atoi(data.c_str())-1;
			cin >> data;
			this->sizeOfBoard = atoi(data.c_str());
			cin >> data;
			this->timeLeft = atoi(data.c_str()); //(elems[2].c_str());
			this->game.setDim(this->sizeOfBoard);
			this->Play();
		}
		void Play();
};
void AIPlayer::Play()
{
	if(this->playerNo==1)
	{
		//Player 2
		string move;
		cin >> move;
		this->game.makeMove(move);
	}
	while(true)
	{
		//making our move
		string moveChosen = this->game.getBestMove();//this->playerNo);
		//cin >> moveChosen;
		this->game.makeMove(moveChosen);
		moveChosen = moveChosen + '\n';
		cout << moveChosen;
		string move;
		cin >> move;
		if(move!="")
			cerr<<"MAKEMOVE"<<move<<endl;
		else if(move=="")
			{
				cerr<<"EXITING";
				exit(0);
			}	

		this->game.makeMove(move); 
	}	
}

int main()
{
	//Default constructor called(unparam)
	AIPlayer aiPlayer("start");
}