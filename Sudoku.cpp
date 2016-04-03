#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Sudoku.h"
#define CYAN "\x1b[36m"
#define RESET "\033[0m"
using namespace std;

Sudoku::Sudoku()
{
	for(int *mapPtr = &map[0][0], *ansPtr = &ans[0][0], *solution1Ptr = &solution1[0][0];
			mapPtr < &map[9][0];  ++mapPtr, ++ansPtr, ++solution1Ptr)
	{	
		*mapPtr = 0;
		*ansPtr = 0;
		*solution1Ptr = 0;
	}
	answerType = -1;

	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
			for(int k = 0; k < 10; ++k)
				candidateAns[i][j][k] = k;
}

void Sudoku::giveQuestion()
{
	int question[9][9] = 
		{
			2, 0, 0, 8, 5, 9, 0, 0, 6, 
			0, 0, 6, 3, 4, 1, 0, 2, 0, 
			0, 0, 9, 6, 7, 2, 5, 4, 0, 
			8, 5, 2, 0, 0, 4, 0, 1, 7, 
			0, 0, 0, 2, 8, 0, 4, 3, 5, 
			0, 0, 3, 0, 1, 0, 0, 9, 0, 
			0, 3, 0, 0, 9, 0, 0, 0, 2, 
			7, 9, 0, 0, 2, 0, 1, 0, 0, 
			1, 0, 0, 7, 6, 0, 0, 5, 4,
		};
	printOut(question);
}

void Sudoku::readIn()
{
	for(int *p = &map[0][0]; p < &map[9][0]; ++p)
		cin >> *p;
}

void Sudoku::solve()
{
	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
				ans[i][j] = map[i][j];

	selectCandidate();
//	printCandidate();

	int row = 0, col = 0;
	while( map[row][col] != 0 )//initialize to the location of first blank in map[][]
	{
		++col;
		if( col == 9 ){	++row; col = 0;	if(row == 9)  break;}
	}
	
	row = isCorrect(map) ? row : -1 ;
	
	for(int iteration = 0; iteration < 2; ++iteration)
	{
		while( row != 9 && row != -1 )//start backtracking
		{
			if( validAnsExist(row, col) )
			{
				do
				{
					++col;
					if( col == 9 ){	++row; col = 0;if(row == 9) break;}
				}while( map[row][col] != 0 );
			}
			else
			{
				do
				{
					--col;
					if( col == -1 ){ --row; col = 8;if(row == -1) break;}
				}while( map[row][col] != 0 );
			}
		}//end of back tracking
			
		if( determineAnsType(row, iteration) )
			break;
		else
		{
			do
			{
				--col;
				if( col == -1 ){ --row; col = 8;if(row == -1) break;}
			}while( map[row][col] != 0 );
		}
	}
	
	
	switch(answerType)//output answer
	{
		case 0:
			cout << "0" << endl;
			return;
		case 1:
			cout << "1" << endl;
			printOut(solution1);
			return;
		case 2:
			cout << "2" << endl;
			return;
	}

}

