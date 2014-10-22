#include "wordsearch.h"
#include <fstream>
#include <iostream>
#include <ctime>


void WordSearch::display(){
	for (int i = 0; i < PUZZLE_SIZE; i++){
		for (int j = 0; j < PUZZLE_SIZE; j++)
			std::cout << puzzle[i][j] << " ";
		std::cout << "\n";
	}
	return;
}

void WordSearch::solve(){
	display();
	for (int i = 0; i < words.size(); i++)
		std::cout << "\n" << words[i]->start.col << words[i]->start.row << ": " << words[i]->end.col << words[i]->end.row << "\t\t" << words[i]->word;
	return;
}

void WordSearch::clear(){
	for (int i = 0; i < PUZZLE_SIZE; i++)
		for (int j = 0; j < PUZZLE_SIZE; j++)
			puzzle[i][j] = '\0';	//set all locations to NUL
	for (int i = 0; i < words.size(); i++)
		delete words[i];
	words.clear();
}

void WordSearch::generate(){
	clear();	
	//Horizontal Directions: -1 for left, 1 for right
	//Vertical Directions: -1 for down, 1 for up
	wordx* word;
	for (int i = 0; i < 500; i++){		//if you can't find a word that fits in 10,000 attempts, stop
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
				puzzle[i][j] = '_';//(rand() % 26) + 97;
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
	baby->start.row = x;
	baby->start.col = y + 65;
	baby->end.row = x + v*i;
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
	fin.open(FILE);
	std::string x;
	while (fin){
		fin >> x;
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