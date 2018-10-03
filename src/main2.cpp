#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctype.h>
#include <climits>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <set>
#include <ctime>
//#include "Board.h"
#define MAX 10000
#define MIN -10000

const int MAXV = 2000000;
using namespace std;
double TIME;
double AVGTIME;
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
	for(int i=0;i<elems.size();i++){
		cerr << elems[i] << " ";
	}
}
class Board{

public:
	int dimension;
	int bestMove;
	bool roadBool;
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
	// Board(const Board &obj)
	// {	
	// 	dimension = obj.dimension;
	// 	board = new vector<string>*[dimension];
	// 	for(int i=0;i<dimension;i++)
	// 		board[i] = new vector<string>[dimension];
	// 	**board = **obj.board;
	// 	listOfPlayers = obj.listOfPlayers;
	// }
	Board(int n){
		this->dimension = n;
		bestMove = 0;
		roadBool = false;
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
		bestMove = obj.bestMove;
		roadBool = obj.roadBool;
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
		// //cerr << "Delete mem ";
	}
	void makeMove(int playerNo, string move);
	void makeMove(int playerNo, string move, bool &hasFlatten);
	void undoMove(int playerNo, string move, bool &hasFlatten);
	int squareToNum(string sqStr);
	int evaluate(int playerNo);
	void setDimension(int n);
	void printBoard();
	vector<string> getValidMoves(int currentPiece);
	vector<string> getValidAdds(int currentPiece);
	vector<string> getValidStackMoves(int currentPiece);
	vector<string> getMove(int direction,int i, int j, int height, bool isTopCapStone, string str, int count);
	vector<int> minimax(Board board, int depth, bool maxNode, int alpha, int beta, int playerNo, int d);
	int minimax_iter(Board board, int depth, bool maxNode, int alpha, int beta, int playerNo, int d);
	int centerEvaluatorDP(int playerNo,int weightForThreat);
	vector<int> centerEvaluatorDP1(int playerNo,int weightForThreat);
	vector<int> neighbours(int top);
	bool checkRoadWin(int playerNo,string dir);
	bool checkRoadWin(int playerNo);		
};
vector<int> Board::neighbours(int top)
{
	vector<int> ret;
	if(top<0 or top>=this->dimension*this->dimension)
		return ret;
	else if(top==0)
	{
		ret.push_back(top+1);
		ret.push_back(top+this->dimension);
		return ret;
	}
	else if(top==this->dimension-1)
	{
		ret.push_back(top-1);
		ret.push_back(top+this->dimension);
		return ret;	
	}
	else if(top==this->dimension*this->dimension - this->dimension)
	{
		ret.push_back(top+1);
		ret.push_back(top-this->dimension);
		
		return ret;	
	}
	else if(top==this->dimension*this->dimension-1)
	{
		ret.push_back(top-1);
		ret.push_back(top-this->dimension);
		
		return ret;	
	}
	else if(top<this->dimension)
	{
		ret.push_back(top-1);
		ret.push_back(top+1);
		ret.push_back(top+this->dimension);
		
		return ret;	
	}
	else if(top%this->dimension==0)
	{
		ret.push_back(top+1);
		ret.push_back(top-this->dimension);
		ret.push_back(top+this->dimension);
		
		return ret;	
	}
	else if((top+1)%this->dimension==0)
	{
		ret.push_back(top-1);
		ret.push_back(top-this->dimension);
		ret.push_back(top+this->dimension);
		
		return ret;	
	}
	else if(top>=this->dimension*this->dimension - this->dimension)
	{
		ret.push_back(top-1);
		ret.push_back(top+1);
		ret.push_back(top-this->dimension);
		
		return ret;	
	}
	else
	{
		ret.push_back(top-1);
		ret.push_back(top+1);
		ret.push_back(top-this->dimension);
		ret.push_back(top+this->dimension);
		return ret;	
	}	
}
bool Board::checkRoadWin(int playerNo,string dir)
{
	set<int> visited;
	vector<int> dfsStack;
	set<int> finalPos;
	if(dir=="Vertical")
	{
		for(int i=0;i<this->dimension;i++)
		{
			vector<string> currVec = this->board[0][i];
			if( currVec.size()>0 and currVec[currVec.size()-1][0]==char(playerNo+49) and currVec[currVec.size()-1][2]!='S' )
			  {
			  	visited.insert(i);
			  	dfsStack.push_back(i);
			  }
			finalPos.insert(this->dimension*this->dimension -1 -i);
		}
	}
	else if(dir=="Horizontal")
	{
		for(int i=0;i<this->dimension;i++)
		{
			// //cerr<<"I: "<<i<<" In horizontal dir"<<endl;
			// if(i==1)
			// 	this->printBoard();
			vector<string> currVec = this->board[i][0];
			////cerr<<"currVec: "<<endl;
			// print(currVec);
			// //cerr<<endl;
			if( currVec.size()>0 and currVec[currVec.size()-1][0]==char(playerNo+49) and currVec[currVec.size()-1][2]!='S')
			{
				visited.insert(i*this->dimension);
				dfsStack.push_back(i*this->dimension);
			}
			finalPos.insert( (i+1)*this->dimension -1 );
		}
	}
	while( dfsStack.size()>0 )
	{
		//print(dfsStack);
		int top = dfsStack[dfsStack.size()-1];
		dfsStack.pop_back();
		
		////cerr<<"stacksize "<<dfsStack.size()<<endl;
		if(finalPos.find(top)!=finalPos.end())
		{
			return true;
		}
		vector<int> ngbr= neighbours(top);
		for(int i=0;i<ngbr.size();i++)
		{
			int currRow = (ngbr[i]/this->dimension);
			int currCol = (ngbr[i]%this->dimension);
			// cout<<"neighbour: "<<ngbr[i]<<endl;
			// cout<<"currRow: "<<currRow<<endl;
			// cout<<"currCol: "<<currCol<<endl;
			if( visited.find(ngbr[i])==visited.end()) 
			{
			//	cout<<"BILKULFIRST"<<endl;
				if(this->board[currRow][currCol].size()>0)
				{
			//		cout<<"PEHLIDO"<<endl;
					if( this->board[currRow][currCol][this->board[currRow][currCol].size()-1][0]==char(playerNo+49) and this->board[currRow][currCol][this->board[currRow][currCol].size()-1][2]!='S' )
					{
			//			cout<<"NEXTDO"<<endl;
						dfsStack.push_back(ngbr[i]);
						visited.insert(ngbr[i]);	
				
					}
				}
			}
		} 	
	}

	return false;
}
bool Board::checkRoadWin(int playerNo)
{
	return checkRoadWin(playerNo,"Horizontal") or checkRoadWin(playerNo,"Vertical");
}
void Board::printBoard(){
	for (int i=0; i<this->dimension; i++){
		for (int j=0; j<this->dimension; j++){
			cerr << " [";
			cerr<<char('a'+j);
			cerr<<(i+1);
			cerr<<"] ";
			print(this->board[i][j]);
			cerr << " ,";
		}
		cerr << endl;
	}		
}

