#ifndef WORDSEARCH_F
#define WORDSEARCH_F

#include "wordsearch.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <ctime>


void WordSearch::display(){
	std::cout << "   ";
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);///For making pretty colors
	if (hStdout == INVALID_HANDLE_VALUE)
	{
		std::cout << "Error while getting input handle" << std::endl;
	}
	//sets color to red on black
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);	

	for (int i = 0; i < PUZZLE_SIZE; i++)
		std::cout << char(i + 65) << " ";
	std::cout << "\n\n";

	//white on black
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	for (int i = 0; i < PUZZLE_SIZE; i++){
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);//red on black
		std::cout << i << "  ";
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//white on black
		for (int j = 0; j < PUZZLE_SIZE; j++)
		{
			if (found[i][j])
				SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // green
			std::cout << puzzle[i][j] << " ";
			SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//white on black
		}
		std::cout << "\n";
	}
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_BLUE);//blue-green
	std::cout << "\n";
	for (int i = 0; i < words.size(); i++)
		if (!words[i]->found)
			std::cout  << words[i]->word << "\n";
			//std::cout << words[i]->word << "\t\t" << words[i]->start.col << words[i]->start.row << " " << words[i]->end.col << words[i]->end.row  << "\n";
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//white
	return;
}

void WordSearch::discover(wordx a){
	int hs = 0;
	int vs = 0;
	if (a.start.col > a.end.col)
		hs = -1;
	else if (a.start.col < a.end.col)
		hs = 1;
	if (a.start.row > a.end.row)
		vs = -1;
	else if (a.start.row < a.end.row)
		vs = 1;
	int hindex = a.start.col - 65;
	int vindex = a.start.row - 48;
	found[vindex][hindex] = true;
	do{
		vindex += vs;
		hindex += hs;
		found[vindex][hindex] = true;		
	} while (hindex != a.end.col - 65 || vindex != a.end.row - 48);
}

bool WordSearch::check(std::string answer)
{
	if (answer.length() != 5)
		return false;
	char column1, column2, row1, row2;
	column1 = answer[0];
	column2 = answer[3];
	row1 = answer[1];
	row2 = answer[4];
	for (int i = 0; i < words.size(); i++)
	{
		if (answer[0] == words[i]->start.col & answer[3] == words[i]->end.col & answer[1] == words[i]->start.row & answer[4] == words[i]->end.row){
			words[i]->found = true;
			discover(*(words[i]));
			return true;
		}
	}
	return false;
}

int WordSearch::words_left()
{
	int x= 0;
	for (int i = 0; i < words.size(); i++)
		if (words[i]->found == false)
			x++;
	return x;
}

int WordSearch::words_found()
{
	int x = 0;
	for (int i = 0; i < words.size(); i++)
		if (words[i]->found == true)
			x++;
	return x;
}

void WordSearch::solve(){
	while (1)
	{
		display();
		std::string answer;
		std::cout << "Enter the range (ex. A1:A9 for horizontal) you believe is an answer: ";
		getline(std::cin, answer);
		std::cout << std::endl;
		for (int i = 0; i < 3; i++) {
			std::cout << ". "; 
			_sleep(300);
		}
		if (check(answer)){
			std::cout << "YOU FOUND A WORD!!!! :) \n\n";
		}
		else{
			std::cout << "Sorry that's not a word :(\n\n";
		}
		if (words_left() == 0)
		{
			std::cout << "Woohoo! You Finished the Puzzle!!!!\n\n\n";
			break;
		}
		_sleep(1000);
	}
}

void WordSearch::clear(){
	for (int i = 0; i < PUZZLE_SIZE; i++)
		for (int j = 0; j < PUZZLE_SIZE; j++)
		{
			puzzle[i][j] = '\0';	//set all locations to NUL
			found[i][j] = false;
		}
	for (int i = 0; i < words.size(); i++)
		delete words[i];
	words.clear();
}

