#include<iostream>
#include<random>
#include<vector>
#include<algorithm>

using namespace std;


vector<int> find_maximal_span(vector<vector<bool> > AdjMatrix, int vertex)  /* Given an adjacent matrix and a vertex, find the list of all vertices that are connected to it with a path. */
{
	int max_size = AdjMatrix.size();
	if ((vertex < 0) || (vertex > max_size))
	{
		vector<int> v;
		return v;
	}
	else
	{
		vector<int> connected(1, vertex);
		unsigned int previous_connected_size = 0;
		while (previous_connected_size != connected.size())
		{
			previous_connected_size = connected.size();  /* copy */
			vector<int> previous_connected = connected;
			for (auto p=previous_connected.begin(); p!=previous_connected.end(); ++p)
			{	
				for (unsigned int j = 0; j < AdjMatrix[*p].size(); ++j)
				{
					if (AdjMatrix[*p][j])
					{
						connected.push_back(j);
					}
				}
			}
			sort(connected.begin(), connected.end());
			vector<int>::iterator it;
			it = unique (connected.begin(), connected.end());
			connected.resize(distance(connected.begin(),it) );
		}
		return connected;
	}
}


/* Helper functions prepared. */


class Board                                 /* definition of a Hex board  */
{
public:

	Board();
	Board(const int& board_size);
	virtual ~Board();

	int GetBoardSize();              /* returns the number of vertices in the graph  */
	void PlotBoard();               /* drawn the board in ASCII   */
	bool IsFull();           /* return whether there is still empty positions on the board */
	bool IsValidPlay(int& row_pos, int& col_pos);           /* return whether the user input position is valid */
	void Play(bool player, int& row_pos, int& col_pos);            /* place a move. player = false means first player; player = true means second player. */
	void RandomPlay(bool player);     /* randomly place a valid move */
	bool FirstPlayerWon();    /* The first player (1) won if the vertices 1 connects from top to bottom. */
	bool SecondPlayerWon();    /* The second player (2) won if the vertices 2 connects from left to right. */
	int EndGame();       /* If no winner yet, returns 0; If first player wins, returns 1; If second player wins, returns 2 */
	
	void PrintBoard();                                         /* FOR DEBUGGING: print vertices and adjacent matrices   */
	vector<int> GetCurrentVertices();               /* FOR DEBUGGING */
	vector<vector<bool> > GetOriginalEdgeAdjMat();               /* FOR DEBUGGING */
	
private:

    int _board_size;                 /* the number of points on each side  */
    int _num_vertices;                 /* the number of points on the entire board  */
    vector<int> _vexs;         /* information (value) of the vertex. 0 means empty, 1 means 1st player played, 2 means 2nd player played.  */
    vector<vector<bool> > _original_edge;    /* original board adjacent matrix  */
    vector<vector<bool> > _fist_edge;    /* first player adjacent matrix  */
	vector<vector<bool> > _second_edge;     /* second player adjacent matrix  */
	
	void _initialize_original_edge();     /* fill in _original_edge, false if not adjacent, true if adjacent  */
	void _update_first_edge();     /* fill in _fist_edge, false if not adjacent, true if adjacent  */
	void _update_second_edge();     /* fill in _second_edge, false if not adjacent, true if adjacent  */
	
};


Board :: Board() : _board_size(11)    /* default size: 11 x 11. */
{	
	this -> _num_vertices = 11 * 11;
	(this -> _vexs).resize(this -> _num_vertices, 0);
	(this -> _original_edge).resize(this -> _num_vertices, vector<bool>(this -> _num_vertices, false));
	(this -> _fist_edge).resize(this -> _num_vertices, vector<bool>(this -> _num_vertices, false));
	(this -> _second_edge).resize(this -> _num_vertices, vector<bool>(this -> _num_vertices, false));
	this -> _initialize_original_edge();
}


Board :: Board(const int& board_size) : _board_size(board_size)
{	
	this -> _num_vertices = board_size * board_size;
	(this -> _vexs).resize(this -> _num_vertices, 0);
	(this -> _original_edge).resize(this -> _num_vertices, vector<bool>(this -> _num_vertices, false));
	(this -> _fist_edge).resize(this -> _num_vertices, vector<bool>(this -> _num_vertices, false));
	(this -> _second_edge).resize(this -> _num_vertices, vector<bool>(this -> _num_vertices, false));
	this -> _initialize_original_edge();
}


Board :: ~Board()
{	
}