void Board::setDimension(int n)
{
		this->dimension = n;
		//Default as 5*5
		if(n==5)
		{
			listOfPlayers.push_back(Player(21,1));
			listOfPlayers.push_back(Player(21,1)); 
		}
		else if(n==6)
		{
			listOfPlayers.push_back(Player(30,1));
			listOfPlayers.push_back(Player(30,1));
		}	
		else if(n==7)
		{
			listOfPlayers.push_back(Player(40,1));
			listOfPlayers.push_back(Player(40,1));
		}
		else
		{
			cerr<<"MAA CHUDA"<<endl;
			exit(0);
		}
		cerr<<listOfPlayers[0].flatStones<<" "<<listOfPlayers[0].capStones<<endl;
		cerr<<listOfPlayers[1].flatStones<<" "<<listOfPlayers[1].capStones<<endl;
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
			//cerr<<"Incompatible Data!, Returning form Board::makeMove"<<endl;
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
			//cerr<<"Incompatible Data!, Returning form Board::makeMove, isdigit branch"<<endl;
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
			// if( (this->board[currRow][currCol].size() >  0) and (this->board[currRow][currCol][lastIndex][1]=='S'))
			// 	this->board[currRow][currCol][lastIndex] = this->board[currRow][currCol][lastIndex][0]+' '+'F';
			if( (this->board[currRow][currCol].size() >  0) and (this->board[currRow][currCol][lastIndex][2]=='S'))
				{
					string s="";
					s += this->board[currRow][currCol][lastIndex][0];
					s += " ";
					s += 'F';
					this->board[currRow][currCol][lastIndex] = s;
				}
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

void Board::makeMove(int currentPiece, string move, bool &flatten)
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
			//cerr<<"Incompatible Data!, Returning form Board::makeMove"<<endl;
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
 		flatten = false;
	}
	else if(isdigit(move[0]))
	{
		int count = int(move[0])-48;
		int isPossible = this->squareToNum(move.substr(1,2));
		if(isPossible==-1)
		{
			//cerr<<"Incompatible Data!, Returning form Board::makeMove, isdigit branch"<<endl;
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
			if ((this->board[currRow][currCol].size() >  0) and (this->board[currRow][currCol][lastIndex][2]=='S')){
				string s="";
				s += this->board[currRow][currCol][lastIndex][0];
				s += " ";
				s += 'F';
				this->board[currRow][currCol][lastIndex] = s;
				flatten = true;
			}
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
void Board::undoMove(int currentPiece, string move, bool &flatten)
{
	if(isalpha(move[0]))
	{
		// int isPossible = this->squareToNum(move.substr(1));
		// if(isPossible==-1)
		// {
		// 	//cerr<<"Incompatible Data!, Returning form Board::makeMove"<<endl;
		// 	return;
		// }
		int col = int(move.substr(1)[0])-97;
		int row = int(move.substr(1)[1])-49; 
		if(move[0]=='F' or move[0]=='S')
		{	
			this->board[row][col].pop_back();
			this->listOfPlayers[currentPiece].flatStones+=1;
 		}
 		else if(move[0]=='C')
 		{
 			this->board[row][col].pop_back();
 			this->listOfPlayers[currentPiece].capStones += 1;
 		}
	}
	else if(isdigit(move[0]))
	{
		// ]int isPossible = this->squareToNum(move.substr(1,2));
		// if(isPossible==-1)
		// {
		// 	//cerr<<"Incompatible Data!, Returning form Board::makeMove, isdigit branch"<<endl;
		// 	return;
		// }
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
			int size = this->board[currRow][currCol].size();
			for(int j = size-nextCount;j<size;j++){
				this->board[row][col].push_back((this->board[currRow][currCol])[j]);
			}
			for (int j=0; j<nextCount; j++){
				this->board[currRow][currCol].pop_back();
			}
			prevSquare = nextSquare;
			if ((i == move.length()-1) && (flatten)){
				string s="";
				s += this->board[currRow][currCol][this->board[currRow][currCol].size()-1][0];
				s += " ";
				s += 'S';
				this->board[currRow][currCol][this->board[currRow][currCol].size()-1] = s;
			}
		}	
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
				if (caps>0){
					string s = "C";
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

int Board::centerEvaluatorDP(int playerNo,int weightForThreat)
{
	int values[this->dimension][this->dimension];
	int prev[this->dimension][this->dimension];
	int max=0;
	int i_max = 0;
	int j_max = 0;
	for(int i=0;i<this->dimension;i++)
	{
		for(int j=0;j<this->dimension;j++)
		{
			vector<string> v = this->board[i][j];
			if (i==0 && j==0){
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					values[i][j] = 1;
					prev[i][j] = 0;
					max = 1;
				}	
				else{
					values[i][j] = 0;
					prev[i][j] = -2;
				}	
			}
			else if (i==0){
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					values[i][j] = 1 + values[i][j-1];
					if (values[i][j] > max){
						max = values[i][j];
						i_max = i;
						j_max = j;
					}
					if (prev[i][j-1]==-2)
						prev[i][j] = 0;
					else
						prev[i][j] = -1;
				}	
				else{
					values[i][j] = 0;
					prev[i][j] = -2;
				}	
			}
			else if (j==0){
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					values[i][j] = 1 + values[i-1][j];
					if (values[i][j] > max){
						max = values[i][j];
						i_max = i;
						j_max = j;
					}
					if (prev[i-1][j]==-2)
						prev[i][j] = 0;
					else
						prev[i][j] = 1;
				}	
				else{
					values[i][j] = 0;
					prev[i][j] = -2;
				}	
			}
			else{
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					if (values[i][j-1] > values[i-1][j]){
						values[i][j] = 1 + values[i][j-1];
						if (values[i][j] > max){
							max = values[i][j];
							i_max = i;
							j_max = j;
						}
						if (prev[i][j-1]==-2)
							prev[i][j] = 0;
						else
							prev[i][j] = -1;

					}
					else{
						values[i][j] = 1 + values[i-1][j];
						if (values[i][j] > max){
							max = values[i][j];
							i_max = i;
							j_max = j;
						}
						if (prev[i-1][j]==-2)
							prev[i][j] = 0;
						else
							prev[i][j] = 1;
					}	
				}
				else{
					values[i][j] = 0;
					prev[i][j] = -2;	
				}
			}
			v.clear();
		}
	}
	vector<int> rows;
	vector<int> cols;
	////cerr << i_max << "----" << j_max << endl;
	while (values[i_max][j_max] >= 1){
		rows.push_back(i_max);
		cols.push_back(j_max);
		if (prev[i_max][j_max] == -1)
			j_max--;
		else if (prev[i_max][j_max] == 1)
			i_max--;
		else if (prev[i_max][j_max] == 0)
			break;
		
	}
	
	int score=0;
	int jMax=INT_MIN,jMin=INT_MAX,iMin=INT_MAX,iMax=INT_MIN;
	for(int i=0;i<rows.size();i++)
	{
		if(rows[i]>iMax)
			iMax = rows[i];
		if(rows[i]<iMin)
			iMin = rows[i];
		if(cols[i]>jMax)
			jMax = cols[i];
		if(cols[i]<jMin)
			jMin = cols[i];
	}

	if( (max>=(this->dimension/2)+2) and (jMax-jMin>=this->dimension/2 or iMax-iMin>=this->dimension/2) )
		score = max*weightForThreat;


	return score;
	// //cerr << "Values : \n";
	// for (int i = 0; i<this->dimension; i++){
	// 	for (int j=0; j<this->dimension; j++){
	// 		//cerr << values[i][j] << " ";
	// 	}
	// 	//cerr << "\n";
	// }
	// //cerr << "Prev : \n";
	// for (int i = 0; i<this->dimension; i++){
	// 	for (int j=0; j<this->dimension; j++){
	// 		//cerr << prev[i][j] << " ";
	// 	}
	// 	//cerr << "\n";
	// }
	// //cerr << "Path : \n";
	// for (int i = 0; i<rows.size(); i++){
	// 	//cerr << rows[i] << "," << cols[i] << endl;
	// }
	
}
class block{
public:
	int value;
	int iMax;
	int iMin;
	int jMax;
	int jMin;
	bool b;
	block(){
		b = false;
	}
	block(int v, int i, int j){
		value = v;
		iMax = i;
		iMin = i;
		jMax = j;
		jMin = j;
		b = true;
	}
};
vector<int> Board::centerEvaluatorDP1(int playerNo,int weightForThreat)
{
	block l[this->dimension*this->dimension];
	block* values[this->dimension][this->dimension];
	int max=0;


	int potentialT = 100;
	int threatW = 300;
	// int i_max = 0;
	// int j_max = 0;
	for(int i=0;i<this->dimension;i++){
		for(int j=0;j<this->dimension;j++){
			vector<string> v = this->board[i][j];
			if (i==0 && j==0){
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					l[this->dimension*i + j] = block(1,i,j);
					values[i][j] = &(l[this->dimension*i + j]);
					max = 1;
				}	
				else{
					l[this->dimension*i + j] = block(0,i,j);
					values[i][j] = &(l[this->dimension*i + j]);
				}	
			}
			else if (i==0){
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					if (values[i][j-1]->value!=0){
						values[i][j-1]->value += 1;
						values[i][j] = values[i][j-1];
						if (values[i][j-1]->iMin>i)
							values[i][j-1]->iMin = i;
						if (values[i][j-1]->jMin>j)
							values[i][j-1]->jMin = j;
						if (values[i][j-1]->iMax<i)
							values[i][j-1]->iMax = i;
						if (values[i][j-1]->jMax<j)
							values[i][j-1]->jMax = j;

						if (values[i][j]->value >= max){
							max = values[i][j]->value;
						}
					}
					else{
						l[this->dimension*i + j] = block(1,i,j);
						values[i][j] = &l[this->dimension*i + j];
						if (values[i][j]->value >= max){
							max = values[i][j]->value;
							// i_max = i;
							// j_max = j;
						}
					}	
				}	
				else{
					l[this->dimension*i + j] = block(0,i,j);
					values[i][j] = &l[this->dimension*i + j];
				}	
			}
			else if (j==0){
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					if (values[i-1][j]->value!=0){
						values[i-1][j]->value += 1;
						values[i][j] = values[i-1][j];
						if (values[i-1][j]->iMin>i)
							values[i-1][j]->iMin = i;
						if (values[i-1][j]->jMin>j)
							values[i-1][j]->jMin = j;
						if (values[i-1][j]->iMax<i)
							values[i-1][j]->iMax = i;
						if (values[i-1][j]->jMax<j)
							values[i-1][j]->jMax = j;

						if (values[i][j]->value >= max){
							max = values[i][j]->value;
							// i_max = i;
							// j_max = j;
						}
					}
					else{
						l[this->dimension*i + j] = block(1,i,j);
						values[i][j] = &l[this->dimension*i + j];
						if (values[i][j]->value >= max){
							max = values[i][j]->value;
							// i_max = i;
							// j_max = j;
						}
					}
				}	
				else{
					l[this->dimension*i + j] = block(0,i,j);
					values[i][j] = &l[this->dimension*i + j];
				}	
			}
			else{
				if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
					if (values[i][j-1]->value!=0 && values[i-1][j]->value!=0){
						if (values[i][j-1] == values[i-1][j]){
							if (values[i-1][j]->iMin>i)
								values[i-1][j]->iMin = i;
							if (values[i-1][j]->jMin>j)
								values[i-1][j]->jMin = j;
							if (values[i-1][j]->iMax<i)
								values[i-1][j]->iMax = i;
							if (values[i-1][j]->jMax<j)
								values[i-1][j]->jMax = j;
							values[i-1][j]->value += 1;
							values[i][j] = values[i-1][j];
							if (values[i][j]->value >= max){
								max = values[i][j]->value;
								// i_max = i;
								// j_max = j;
							}
						}
						else{
							values[i-1][j]->value += 1 + values[i][j-1]->value;
							values[i][j-1] = values[i-1][j];
							values[i][j] = values[i-1][j];
							if (values[i-1][j]->iMin>i)
								values[i-1][j]->iMin = i;
							if (values[i-1][j]->jMin>j)
								values[i-1][j]->jMin = j;
							if (values[i-1][j]->iMax<i)
								values[i-1][j]->iMax = i;
							if (values[i-1][j]->jMax<j)
								values[i-1][j]->jMax = j;
							if (values[i][j]->value >= max){
								max = values[i][j]->value;
								// i_max = i;
								// j_max = j;
							}
						}
					}
					else if (values[i][j-1]->value==0 && values[i-1][j]->value!=0){
						values[i-1][j]->value += 1;
						values[i][j] = values[i-1][j];
						if (values[i-1][j]->iMin>i)
							values[i-1][j]->iMin = i;
						if (values[i-1][j]->jMin>j)
							values[i-1][j]->jMin = j;
						if (values[i-1][j]->iMax<i)
							values[i-1][j]->iMax = i;
						if (values[i-1][j]->jMax<j)
							values[i-1][j]->jMax = j;
						if (values[i][j]->value >= max){
							max = values[i][j]->value;
							// i_max = i;
							// j_max = j;
						}
					}
					else if (values[i][j-1]->value!=0 && values[i-1][j]->value==0){
						values[i][j-1]->value += 1;
						values[i][j] = values[i][j-1];
						if (values[i][j-1]->iMin>i)
							values[i][j-1]->iMin = i;
						if (values[i][j-1]->jMin>j)
							values[i][j-1]->jMin = j;
						if (values[i][j-1]->iMax<i)
							values[i][j-1]->iMax = i;
						if (values[i][j-1]->jMax<j)
							values[i][j-1]->jMax = j;
						if (values[i][j]->value >= max){
							max = values[i][j]->value;
							// i_max = i;
							// j_max = j;
						}
					}
					else{
						l[this->dimension*i + j] = block(1,i,j);
						values[i][j] = &l[this->dimension*i + j];
						if (values[i][j]->value >= max){
							max = values[i][j]->value;
							// i_max = i;
							// j_max = j;
						}
					}	
				}
				else{
					l[this->dimension*i + j] = block(0,i,j);
					values[i][j] = &l[this->dimension*i + j];
				}
			}
			v.clear();
		}
	}
	vector<int> v;
	int score = 0;		////Number of such threats
	int threat = 0;		////Cost of threat
	for (int i=0; i<(this->dimension*this->dimension); i++){
		if (l[i].b){
			// cout << i << "_" << l[i].value << "_" << l[i].iMin << "_" << l[i].iMax << "_" << l[i].jMin << "_" << l[i].jMax << endl;
			if ((l[i].value > this->dimension-2) && (l[i].iMax-l[i].iMin >= this->dimension-2 || l[i].jMax-l[i].jMin >= this->dimension-2)){
				score = MAXV/100;
				threat = MAXV/100;
				v.push_back(score);
				v.push_back(threat);
				return v;
			}
			else if ((l[i].value == this->dimension-2) && (l[i].iMax-l[i].iMin >= this->dimension-2 || l[i].jMax-l[i].jMin >= this->dimension-2)){
				score += 1;
				threat += threatW * (this->dimension-2);
			}
		}
	}
	// 	cerr << "Values : \n";
	// 	for (int i = 0; i<this->dimension; i++){
	// 		for (int j=0; j<this->dimension; j++){
	// 			cerr << values[i][j]->value << " ";
	// 		}
	// 		cerr << "\n";
	// }

	v.push_back(score);
	v.push_back(threat);
	return v;
}

