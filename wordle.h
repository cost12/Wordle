#ifndef __wordle_h_
#define __wordle_h_

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

class Wordle {
public:
	Wordle();
	Wordle(const std::unordered_set<std::string> &dict);
	Wordle(const std::unordered_set<std::string> &dict, const std::unordered_set<std::string> &valid);

	bool setWord(const std::string &w);
	void setWord();

	bool makeGuess(const std::string &w);
	void makeGuess();

	bool isOver() const ;
	bool gameWon() const ;

	bool inDictionary(const std::string &s) const ;
	bool addToBoard(const std::string &guess, const std::string &score);

	void printValidGuesses() const ;
	void printBoard() const ;

	void outputBestWords() const ;

private:

	std::string scoreGuess(const std::string &g) const ;
	std::string scoreGuess(const std::string &g, const std::string &sW) const ;

	void addGuess(const std::string &g);

	unsigned int maxGuesses = 6;
	unsigned int wordLength = 5;
	std::unordered_set<std::string> dictionary;
	std::unordered_set<std::string> stillValid;
	std::string theWord;
	std::vector<std::string> guesses;
	std::vector<std::string> scores;
};

std::string toUpperCase(const std::string &s);


#endif