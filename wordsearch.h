#include <vector>
#include <string>

struct point{
	char col;	//column
	int row;	//row
};

struct wordx{//stores start point, end point, word
	point start;
	point end;
	std::string word;
	bool found;
};

class WordSearch{
	char puzzle[10][10];
	std::vector<wordx> words;
	void display();		//displays the Puzzle, called from solve
public:
	void generate();	//generate new puzzle with random words
	void solve();		//User Interface for solving the Word Search
};