int Board::evaluate(int playerNo)
{

	////cerr << "Yo\n";
	// if (this->checkRoadWin(playerNo))
	// 	return INT_MAX;
//---------------------------------------------------------------------------------------------------
	int commonweight = 200;
	// int weightForFlat=20;
	// int weightForCaptivesSelf=15;
	// int weightForCaptivesNSelf=5;
	// int weightForWall=20; 
	// int weightForStackInternal=25;
	// int weightForStackExternal=10;
	// int weightForThreat=10;
	// int weightForCap=10;
	// int selfStackingPenalty=36;
	
	int weightForFlat=600;
	int standingW = 200;
	int putStand = 100;
	int capW = 300;
	int flatCapW = 200;
	int standCapW = 300;
	int capCapW = 250;
	int potentialT = 100;
	int threatW = 300;

	int weightForCaptivesSelf=15;
	int weightForCaptivesNSelf=5;
	int weightForWall=20; 
	int weightForStackInternal=25;
	int weightForStackExternal=10;
	int weightForThreat=10;
	int weightForCap=10;
	int selfStackingPenalty=400;
	

	// int weightForFlat=commonweight;
	// int weightForCaptivesSelf=commonweight;
	// int weightForCaptivesNSelf=commonweight;
	// int weightForWall=commonweight; 
	// int weightForStackInternal=commonweight;
	// int weightForStackExternal=commonweight;
	// int weightForThreat=commonweight;
	// int weightForCap=commonweight;
	// int selfStackingPenalty=commonweight;
	

	int score = 0;
	
	if(checkRoadWin(playerNo))
	{
		score+=MAXV;
		return score;
	}
	if (checkRoadWin(1-playerNo)){
		score-=MAXV;
		return score;
	}
//---------------------------------------------------------------------------------------------------
	
	// CASE1

	// int weightForFlat=10;
	// int weightForCaptivesSelf=15;
	// int weightForCaptivesNSelf=5;
	// int weightForWall=200; 
	// int weightForStackInternal=250;
	// int weightForStackExternal=100;
	// int weightForThreat=10;
	// int weightForCap=10;
	// int selfStackingPenalty=360;
	// int score = 0;

	// if(checkRoadWin(playerNo))
	// {
	// 	score+=INT_MAX/2;
	// 	return score;
	// }
	// if (checkRoadWin(1-playerNo)){
	// 	score-=INT_MAX/2;
	// 	return score;
	// }


	// int finalThreatWeight = centerEvaluatorDP(1-playerNo,weightForThreat);
	// score+=centerEvaluatorDP(playerNo,weightForThreat);
	// score-=finalThreatWeight;

	//cerr<<"centerEvaluatorDP1(playerNo,0)"<<endl;
	vector<int> vec1 = centerEvaluatorDP1(playerNo,0);
	//cerr<<"centerEvaluatorDP1(playerNo,0) COMPLETED"<<endl;

	//cerr<<"centerEvaluatorDP1(1-playerNo,0)"<<endl;
	vector<int> vec2 = centerEvaluatorDP1(1-playerNo,0);
	//cerr<<"centerEvaluatorDP1(1-playerNo,0) COMPLETED"<<endl;

	score+=(vec1[1]);
	score-=(vec2[1]);

	int dim = this->dimension;
	for (int i=0; i<this->dimension; i++){
		for (int j=0; j<this->dimension; j++)
		{
			vector<string> v = this->board[i][j];
			if (!v.empty() && v.back()[0]==char('0'+(playerNo+1))){
				
				// if(v.size()>2 and v[v.size()-2][0]==v.back()[0])
				// 	score-=selfStackingPenalty;
				
				if (v.back()[2]=='F')
				{
					score+=weightForFlat;
					if(v.size()>1)
						if(not( ( (i==0 or i==dim-1) and (j>=0 and j<=dim-1) ) or ( (j==0 or j==dim-1) and (i>=1 and i<=dim-2) ) ))
							score+=flatCapW;
						else if( ( (i==0 or i==dim-1) and (j>=0 and j<=dim-1) ) or ( (j==0 or j==dim-1) and (i>=1 and i<=dim-2) ) )
							score+=(flatCapW*7)/10;
				} 
				else if (v.back()[2]=='C'){
					// if(v.size()>=2)
					// {
					// 	////cerr<<"finalThreatWeight: "<<finalThreatWeight<<endl;
					// 	// if(finalThreatWeight<((this->dimension-1)*9))
					// 	// 	score-= (finalThreatWeight!=0?finalThreatWeight:100);
					// 	// else
					// 	// 	score += finalThreatWeight;
					// 	// //cerr<<"score: "<<score<<endl;
					// }
					// else
					// {
					int countSelf=0;
					
						for (int k=0; k<v.size(); k++){
							if (v[k][0] == char('0'+(playerNo+1)))
							{
								//score+=weightForCaptivesSelf;
								score+=flatCapW;
								countSelf++;
							}
						}				

						if(v.size()>1)
							if(not( ( (i==0 or i==dim-1) and (j>=0 and j<=dim-1) ) or ( (j==0 or j==dim-1) and (i>=1 and i<=dim-2) ) ))
								score+=capCapW*countSelf;
							else if( ( (i==0 or i==dim-1) and (j>=0 and j<=dim-1) ) or ( (j==0 or j==dim-1) and (i>=1 and i<=dim-2) ) )
								score+=capCapW*countSelf;
					//}
				}
				else if (v.back()[2]=='S')
				{
//					score += standingW;
					int countSelf=0;
					if(v.size()==1)
					{
						score+=putStand;
						continue;

					}
					for (int k=0; k<v.size(); k++){
							if (v[k][0] == char('0'+(playerNo+1)))
							{
								//score+=weightForCaptivesSelf;
								score+=standingW;
								countSelf++;
							}
						}
				}
				else
					continue;
			}
			if (!v.empty() && v.back()[0]==char('0'+(2-playerNo))){
				if (v.back()[2]=='C'){
					//for (int k=0; k<v.size(); k++){
						//if (v[k][0] == char('0'+(playerNo+1)))
						score-= v.size()*capCapW;
					//}
				}
				else if (v.back()[2]=='S')
					score -= v.size()*standingW;
				else if(v.back()[2]=='F')
					score -=v.size()*weightForFlat;
				else
					continue;
			}
			


			v.clear();
		}
	}
	return score;
	//cerr << "Yo1\n";
	// return 1;
}		