void Sudoku::selectCandidate()
{
	//select candidateAns according to row
	for(int i = 0; i < 9; ++i)
	{
		bool candidateToDelete[10] = {false};
		
		for(int j = 0; j < 9; ++j)
			if( map[i][j] != 0 ) 
				candidateToDelete[ map[i][j] ] = true;
		
		for(int j = 0; j < 9; ++j)
		{
			if( map[i][j] == 0 )
			{
				for(int k = 1; k < 10; ++k)
					if( candidateToDelete[k] == true )
						candidateAns[i][j][k] = 0;
			}
		}
	}
	
	//select candidateAns according to column
	for(int i = 0; i < 9; ++i)
	{
		bool candidateToDelete[10] = {false};
		
		for(int j = 0; j < 9; ++j)
			if( map[j][i] != 0 ) 
				candidateToDelete[ map[j][i] ] = true;
		
		for(int j = 0; j < 9; ++j)
		{
			if( map[j][i] == 0 )
			{
				for(int k = 1; k < 10; ++k)
					if( candidateToDelete[k] == true)
						candidateAns[j][i][k] = 0;
			}
		}
	}

	//select candidateAns according to cell
	for(int i = 0; i <= 2; ++i)
	{
		for(int j = 0; j <= 2 ; ++j)
		{
			bool candidateToDelete[10] = {false};

			for(int row = 3 * i; row <= 3 * i + 2; ++row)
				for(int col = 3 * j; col <= 3 * j + 2; ++col)
					if( map[row][col] != 0 )
						candidateToDelete[ map[row][col] ] = true;
			
			for(int row = 3 * i; row <= 3 * i + 2; ++row)
				for(int col = 3 * j; col <= 3 * j + 2; ++col)
				{	
					if( map[row][col] == 0 )
					{	
						for(int k = 1; k < 10; ++k)
							if( candidateToDelete[k] == true )
								candidateAns[row][col][k] = 0;
					}
				}
		}
	}

	//find the set with only one candidate answer
	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
		{
			if( map[i][j] == 0 )
			{
				int countElement = 0,
					elementToDelete;
				for(int k = 1; k < 10; ++k)
				{
					if( candidateAns[i][j][k] != 0 )
					{	
						++countElement; if(countElement == 2) goto nextBlank;
						elementToDelete = candidateAns[i][j][k];
					}
				}
				
				//clear row
				for(int k = 0; k < 9; ++k)
					if( candidateAns[i][k][elementToDelete] != 0 && k != j )
						candidateAns[i][k][elementToDelete] = 0;
				
				//clear column
				for(int k = 0; k < 9; ++k)
					if( candidateAns[k][j][elementToDelete] != 0 && k != i )
						candidateAns[k][j][elementToDelete] = 0;
				
				//clear cell
				for(int cellX = 3 * (i/3); cellX <= 3 * (i/3) + 2; ++cellX)
					for(int cellY = 3 * (j/3); cellY <= 3 * (j/3) + 2; ++cellY)
					{
						if( candidateAns[cellX][cellY][elementToDelete] != 0 && (cellX != i && cellY != j) )
							candidateAns[cellX][cellY][elementToDelete] = 0;
					}
			}
			nextBlank:;
		}

}


bool Sudoku::determineAnsType(int currentRow, int iteration)
{
	if( currentRow == -1 && iteration == 0 )
	{
		answerType = 0;
		return true;
	}
	else if( iteration == 1 )
	{
		switch(currentRow)
		{
			case -1:
				answerType = 1;
				return true;
			case 9:
				answerType = 2;
				return true;
		}
	}
	for(int i = 0; i < 9; ++i)
		for(int j = 0; j < 9; ++j)
			solution1[i][j] = ans[i][j];
	return false;
}

bool Sudoku::validAnsExist(int row, int col)
{
	//find possible answer from candidate answer(the goto is need for breaking nested loop)
	
	for(int testAns = ans[row][col]; testAns != 10; )
	{
		do
		{
			++testAns;
			if( testAns == 10 ){ ans[row][col] = 0; return false;}
		}while( candidateAns[row][col][testAns] == 0 );
		
		int numCounter[9] = {0};

		//Verify row
		++numCounter[testAns - 1];
		for(int i = 0; i < 9; ++i)
		{
			if( ans[row][i] == 0 || i == col )
				continue;
			else
				++numCounter[ ans[row][i] - 1 ];
		}
		for(int i = 0; i < 9; ++i)
		{
			if( numCounter[i] >= 2 )
				goto end;
			numCounter[i] = 0;//Re-initialize
		}
			
	
		//Verify column
		++numCounter[testAns - 1];
		for(int i = 0; i < 9; ++i)
		{
			if( ans[i][col] == 0 || i == row )
				continue;
			else
				++numCounter[ ans[i][col] - 1];
		}
		for(int *p = &numCounter[0]; p < &numCounter[9]; ++p)
		{
			if( *p >= 2 )
				goto end;
			*p = 0;
		}



		{	//Verify cell
			++numCounter[testAns - 1];
			int cellX = 3 * (row/3), cellY = 3 * (col/3);//identify the element belong to what cell
			for(int i = 0; i < 3; ++i)
				for(int j = 0; j < 3; ++j)
				{
					if( ans[ cellX + i ][ cellY + j ] == 0 ||
						( cellX + i == row  &&  cellY + j == col ) )
						continue;
					else
						++numCounter[ ans[ cellX + i ][ cellY + j ] - 1 ];
				}
			for(int *p = &numCounter[0]; p < &numCounter[9]; ++p)
			{
				if( *p >= 2 )
					goto end;
			}
		}
		
		//if pass all the verifying test
		ans[row][col] = testAns;
		return true;
		
		//three of one of the verifying test failed
		end:;
	}
	ans[row][col] = 0;
	return false;
}