void Board :: _initialize_original_edge()
{
	for(int i = 0; i < this -> _board_size; i++)
	{
		for(int j=0; j < this -> _board_size; j++)
		{	
			if (i == 0)
			{
				if (j == 0)
				{
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j] = true;
				}
				else if (j == (this -> _board_size) - 1)
				{
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j] = true;
				}
				else
				{
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j] = true;
				}
			}
			else if (i == (this -> _board_size) - 1)
			{
				if (j == 0)
				{
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j + 1] = true;
				}
				else if (j == (this -> _board_size) - 1)
				{
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j] = true;
				}
				else
				{
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j + 1] = true;
				}
			}
			else
			{
				if (j == 0)
				{
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j] = true;
				}
				else if (j == (this -> _board_size) - 1)
				{
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j - 1] = true;
				}
				else
				{
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i - 1) * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][i * (this -> _board_size) + j + 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j - 1] = true;
					this -> _original_edge[i * (this -> _board_size) + j][(i + 1) * (this -> _board_size) + j] = true;
				}
			}
		}
	}
}


void Board :: _update_first_edge()
{
	for(int i = 0; i < this -> _num_vertices; i++)
	{
		for(int j=0; j < this -> _num_vertices; j++)
		{
			if ((this -> _original_edge)[i][j] && ((this -> _vexs)[i] == 1) && ((this -> _vexs)[j] == 1))
			{
				(this -> _fist_edge)[i][j] = true;
			}
		}
	}
} 


void Board :: _update_second_edge()
{
	for(int i = 0; i < this -> _num_vertices; i++)
	{
		for(int j=0; j < this -> _num_vertices; j++)
		{
			if ((this -> _original_edge)[i][j] && ((this -> _vexs)[i] == 2) && ((this -> _vexs)[j] == 2))
			{
				(this -> _second_edge)[i][j] = true;
			}
		}
	}
}


void Board :: PlotBoard()
{
	/*  To Do: make color printers... */
	cout << endl << "---------------------------------------------------------------------------" << endl;
	cout << "First Player: X \tSecond Player: O\t Empty Positions: ." << endl;
	cout << "First Player (1) aims to connect top and bottom;" << endl;
	cout << "Second Player (2) aims to connect left and right;" << endl;
	cout << "Current Board: " << endl << endl << "   ";
	for (int j=0; j < this -> _board_size; j++)
	{	
		if (j < 10)
		{
			cout << j << "   ";
		}
		else
		{
			cout << j << "  ";
		}
	}
	cout << endl << endl;
	
	for(int i = 0; i < this -> _board_size; i++)
	{	
		if (i < 10)
		{
			cout << i << "  ";
		}
		else
		{
			cout << i << " ";
		}
		for (int t = 0; t < i; t++)
		{
			cout << "  ";
		}
		for(int j=0; j < this -> _board_size; j++)
		{	
			if (j < this -> _board_size - 1)
			{
				if ((this -> _vexs[i * (this -> _board_size) + j]) == 1)
				{
					cout << "X - ";
				}
				else if ((this -> _vexs[i * (this -> _board_size) + j]) == 2)
				{
					cout << "O - ";
				}
				else
				{
					cout << ". - ";
				}
			}
			else
			{
				if ((this -> _vexs[i * (this -> _board_size) + j]) == 1)
				{
					cout << "X";
				}
				else if ((this -> _vexs[i * (this -> _board_size) + j]) == 2)
				{
					cout << "O";
				}
				else
				{
					cout << ".";
				}
			}
		}
		
		if (i < this -> _board_size - 1)
		{
			cout << endl << "    ";
			for (int t = 0; t < i; t++)
			{
				cout << "  ";
			}
			for(int j=0; j < this -> _board_size; j++)
			{
				if (j < this -> _board_size - 1)
				{
					cout << "\\ / ";
				}
				else
				{
					cout << "\\";
				}
			}
			cout << endl;
		}
	}
	cout << endl << endl << "---------------------------------------------------------------------------" << endl;
}