///////////////Correct

vector<int> Board::minimax(Board board1, int depth, bool maxNode, int alpha, int beta, int playerNo, int d)
{	///// v[0] = bestIndex   v[1] = bestvalue
	///Assume depth of 4
	vector<int> v;
	int bestMoveI = 0;
	if (depth == d){
		int best = 0;
		if (depth%2==1)
			best = board1.evaluate(1-playerNo);
		else
			best = board1.evaluate(playerNo);
		v.push_back(0);
		v.push_back(best);
		return (v);
	}
	vector<string> validMoves = board1.getValidMoves(playerNo);
	if (maxNode)
	{
		// //cerr << "Here1" << validMoves.size();
		int best = INT_MIN;
		bool b = true;
		for (int i=0; i < validMoves.size(); i++)
		{
			Board boardTemp(board1) ;
			boardTemp.makeMove(playerNo,validMoves[i]);
			int value = minimax(boardTemp, depth+1, false, alpha, beta, 1-playerNo,d)[1];
			if (value >= 1000){
				bestMoveI = i;
				best = value;
			}
			// b = b and (this->roadBool);
			// if (!this->roadBool){
			if (best < value){
				bestMoveI = i;
				best = value;
			}
			
			alpha = std::max(alpha, best);

			if (beta <= alpha){
				// //cerr << "kl";
				break;
			}
		}
		v.push_back(bestMoveI);
		v.push_back(best);
		return v;
		// if (b){
		// 	if (depth == 0){
		// 		this->roadBool = false;
		// 	}
		// 	return best;	
		// }
		// else{
		// 	this->roadBool = false;
		// 	return best;
		// }
	}
	else
	{
		int best = INT_MAX;
		// //cerr << "Here-1" << validMoves.size();
		for (int i=0; i< validMoves.size(); i++)
		{
			Board boardTemp(board1) ;
			boardTemp.makeMove(playerNo,validMoves[i]);
			int value = minimax(boardTemp, depth+1, true, alpha, beta,1-playerNo,d)[1];
			// if (value == -1000){
			// 	this->roadBool = true;
			// }
			// if (!this->roadBool){
			if (value <= -1000){
				bestMoveI = i;
				best = value;
			}
			if (best > value){
				bestMoveI = i;
				best = value;
			}
			// }	
			beta = std::min(beta, best);

			if (beta <= alpha){
				// //cerr << "LK";
				break;
			}
		}
		v.push_back(bestMoveI);
		v.push_back(best);
		return v;
	}
	//validMoves.clear();
}


