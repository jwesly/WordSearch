#include <vector>
#include <string>
#define ALPHABET 26

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
	Dictionary dict;
	void display();		//displays the Puzzle, called from solve
public:
	void generate();	//generate new puzzle with random words
	void solve();		//User Interface for solving the Word Search
};