void WordSearch::generate(){
	clear();	
	//Horizontal Directions: -1 for left, 1 for right
	//Vertical Directions: -1 for down, 1 for up
	wordx* word;
	for (int i = 0; i < 500; i++){		//if you can't find a word that fits in 500 attempts, stop
		int dh, dv;
		do{
			dh = rand() % 3 - 1;
			dv = rand() % 3 - 1;
		} while (dv == 0 && dh == 0);
		int x = rand() % PUZZLE_SIZE;
		int y = rand() % PUZZLE_SIZE;
		std::string a;
		do{
			a = dict.random();
		} while (exists(a));
		word = testWord(a, x, y, dv, dh);
		if (word != NULL){
			words.push_back(word);
			i = 0;//reset attempts 
		}
	}
	for (int i = 0; i < PUZZLE_SIZE; i++)
		for (int j = 0; j < PUZZLE_SIZE; j++)
			if (puzzle[i][j] == '\0')
				puzzle[i][j] = (rand() % 26) + 97;
	return;
}

bool WordSearch::exists(std::string a){
	for (int i = 0; i < words.size(); i++)
		if (words[i]->word == a)
			return true;
	return false;
}

wordx* WordSearch::testWord(std::string word, int x, int y, int v, int h){
	int i = 0;//make i global
	for (; word[i] != '\0'; i++){
		if (x + v*i < BOUND_LOWER || x + v*i > BOUND_UPPER || y + h*i < BOUND_LOWER || y + h*i > BOUND_UPPER)//if out of range
			return NULL;
		if (puzzle[x + v*i][y + h*i] != word[i] && puzzle[x + v*i][y + h*i] != '\0')//if occupied by different letter
			return NULL;
	}
	i--;
	wordx* baby = new wordx;
	baby->found = false;
	baby->word = word;
	baby->start.row = x + 48;
	baby->start.col = y + 65;
	baby->end.row = x + v*i + 48;
	baby->end.col = y + h*i + 65;
	for (i = 0; word[i] != '\0'; i++)
		puzzle[x + v*i][y + h*i] = word[i];//write word into puzzle
	return baby;
}

WordSearch::~WordSearch(){
	clear();
}

Dictionary::Dictionary(){
	for (int i = 0; i < ALPHABET; i++){
		root.children[i] = NULL;
	}
	srand(time(0));
	root.is_word = false;
	std::ifstream fin;
	fin.open(FILE_Z);
	std::string x;
	while (fin){
		fin >> x;
		if (x.length() > MIN_WORD_LENGTH)
			load( x.c_str() , &root);//Load all words in dictionary file into the dictionary
	}
	fin.close();
}

bool Dictionary::load(const char* word, trie* node){
	if (*(word) == '\0')//if the word is finished
	{
		node->is_word = true;//set flag to say that this is a word
		return true;
	}
	//std::cout << node->children[*(word)-97];
	if (node->children[*(word)-97] == NULL)//if memory has not been allocated for child, allocate memory
	{
		trie* baby = new trie;
		baby->is_word = false;
		for (int i = 0; i < ALPHABET; i++)
			baby->children[i] = NULL;
		node->children[*(word)-97] = baby;
	}
	load(word + 1, node->children[*(word)-97]);//recursively load word
	return true;
}

std::string Dictionary::choose(trie* node){
	char a;
	do{
		a = (rand() % 27) + 97;
		if (a == '{'){
			if (node->is_word)
				return "";
			else
				a = 'a';
		}
		
	} while (node->children[a-97] == NULL);
	return a + choose(node->children[a - 97]);
}

std::string Dictionary::random(){
	return choose(&root);
}

void Dictionary::unload(trie* node){
	for (int i = 0; i < ALPHABET; i++){
		if (node->children[i] != NULL)
			unload(node->children[i]);//recursively unload word
	}
	if (node != &root) //root was not dynamically allocated so it cannot be dynamically deleted
		delete node;
	return;
}

Dictionary::~Dictionary(){
	unload(&root);
}

#endif