int Board::minimax_iter(Board board1, int depth, bool maxNode, int alpha, int beta, int playerNo, int i){
	int besMove_ = 0;
	int max = INT_MIN;
	vector<int> n = this->minimax(board1, depth, maxNode, alpha, beta, playerNo, 1);
	if (n[1]>=1000)
		return n[0];
	n = this->minimax(board1, depth, maxNode, alpha, beta, playerNo, i);
	vector<int> n1 = this->minimax(board1, depth, maxNode, alpha, beta, playerNo, 2);
	// cerr << "Score : " << n[1] << endl;
	// cerr << "Score1 : " << n1[1] << endl;
	if (n[1] >= n1[1])
		return n[0];
	else
		return n1[0];
	////cerr << "Score : " << max << endl;
	// return besMove_;
}

struct Node{
	Node *children;
	int value;
	int noOfChildren;
	string move;
	bool flatten;

	Node(){
		value = -1;
		noOfChildren = 0;
		move = "";
		flatten = false;
	}

	Node(int val,int n, string mov, bool b){
		value = val;
		noOfChildren = n;
		children = new Node[noOfChildren];
		move = mov;
		flatten = b;
	}

	Node(int val,int n){
		value = val;
		noOfChildren = n;
		move = "";
		flatten = false;
	}

	Node(int val){
		value = val;
	}

	void listInit(int n){
		noOfChildren = n;
		children = new Node[noOfChildren];
	}

	friend bool operator < (const Node& a, const Node& b){
		return a.value<b.value;
	}

	void msort(){
		sort(children, children+noOfChildren);
	}

	void deleteNode();
	string getBestMove(Board &board, int depth, int playerNo);
	void makeCompleteTree(Board &board, int maxDepth, int playerNo);
	void makeTree(Board &board, int maxDepth, int depth, int playerNo);
	void makeCompleteMinimaxTree(Board &board, int maxDepth, int playerNo);
	void makeMinimaxTree(Board &board, int maxDepth, int depth, int playerNo, int alpha, int beta,int currVal);
	void extendTree(Board &board, bool isMax, int playerNo);
};