//Transform
void Sudoku::changeNum(int a, int b)
{
	if( a == 0 || b == 0 )
		return;
	if( a == b )
		return;
	
	for(int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			if( map[i][j] == a )
				map[i][j] = b;
			else if( map[i][j] == b )
					map[i][j] = a;
		}
	}
}

void Sudoku::changeRow(int a, int b)
{
	if( a == b )
		return;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			int temp = map[3 * a + i][j];
			map[3 * a + i][j] = map[3 * b + i][j];
			map[3 * b + i][j] = temp;
		}
	}
}

void Sudoku::changeCol(int a, int b)
{
	if( a == b )
		return;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			int temp = map[j][3 * a + i];;
			map[j][3 * a + i] = map[j][3 * b + i];
			map[j][3 * b + i] = temp;
		}
	}
}

void Sudoku::rotate(int n)
{
	n = n % 4;
	
	for(int times = 0; times < n; ++times)
	{	
		for(int i = 0; i < 9; ++i)
			for(int j = i; j < 9; ++j)
			{
				int temp = map[i][j];
				map[i][j] = map[j][i];
				map[j][i] = temp;
			}

		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 9; ++j)
			{
				int temp = map[j][i];
				map[j][i] = map[j][8 - i];
				map[j][8 - i] = temp;
			}
	}
}

void Sudoku::flip(int n)
{
	if( n == 0 )//flip vertically
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 9; ++j)
			{
				int temp = map[j][i];
				map[j][i] = map[j][8 - i];
				map[j][8 - i] = temp;
			}	
	}
	else//flip horizontally
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 9; ++j)
			{
				int temp = map[i][j];
				map[i][j] = map[8 - i][j];
				map[8 - i][j] = temp;
			}	
	}
}

void Sudoku::transform()
{
	srand(time(NULL));
	readIn();
	changeNum(rand() % 10, rand() % 10);
	changeRow(rand() %  3, rand() %  3);
	changeCol(rand() %  3, rand() %  3);
	rotate(rand() % 101);
	flip(rand() % 2);
	printOut(map);
}

bool Sudoku::isCorrect(int array[9][9])
{
	int numCounter[10] = {0};
	
	//verify rows
	for(int i = 0; i < 9; ++i)
	{
		
		for(int j = 0; j < 9; ++j)
			++numCounter[ array[i][j] ];
		for(int *p = &numCounter[1]; p < &numCounter[10]; ++p)
		{
			if( *p >= 2 )
				return false;
			*p = 0;//Re-initialize
		}
	}
	
	//verify columns
	for(int i = 0; i < 9; ++i)
	{
		int numCounter[10] = {0};

		for(int j = 0; j < 9; ++j)
			++numCounter[ array[j][i] ];
		for(int *p = &numCounter[1]; p < &numCounter[10]; ++p)
		{
			if( *p >= 2 )
				return false;
			*p = 0;//Re-initialize
		}
	}

	//verify cells
	for(int i = 0; i <= 2; ++i)
	{
		for(int j = 0; j <=2 ; ++j)
		{
			int numCounter[10] = {0},
				row = 3 * i,
				col = 3 * j;

			for(;row <= 3 * i + 2; ++row)
				for(;col <= 3 * j + 2; ++col)
					++numCounter[ array[row][col] ];
		
			for(int *p = &numCounter[1]; p < &numCounter[10]; ++p)
			{
				if( *p >= 2 )
					return false;
			}
		}
	}
	return true;
}

void Sudoku::printOut(int array[9][9])
{
	for(int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
			cout << array[i][j] << " ";
		cout << "\n";
	}
}

void Sudoku::printCandidate()
{
	for(int i = 0; i < 9; ++i)
	{
		for(int block = 0; block < 45; ++block) cout << "-";
		cout << "\n";
		
		for(int line = 0; line < 3; ++line)
		{
			for(int j = 0; j < 9; ++j)
			{	
				cout << "|";
				if( map[i][j] == 0 )
				{	
					for(int k = 1; k < 4; ++k)
					{
						if( candidateAns[i][j][ 3 * line + k ] != 0 )
							cout << candidateAns[i][j][ 3 * line + k ];		
						else
							cout << " ";
					}	
				}
				else
				{
					if( line != 1 )
						cout << "   ";
					else
						cout << " " << CYAN << map[i][j] << RESET << " ";
				}
				cout << "|";
			}
			cout << "\n";
		}	
	}
}	
