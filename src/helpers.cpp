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
	else if(top<this->n)
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
			vector<string> currVec = this->board[i*this->dimension][0];
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
		int top = dfsStack.pop_back();
		if(finalPos.find(top)!=finalPos.end())
		{
			return true;
		}
		vector<int> ngbr= neighbours(top);
		for(int i=0;i<ngbr.size();i++)
		{
			int currRow = (ngbr[i]%this->dimension==0?ngbr[i]/this->dimension-1:ngbr[i]/this->dimension);
			int currCol = (ngbr[i]%this->dimension==0?this->dimension-1:ngbr[i]%this->dimension-1);
			if( visited.find(ngbr[i])==visited.end() and this->board[currRow][currCol].size()>0 and this->board[currRow][currCol][this->board[currRow][currCol].size()-1][0]==char(playerNo+49) and this->board[currRow][currCol][this->board[currRow][currCol].size()-1][0][2]!='S')
			{
				dfsStack.push_back(ngbr[i]);
				visited.insert(ngbr);	
			}
		} 	
	}

	return false;
}
bool Board::checkRoadWin(int playerNo)
{
	return checkRoadWin(playerNo,"Horizontal") or checkRoadWin(playerNo,"Vertical");
}	