struct NodeComparator{
	inline bool operator() (const Node& node1, const Node& node2)
	{
		if(node1.value!=node2.value)
			return node1.value<node2.value ;
		else
		{
			cerr<<"SAME VALUE MOTHERFUCKER!!!!!!!!!!!!!!"<<endl;
			if(isdigit(node1.move[0]))	
			{
				return true;
			}
			else
			{
				if(isdigit(node2.move[0]))
					return false;
				else
				{
					if(node1.move[0]=='F')
						return false;
					else
						if(node2.move[0]=='F')
							return true;
						else
							return true;

				}
			}

		}
	}
};

void Node::deleteNode(){
	for (int i = 0; i != this->noOfChildren; ++i)
		this->children[i].deleteNode();
	delete [] this->children;
	delete this;
}

// string Node::getBestMove(Board &board, int depth, int playerNo){
// 	int valueOfRoot = board.evaluate(playerNo);
// 	Node node = Node(valueOfRoot, 0, "", false);
// 	node.makeCompleteTree(board, depth, playerNo);
// 	//string move = minimax(node, playerNo);
// 	string move = node.children[node.noOfChildren-1].move;
// 	return move;
// }

void Node::makeCompleteTree(Board &board, int maxDepth, int playerNo){
	for (int i=1; i<=maxDepth; i++){
		this->makeTree(board, i, 0, playerNo);
	}
}

void Node::makeTree(Board &board, int maxDepth, int depth, int playerNo){
	if (depth == maxDepth){
		return;
	}
	else{			/////go on
		if (this->noOfChildren == 0){
			int score = this->value;
			if (score == 1000 || score == -1000)		////// In case of a win
				return;
			else{
				vector<string> validMoves = board.getValidMoves(playerNo);
				this->noOfChildren = validMoves.size();
				this->listInit(noOfChildren);
				for (int i=0; i<noOfChildren; i++){
					bool b = false;
					board.makeMove(playerNo,validMoves[i],b);
					if (depth%2==0)
						this->children[i] = Node(board.evaluate(playerNo),0,validMoves[i],b);
					else
						this->children[i] = Node(board.evaluate(1-playerNo),0,validMoves[i],b);
					board.undoMove(playerNo,validMoves[i],b);
				}
				this->msort();
				if (depth%2==0){		///// Max Node
					if (this->value>this->children[noOfChildren-1].value)
						this->value = this->children[noOfChildren-1].value;
				}
				else{					///// Min Node
					if (this->value<this->children[noOfChildren-1].value)
						this->value = this->children[noOfChildren-1].value;
				}
			}
		}
		else{
			for (int i=0; i<noOfChildren; i++){
				board.makeMove(playerNo, this->children[i].move,this->children[i].flatten);
				this->children[i].makeTree(board, maxDepth, depth+1, 1-playerNo);
				board.undoMove(playerNo, this->children[i].move,this->children[i].flatten);	
			}
			this->msort();
			if (depth%2==0){		///// Max Node
				if (this->value>this->children[noOfChildren-1].value)
					this->value = this->children[noOfChildren-1].value;
			}
			else{					///// Min Node
				if (this->value<this->children[noOfChildren-1].value)
					this->value = this->children[noOfChildren-1].value;
			}
		}
	}
}

void Node::makeCompleteMinimaxTree(Board &board, int maxDepth, int playerNo){
	double currTime = ::TIME;
	double TOTALTIME = (board.dimension-3)*60;
	double remTime = TOTALTIME-currTime ;
	int currValue = this->value;
	for (int i=1; i<=maxDepth; i++){
		clock_t t = clock();
		this->makeMinimaxTree(board, i, 0, playerNo, INT_MIN, INT_MAX,currValue);
		double timeconsumed = (double)(clock()-t)/((double)CLOCKS_PER_SEC);
		remTime -= timeconsumed;
		cerr<<"AVERAGE MOVES REMAINING: "<<(int)(remTime/::AVGTIME)<<endl;
		cerr<<"MAXDEPTH: "<<i<<endl;
		for (int j=0; j<this->noOfChildren; j++){
			cerr << this->children[j].move << " -> " << this->children[j].value << endl;
		}
		if( (int)(remTime/::AVGTIME)<10 && i==3)
		{
			cerr<<"BREAKING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
			break;
		}	
	}
}

// void Node::makeMinimaxTree(Board &board, int maxDepth, int depth, int playerNo, int alpha, int beta){
// 	if (depth == maxDepth){
// 		return;
// 	}
// 	else{			/////go on
// 		if (this->noOfChildren == 0){
// 			// int score = this->value;
// 			// if (score == 1000 || score == -1000)		////// In case of a win
// 			// 	return;
// 			// else{
// 				vector<string> validMoves = board.getValidMoves(playerNo);
// 				this->noOfChildren = validMoves.size();
// 				this->listInit(noOfChildren);
// 				for (int i=0; i<noOfChildren; i++){
// 					bool b = false;
// 					board.makeMove(playerNo,validMoves[i],b);
// 					if (depth%2==0)
// 						this->children[i] = Node(board.evaluate(playerNo),0,validMoves[i],b);
// 					else
// 						this->children[i] = Node(board.evaluate(1-playerNo),0,validMoves[i],b);
// 					board.undoMove(playerNo,validMoves[i],b);
// 				}
// 				this->msort();
// 				if (depth%2==0){		///// Max Node
// 					this->value = INT_MIN;
// 					for (int i=noOfChildren-1; i>=0; i--){
// 						bool b = false;
// 						board.makeMove(playerNo,this->children[i].move,b);
// 						// this->children[i] = Node(board.evaluate(playerNo),0,this->children[i].move,b);
// 						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 						board.undoMove(playerNo,this->children[i].move,b);
// 						if (depth == 2 && this->children[i].value <= -MAXV/3){
// 							this->value = this->children[i].value/2;
// 							return;
// 						}
// 						if (this->value < this->children[i].value)
// 							this->value = this->children[i].value;
// 						alpha = std::max(alpha,this->value);
// 						if (beta <= alpha){
// 							// cerr << "ijcsnkjs : " << i << " " << depth << " " << this->children[i].move << endl;
// 							break;
// 						}
// 					}
// 				}
// 				else{					///// Min Node
// 					this->value = INT_MAX;
// 					for (int i=0; i<noOfChildren; i++){
// 						bool b = false;
// 						board.makeMove(playerNo,this->children[i].move,b);
// 						// this->children[i] = Node(board.evaluate(1-playerNo),0,this->children[i].move,b);
// 						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 						board.undoMove(playerNo,this->children[i].move,b);
// 						if (this->value > this->children[i].value)
// 							this->value = this->children[i].value;
// 						beta = std::min(beta,this->value);
// 						if (beta <= alpha){
// 							// cerr << "ijdsdfdds : " << i << " " << depth << " " << this->children[i].move << endl;
// 							break;
// 						}
// 					}
// 				}
// 				this->msort();
// 			// }
// 		}
// 		else{
// 			if (depth%2==0){		///// Max Node
// 				this->value = INT_MIN;
// 				for (int i=noOfChildren-1; i>=0; i--){
// 					if (this->children[i].value <= -MAXV/3)
// 						continue;
// 					else{		
// 						bool b = false;
// 						board.makeMove(playerNo,this->children[i].move,b);
// 						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 						board.undoMove(playerNo,this->children[i].move,b);
// 						if (depth == 2 && this->children[i].value <= -MAXV/3){
// 							this->value = this->children[i].value/2;
// 							return;
// 						}
// 						if (this->value < this->children[i].value)
// 							this->value = this->children[i].value;
// 						alpha = std::max(alpha,this->value);
// 						if (beta <= alpha){
// 							// cerr << "sadsfds : " << i << " " << depth << " " << this->children[i].move << endl;
// 							break;
// 						}
// 					}	
// 				}
// 			}
// 			else{					///// Min Node
// 				this->value = INT_MAX;
// 				for (int i=0; i<noOfChildren; i++){
// 					bool b = false;
// 					board.makeMove(playerNo,this->children[i].move,b);
// 					this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 					board.undoMove(playerNo,this->children[i].move,b);
// 					if (this->value > this->children[i].value)
// 						this->value = this->children[i].value;
// 					beta = std::min(beta,this->value);
// 					if (beta <= alpha){
// 						// cerr << "jcsdnkjn : " << i << " " << depth << " " << this->children[i].move << endl;
// 						break;
// 					}
// 				}
// 			}
// 			this->msort();
// 		}
// 	}
// }

