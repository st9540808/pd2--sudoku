#include <iostream>
using namespace std;

class Sudoku
{
	public:
		Sudoku();
		void giveQuestion();
		void readIn();
		void solve();
		void changeNum(int a, int b);
		void changeRow(int a, int b);
		void changeCol(int a, int b);
		void rotate(int n);
		void flip(int n);
		void transform();
	
		bool isCorrect(int array[9][9]);//used to ckeck if a sudoku board is correct in its rule
			
	private:
		int map[9][9];//uesd to store question from readIn
		int ans[9][9];//used to test all possible asnwer
		int candidateAns[9][9][10];//used to stored all candidate(possible) answer
		int solution1[9][9];//used to store a correct answer
		
		int answerType;
		void selectCandidate();//select all candidate answers and store in candidateAns[][][] array
		bool validAnsExist(int row, int col);//test if a valid answer is exist in certain point and time
		bool determineAnsType(int currentRow, int iteration);//determine the answer type of question 
		void printOut(int array[9][9]);//print out an array
	
		void printCandidate();//debug
};
