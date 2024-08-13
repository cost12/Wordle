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

	void addGuessScore(const std::string &s1, const std::string &s2, const std::string &s3);

	void reset();

	bool setWord(const std::string &w);
	void setWord();

	bool makeGuess(const std::string &w);
	void makeGuess();
	void deepGuess();

	bool isOver() const ;
	bool gameWon() const ;

	bool inDictionary(const std::string &s) const ;
	bool isValid(const std::string &s) const ;
	bool addToBoard(const std::string &guess, const std::string &score);

	void printValidGuesses() const ;
	void printBoard() const ;

	void outputBestWords();
	void outputDeepWordScore(const std::string &w);

	void printAllScores();

private:

	std::string scoreGuess(const std::string &g);
	std::string scoreGuess(const std::string &g, const std::string &sW);

	void addGuess(const std::string &g);

	unsigned int maxGuesses = 6;
	unsigned int wordLength = 5;
	
	std::unordered_set<std::string> dictionary;
	std::unordered_set<std::string> common;
	std::unordered_set<std::string> stillValid;
	std::string theWord;
	std::vector<std::string> guesses;
	std::vector<std::string> scores;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> wordScores;
};

std::string toUpperCase(const std::string &s);

std::string combineGuesses(const std::string &g1, const std::string &g2, const std::string &w1, const std::string &w2);


#endif