void Node::makeMinimaxTree(Board &board, int maxDepth, int depth, int playerNo, int alpha, int beta, int currVal){
	if (depth == maxDepth){
		return;
	}
	else{			/////go on
		if (this->noOfChildren == 0){
			// int score = this->value;
			// if (score == 1000 || score == -1000)		////// In case of a win
			// 	return;
			// else{
				vector<string> validMoves = board.getValidMoves(playerNo);
				this->noOfChildren = validMoves.size();
				this->listInit(noOfChildren);
				for (int i=0; i<noOfChildren; i++){
					bool b = false;
					board.makeMove(playerNo,validMoves[i],b);
					if (depth%2==0)
						this->children[i] = Node(board.evaluate(playerNo),0,validMoves[i],b);
					else
						this->children[i] = Node(board.evaluate(1-playerNo),0,validMoves[i],b);
					board.undoMove(playerNo,validMoves[i],b);
				}
				this->msort();
				if (depth%2==0){		///// Max Node
					bool ab = false;
					this->value = INT_MIN;
					for (int i=noOfChildren-1; i>=0; i--){
						bool b = false;
						board.makeMove(playerNo,this->children[i].move,b);
						// this->children[i] = Node(board.evaluate(playerNo),0,this->children[i].move,b);
						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta, currVal);
						board.undoMove(playerNo,this->children[i].move,b);
						if (depth == 2 && this->children[i].value <= -1 *(MAXV/3)){
							ab = true;
							// this->value = this->children[i].value/2;
							// return;
						}
						if (this->value < this->children[i].value)
							this->value = this->children[i].value;
						alpha = std::max(alpha,this->value);
						if (beta <= alpha){
							// cerr << "ijcsnkjs : " << i << " " << depth << " " << this->children[i].move << endl;
							break;
						}
					}
					if ((currVal > this->value)  && ab){
						this->value = -1*(MAXV/4);
					}
				}
				else{					///// Min Node
					this->value = INT_MAX;
					for (int i=0; i<noOfChildren; i++){
						bool b = false;
						board.makeMove(playerNo,this->children[i].move,b);
						// this->children[i] = Node(board.evaluate(1-playerNo),0,this->children[i].move,b);
						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta, currVal);
						board.undoMove(playerNo,this->children[i].move,b);
						if (this->value > this->children[i].value)
							this->value = this->children[i].value;
						beta = std::min(beta,this->value);
						if (beta <= alpha){
							// cerr << "ijdsdfdds : " << i << " " << depth << " " << this->children[i].move << endl;
							break;
						}
					}
				}
				this->msort();
			// }
		}
		else{
			if (depth%2==0){		///// Max Node
				bool ab = false;
				this->value = INT_MIN;
				for (int i=noOfChildren-1; i>=0; i--){
					if (this->children[i].value <= -1 * (MAXV/3))
						continue;
					else{		
						bool b = false;
						board.makeMove(playerNo,this->children[i].move,b);
						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta, currVal);
						board.undoMove(playerNo,this->children[i].move,b);
						if (depth == 2 && this->children[i].value <= -1 * INT_MAX/2){
							ab = true;
							// this->value = this->children[i].value/2;
							// return;
						}
						if (this->value < this->children[i].value)
							this->value = this->children[i].value;
						alpha = std::max(alpha,this->value);
						if (beta <= alpha){
							// cerr << "sadsfds : " << i << " " << depth << " " << this->children[i].move << endl;
							break;
						}
					}	
				}
				if ((currVal > this->value)  && ab){
					this->value = -1*(MAXV/4);
				}
			}
			else{					///// Min Node
				this->value = INT_MAX;
				for (int i=0; i<noOfChildren; i++){
					bool b = false;
					board.makeMove(playerNo,this->children[i].move,b);
					this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta, currVal);
					board.undoMove(playerNo,this->children[i].move,b);
					if (this->value > this->children[i].value)
						this->value = this->children[i].value;
					beta = std::min(beta,this->value);
					if (beta <= alpha){
						// cerr << "jcsdnkjn : " << i << " " << depth << " " << this->children[i].move << endl;
						break;
					}
				}
			}
			this->msort();
		}
	}
}