void Board :: PrintBoard()
{
	cout << endl << "---------------------------------------------------------------------------" << endl;
	cout << "First Player: 1 \tSecond Player: 2\t Empty Positions: 0" << endl;
	cout << "Printing vertices:" << endl;
	for (int i=0; i < this -> _board_size; i++)
	{	
		for (int j=0; j < this -> _board_size; j++)
		{
			cout << (this -> _vexs)[i * (this -> _board_size) + j] << "  ";
		}
		cout << endl;
	}
	cout << endl << "---------------------------------------------------------------------------" << endl;
	cout << "Printing board adjacent matrix:" << endl;
	for (int i=0; i < (this -> _num_vertices); i++)
	{	
		for (int j=0; j < (this -> _num_vertices); j++)
		{
			cout << (this -> _original_edge)[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl << "---------------------------------------------------------------------------" << endl;
	cout << "Printing first player's adjacent matrix:" << endl;
	for (int i=0; i < (this -> _num_vertices); i++)
	{	
		for (int j=0; j < (this -> _num_vertices); j++)
		{
			cout << (this -> _fist_edge)[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl << "---------------------------------------------------------------------------" << endl;
	cout << "Printing second player's adjacent matrix:" << endl;
	for (int i=0; i < (this -> _num_vertices); i++)
	{	
		for (int j=0; j < (this -> _num_vertices); j++)
		{
			cout << (this -> _second_edge)[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl << "---------------------------------------------------------------------------" << endl << endl;
}


int Board :: GetBoardSize() 
{
	return this -> _board_size;
}


vector<int> Board :: GetCurrentVertices()
{
	return (this -> _vexs);
}


vector<vector<bool> > Board :: GetOriginalEdgeAdjMat()
{
	return this -> _original_edge ;
}


bool Board :: IsFull()
{
	return (count((this -> _vexs).begin(), (this -> _vexs).end(), 0) == 0);
}


bool Board :: IsValidPlay(int& row_pos, int& col_pos)
{
	if ((row_pos < 0) || (row_pos >= (this -> _board_size)))
	{
		return false;
	}
	if ((col_pos < 0) || (col_pos >= (this -> _board_size)))
	{
		return false;
	}
	return ((this -> _vexs)[row_pos * (this -> _board_size) + col_pos] == 0);
}


void Board :: Play(bool player, int& row_pos, int& col_pos)
{
	if ((this -> IsValidPlay)(row_pos, col_pos))
	{
		(this -> _vexs)[row_pos * (this -> _board_size) + col_pos] = int(player) + 1;  /* player = false means first player; player = true means second player. */
		if (player)
		{
			this -> _update_second_edge();
		}
		else
		{
			this -> _update_first_edge();
		}
	}
}


bool Board :: FirstPlayerWon()    /* The first player (1) won if the vertices 1 connects from top to bottom. */
{
	int min_bottom = (this -> _num_vertices) - (this -> _board_size);
	for(int i=0; i < (this -> _board_size); i++)
	{
		if ((this -> _vexs)[i] != 1)
		{
			continue;
		}
		vector<int> span = find_maximal_span(this -> _fist_edge, i);
		for (auto p = span.begin(); p != span.end(); p++)
		{
			if (*p >= min_bottom)
			{
				return true;
			}
		}
	}
	return false;
}


bool Board :: SecondPlayerWon()    /* The second player (2) won if the vertices 2 connects from left to right. */
{
	for(int i=0; i < (this -> _num_vertices); i = i + (this -> _board_size))
	{
		if ((this -> _vexs)[i] != 2)
		{
			continue;
		}
		vector<int> span = find_maximal_span(this -> _second_edge, i);
		for (auto p = span.begin(); p != span.end(); p++)
		{
			if (*p % (this -> _board_size) == ((this -> _board_size) - 1))
			{
				return true;
			}
		}
	}
	return false;
}


int Board :: EndGame()       /* If no winner yet, returns 0; If first player wins, returns 1; If second player wins, returns 2 */
{
	if (this -> FirstPlayerWon())
	{
		return 1;
	}
	else if (this -> SecondPlayerWon()) 
	{
		return 2;
	}
	else
	{
		return 0;
	}
}


void Board :: RandomPlay(bool player)
{	
	
	int num_valid = count((this -> _vexs).begin(), (this -> _vexs).end(), 0);
	if (num_valid > 0)
	{
		random_device rd;
		mt19937 eng(rd());
		uniform_int_distribution<int> distr(0, num_valid - 1);
		
		int index = distr(eng);
		for(auto p=(this -> _vexs).begin(); p!=(this -> _vexs).end(); ++p)
		{
			if ((index == 0) && (*p == 0))
			{
				*p = int(player) + 1;  /* player = false means first player; player = true means second player. */
				break;
			}
			if (*p == 0)
			{
				index -= 1;
			}
		}
		
		if (player)
		{
			this -> _update_second_edge();
		}
		else
		{
			this -> _update_first_edge();
		}
	}
}


/* Completed Inplementing Board. */


int main()
{
   
    int board_size;
    cout << "How Many Points On Each Side? " << endl;
    cout << " - Input an integer between 3 and 20..." << endl;
    cin >> board_size;
    
    Board board = Board(board_size);
    board.PlotBoard();
    
    cout << "You are the first player. " << endl;
    
    while ((!board.IsFull()) && (!board.EndGame()))
    {
		int a, b;
		char comma;
	    cout << "Place a move. Input two comma separated integers, eg, type 0, 0" << endl;
	    cin >> a >> comma >> b;
	    if (board.IsValidPlay(a, b))
	    {
			board.Play(false, a, b);
			if ((!board.IsFull()) && (!board.EndGame()))
			{
				board.RandomPlay(true);
			}
			board.PlotBoard();
	    }
	    else
	    {
			cout << "Invalid move..." << endl;
		}
	}
   
	int result = board.EndGame();
	if (result == 1)
	{
		board.PlotBoard();
		cout << "Congratulations! You (Player 1) win!" << endl;
	}
	else if (result == 2)
	{
		board.PlotBoard();
		cout << "You lost. Computer (Player 2) wins!" << endl;
	}
	else
	{
		board.PlotBoard();
		cout << "Draw." << endl;    /* This never happens */
	}
}
