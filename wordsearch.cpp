#include "wordsearch.h"
#include <fstream>
#include <iostream>
#include <ctime>

void WordSearch::display(){
	return;
}

void WordSearch::solve(){
	return;
}

void WordSearch::generate(){
	return;
}

#define FILE "../WordSearch/dictionary.txt"

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
	char cont = 'y';
	while (1){

		std::cout << "\n" << random() << "\nEnter y to cont: ";
		std::cin >> cont;
		if (cont != 'y')
			break;
	}
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
	std::cout << "Memory being deallocated............";
	unload(&root);
	std::cout << "Deallocated!!!\n\n";
}