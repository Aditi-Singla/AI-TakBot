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
	int bestMove;
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
	Board(int n){
		this->dimension = n;
		bestMove = 0;
		//Default as 5*5
		listOfPlayers.push_back(Player(21,1));
		listOfPlayers.push_back(Player(21,1)); 
		board = new vector<string>*[dimension];
		for(int i=0;i<dimension;i++)
			board[i] = new vector<string>[dimension];
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

	void makeMove(int playerNo, string move);
	int squareToNum(string sqStr);
	int evaluate(int playerNo);
	void setDimension(int n);
	void printBoard();
	vector<string> getValidMoves(int currentPiece);
	vector<string> getValidAdds(int currentPiece);
	vector<string> getValidStackMoves(int currentPiece);
	vector<string> getMove(int direction,int i, int j, int height, bool isTopCapStone, string str, int count);
	int minimax(Board board, int depth, bool maxNode, int alpha, int beta, int playerNo, int d);
	int minimax_iter(Board board, int depth, bool maxNode, int alpha, int beta, int playerNo);
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
		return -1;
	}
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
			cout<<"Incompatible Data!, Returning form Board::makeMove"<<endl;
			return;
		}
		int col = int(move.substr(1)[0])-97;
		int row = int(move.substr(1)[1])-49; 
		if(move[0]=='F' or move[0]=='S')
		{
			string s="";
			s += char(currentPiece+49);
			s += " ";
			s += move[0];
			this->board[row][col].push_back(s);
			this->listOfPlayers[currentPiece].flatStones-=1;
 		}
 		else if(move[0]=='C')
 		{
 			string s="";
			s += char(currentPiece+49);
			s += " ";
			s += move[0];
			this->board[row][col].push_back(s);
 			this->listOfPlayers[currentPiece].capStones -= 1;
 		}
	}
	else if(isdigit(move[0]))
	{
		int count = int(move[0])-48;
		int isPossible = this->squareToNum(move.substr(1,2));
		if(isPossible==-1)
		{
			cout<<"Incompatible Data!, Returning form Board::makeMove, isdigit branch"<<endl;
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
			int currRow = (nextSquare%dimension==0?nextSquare/dimension-1:nextSquare/dimension);
			int currCol = (nextSquare%dimension==0?dimension-1:nextSquare%dimension-1);
			int lastIndex = this->board[currRow][currCol].size()-1;
			if( (this->board[currRow][currCol].size() >  0) and (this->board[currRow][currCol][lastIndex][1]=='S'))
				this->board[currRow][currCol][lastIndex] = this->board[currRow][currCol][lastIndex][0]+' '+'F';
			vector<string> initVec = this->board[row][col];
			vector<string> toAdd;
			int size = initVec.size();
			for(int j = size-count;j<size-count+nextCount;j++)
			{
				toAdd.push_back(initVec[j]);
			}
			if(this->board[currRow][currCol].size()!=0)
				this->board[currRow][currCol].insert(this->board[currRow][currCol].end(),toAdd.begin(),toAdd.end());
			else{
				// this->board[currRow][currCol] = toAdd
				for (int k=0; k<toAdd.size(); k++){
					this->board[currRow][currCol].push_back(toAdd[k]);
				}				
			}
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

int Board::evaluate(int playerNo)
{
	cerr << "Yo\n";
	int score = 0;
	for (int i=0; i<this->dimension; i++){
		for (int j=0; j<this->dimension; j++){
			vector<string> v = this->board[i][j];
			if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
				if (v.back()[2]=='F')
					score++;
				else if (v.back()[2]=='C'){
					for (int k=0; k<v.size(); k++){
						if (v[k][0] == char('0'+(playerNo+1)))
							score++;
					}
				}
				else
					continue;
			}
		}
	}
	return score;
	cerr << "Yo1\n";
	// return 1;
}

int Board::minimax(Board board1, int depth, bool maxNode, int alpha, int beta, int playerNo, int d)
{
	///Assume depth of 4
	if (depth == d){
		return (board1.evaluate(playerNo));
	}
	// this->printBoard(); 
	vector<string> validMoves = board1.getValidMoves(playerNo);
	// cerr << "Here " << depth << " " << validMoves.size() << endl;
	if (maxNode)
	{
		// cerr << "Here1" << validMoves.size();
		int best = INT_MIN;
		for (int i=0; i < validMoves.size(); i++)
		{
			Board boardTemp(board1) ;
			boardTemp.makeMove(playerNo,validMoves[i]);
			int value = minimax(boardTemp, depth+1, false, alpha, beta, 1-playerNo,d);
			if (best < value){
				this->bestMove = i;
				best = value;
			}
			alpha = std::max(alpha, best);

			if (beta <= alpha){
				// cerr << "kl";
				break;
			}
		}
		return best;
	}
	else
	{
		int best = INT_MAX;
		// cerr << "Here-1" << validMoves.size();
		for (int i=0; i< validMoves.size(); i++)
		{
			Board boardTemp(board1) ;
			boardTemp.makeMove(playerNo,validMoves[i]);
			int value = minimax(boardTemp, depth+1, true, alpha, beta,1-playerNo,d);
			if (best > value){
				this->bestMove = i;
				best = value;
			}
			beta = std::min(beta, best);

			if (beta <= alpha){
				// cerr << "LK";
				break;
			}
		}
		return best;
	}
}

int Board::minimax_iter(Board board1, int depth, bool maxNode, int alpha, int beta, int playerNo){
	int max = INT_MIN;
	for (int i=1; i<=4; i++){
		int n = this->minimax(board1, depth, maxNode, alpha, beta, playerNo, i);
		if (this->bestMove > max){
			max = this->bestMove;
		}
	}
	return max;
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
		this->totSquares = n*n;
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
	if(this->noOfMoves!=1){
		currentPiece = this->currTurnNo;
		// int highestValue = this->board.minimax(this->board, 0, true, INT_MIN, INT_MAX,currentPiece);
		// int bestMoveIndex = this->board.bestMove;
		int bestMoveIndex = this->board.minimax_iter(this->board, 0, true, INT_MIN, INT_MAX,currentPiece);
		vector<string> v = this->board.getValidMoves(currentPiece);
		cerr<<"ALL VALID MOVES!!!!!: ";
		print(v);
		cerr << endl;
		string bestMove = v[bestMoveIndex];
		return bestMove;
	}
	else{
		currentPiece = 1 - this->currTurnNo;
		// this->noOfMoves--;
		if (this->board.board[0][0].empty()){
			string bestMove = "Fa1";
			return bestMove;
		}
		else{
			string bestMove = "Fa" ;
			bestMove += std::to_string(this->dimension);
			return bestMove;
		}
	}
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