// void Node::makeMinimaxTree(Board &board, int maxDepth, int depth, int playerNo, int alpha, int beta){
// 	if (depth == maxDepth){
// 		return;
// 	}
// 	else{			/////go on
// 		if (this->noOfChildren == 0){
// 			// int score = this->value;
// 			// if (score == 1000 || score == -1000)		////// In case of a win
// 			// 	return;
// 			// else{
// 				vector<string> validMoves = board.getValidMoves(playerNo);
// 				this->noOfChildren = validMoves.size();
// 				this->listInit(noOfChildren);
// 				for (int i=0; i<noOfChildren; i++){
// 					bool b = false;
// 					board.makeMove(playerNo,validMoves[i],b);
// 					if (depth%2==0)
// 						this->children[i] = Node(board.evaluate(playerNo),0,validMoves[i],b);
// 					else
// 						this->children[i] = Node(board.evaluate(1-playerNo),0,validMoves[i],b);
// 					board.undoMove(playerNo,validMoves[i],b);
// 				}
// 				this->msort();
// 				if (depth%2==0){		///// Max Node
// 					this->value = INT_MIN;
// 					for (int i=noOfChildren-1; i>=0; i--){
// 						bool b = false;
// 						board.makeMove(playerNo,this->children[i].move,b);
// 						this->children[i] = Node(board.evaluate(playerNo),0,this->children[i].move,b);
// 						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 						board.undoMove(playerNo,this->children[i].move,b);
// 						if (this->value < this->children[i].value)
// 							this->value = this->children[i].value;
// 						alpha = std::max(alpha,this->value);
// 						if (beta <= alpha || this->value == 1000){
// 							// cerr << "ijcsnkjs : " << i << " " << depth << " " << this->children[i].move << endl;
// 							break;
// 						}
// 					}
// 				}
// 				else{					///// Min Node
// 					this->value = INT_MAX;
// 					for (int i=0; i<noOfChildren; i++){
// 						bool b = false;
// 						board.makeMove(playerNo,this->children[i].move,b);
// 						this->children[i] = Node(board.evaluate(1-playerNo),0,this->children[i].move,b);
// 						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 						board.undoMove(playerNo,this->children[i].move,b);
// 						if (this->value > this->children[i].value)
// 							this->value = this->children[i].value;
// 						beta = std::min(beta,this->value);
// 						if (beta <= alpha){
// 							// cerr << "ijdsdfdds : " << i << " " << depth << " " << this->children[i].move << endl;
// 							break;
// 						}
// 					}
// 				}
// 				this->msort();
// 			// }
// 		}
// 		else{
// 			if (depth%2==0){		///// Max Node
// 				this->value = INT_MIN;
// 				for (int i=noOfChildren-1; i>=0; i--){
// 					if(this->children[i].value<=(-1)*INT_MAX/3)
// 						continue;
					
// 					else{
// 						bool b = false;
// 						board.makeMove(playerNo,this->children[i].move,b);
// 						this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 						board.undoMove(playerNo,this->children[i].move,b);
// 						if (this->value < this->children[i].value)
// 							this->value = this->children[i].value;
// 						alpha = std::max(alpha,this->value);
// 						if (beta <= alpha){
// 							// cerr << "sadsfds : " << i << " " << depth << " " << this->children[i].move << endl;
// 							break;
// 						}
// 					}
// 				}
// 			}
// 			else{					///// Min Node
// 				this->value = INT_MAX;
// 				for (int i=0; i<noOfChildren; i++){
// 					bool b = false;
// 					board.makeMove(playerNo,this->children[i].move,b);
// 					this->children[i].makeMinimaxTree(board, maxDepth, depth+1, 1-playerNo, alpha, beta);
// 					board.undoMove(playerNo,this->children[i].move,b);
// 					if (this->value > this->children[i].value)
// 						this->value = this->children[i].value;
// 					beta = std::min(beta,this->value);
// 					if (beta <= alpha){
// 						// cerr << "jcsdnkjn : " << i << " " << depth << " " << this->children[i].move << endl;
// 						break;
// 					}
// 				}
// 			}
// 			this->msort();
// 		}
// 	}
// }
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
		int valueOfRoot = this->board.evaluate(currentPiece);
		Node node = Node(valueOfRoot, 0, "", false);
		
		if (this->dimension==5){
			if ((this->board.listOfPlayers[0].flatStones + this->board.listOfPlayers[0].capStones >= 18) && (this->board.listOfPlayers[1].flatStones + this->board.listOfPlayers[1].capStones >= 18))
				node.makeCompleteMinimaxTree(board, 2, currentPiece);
			else
				node.makeCompleteMinimaxTree(board, 4, currentPiece);
		}
		else if (this->dimension==6){
			if ((this->board.listOfPlayers[0].flatStones + this->board.listOfPlayers[0].capStones >= 26) && (this->board.listOfPlayers[1].flatStones + this->board.listOfPlayers[1].capStones >= 26))
				node.makeCompleteMinimaxTree(board, 2, currentPiece);
			else
				node.makeCompleteMinimaxTree(board, 4, currentPiece);
		}
		else{
			if ((this->board.listOfPlayers[0].flatStones + this->board.listOfPlayers[0].capStones >= 35) && (this->board.listOfPlayers[1].flatStones + this->board.listOfPlayers[1].capStones >= 35))
				node.makeCompleteMinimaxTree(board, 2, currentPiece);
			else
				node.makeCompleteMinimaxTree(board, 3, currentPiece);
		}
		
		//node.makeCompleteMinimaxTree(board, 4, currentPiece);
		
		// for (int i=0; i<node.noOfChildren; i++){
		// 	cerr << node.children[i].move << " -> " << node.children[i].value << endl;
		// }
		string bestMove = node.children[node.noOfChildren-1].move;
		cerr<<"Move chosen: "<<bestMove<<" Value: "<<node.children[node.noOfChildren-1].value<<endl;
		// node.deleteNode();
		this->board.makeMove(currentPiece, bestMove);
		this->currTurnNo = 1 - this->currTurnNo;
		this->board.printBoard();
		return bestMove;
	}
	else{
		currentPiece = 1 - this->currTurnNo;
		if (this->board.board[0][0].empty()){
			string bestMove = "Fa1";
			this->board.makeMove(currentPiece, bestMove);
			this->currTurnNo = 1 - this->currTurnNo;
			this->board.printBoard();
			// ////cerr << "Board after " << bestMove << " by Player " << currentPiece+1 << " :\n";
			// this->board.printBoard();
			// this->board.centerEvaluatorDP(currentPiece);
			return bestMove;
		}
		else{	
			string bestMove = "Fa" ;
			bestMove += std::to_string(this->dimension);
			this->board.makeMove(currentPiece, bestMove);
			this->currTurnNo = 1 - this->currTurnNo;
			this->board.printBoard();
			// ////cerr << "Board after " << bestMove << " by Player " << currentPiece+1 << " :\n";
			// this->board.printBoard();
			// this->board.centerEvaluatorDP(currentPiece);
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
	////cerr << "Board after " << move << " by Player " << currentPiece+1 << " :\n";
	//this->board.printBoard();
	//this->board.centerEvaluatorDP(currentPiece);
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
	double average = 0.0001;
	::AVGTIME = average;
	int count = 0;
	while(true)
	{
		//making our move
		
		clock_t start = clock();
		string moveChosen = this->game.getBestMove();//this->playerNo);
		double a = (clock()-start)/(double(CLOCKS_PER_SEC));
		
		average = 1.0*(count*average + a)/(count+1);
		count++;
		cerr<<"Time taken for move: "<<a<<endl;	
		::TIME += double(a);
		cerr<<::TIME<<endl;
		cerr<<"Average Time taken: "<<average<<endl;
		::AVGTIME = average;
		moveChosen = moveChosen + '\n';
		cout << moveChosen;
		string move;
		cin >> move;
		if(move=="")
		{
			////cerr<<"EXITING";
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
