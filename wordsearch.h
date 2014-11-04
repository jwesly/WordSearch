#include <vector>
#include <string>
#define FILE "../WordSearch/dictionary.txt"		//defines dictionary file
#define ALPHABET 26								//defines number of chars in alphabet
#define PUZZLE_SIZE 10							//defines size of puzzle (all are square)
#define BOUND_LOWER 0							//defines lowest  possible index of location in puzzle
#define BOUND_UPPER PUZZLE_SIZE - 1				//defines highest possible index of location in puzzle

struct trie{
	bool is_word;
	trie* children[ALPHABET];
};

class Dictionary{
	trie root;
	bool load(const char*, trie*);
	void unload(trie*);
	std::string choose(trie*);
public:
	Dictionary();
	std::string random();
	~Dictionary();
};

struct point{
	char col;	//column
	char row;	//row
};

struct wordx{//stores start point, end point, word
	point start;
	point end;
	std::string word;
	bool found;
};

class WordSearch{
	char puzzle[PUZZLE_SIZE][PUZZLE_SIZE];
	bool  found[PUZZLE_SIZE][PUZZLE_SIZE];
	std::vector<wordx*> words;	//vector of words and their locations in the puzzle
	Dictionary dict;
	wordx* testWord(std::string,int,int,int,int);	
	/*checks if word can be stored with given string, x coordinate, y coordinate, direction
	if possible write word into puzzle and return pointer to word, else return NULL*/
	void display();				//displays the Puzzle, called from solve
	bool exists(std::string);	//returns true if word has been used in puzzle already
	bool check(std::string);
	int words_left();
	int words_found();
	void discover(wordx);	//sets bools in found array to true upon discovery of a word
public:
	~WordSearch();
	void generate();	//generate new puzzle with random words
	void solve();		//User Interface for solving the Word Search
	void clear();		//clear the puzzle
	int stats();
};