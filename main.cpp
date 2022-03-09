#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_set>
#include <iostream>

#include "wordle.h"

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 4) {
		std::cerr << "ERROR: need to pass in exactly one file and optionally one play type" << std::endl;
		exit(1);
	}

	std::ifstream istr(argv[1]);
	std::ifstream istr2(argv[2]);
	if (!istr) {
		std::cerr << "ERROR: invalid file 1" << std::endl;
		exit(1);
	}
	if (argc == 4) {
		if (!istr2) {
			std::cerr << "ERROR: invalid file 2" << std::endl;
			exit(1);
		}
	}

	std::string playType = "";
	if (argc == 3) {
		playType = argv[2];
	} else if (argc == 4) {
		playType = argv[3];
	}

	if (playType == "parse") {
		std::vector<std::string> dict;
		for (std::string word; std::getline(istr, word, ','); ) {
			dict.push_back(word.substr(1, word.size()-2));
		}
		for (unsigned int i = 0; i < dict.size(); i++) {
			std::cout << dict[i] << std::endl;
		}
		return 0;
	}

	std::unordered_set<std::string> dict;
	std::string word;
	while (istr >> word) {
		dict.insert(word);
	}

	Wordle wordleGame;

	if (argc == 3) {
		wordleGame = Wordle(dict);
	} else if (argc == 4) {
		std::unordered_set<std::string> valid;
		std::string word;
		while (istr2 >> word) {
			valid.insert(word);
		}
		wordleGame = Wordle(dict, valid);
	}
	

	if (playType == "Guess_Word") {
		std::string guess;
		wordleGame.setWord();
		wordleGame.printBoard();
		std::cout << "Word selected" << std::endl;
		while (!wordleGame.isOver()) {
			std::cout << "Make your guess: ";
			std::cin >> guess;
			if (wordleGame.makeGuess(guess)) {
				wordleGame.printBoard();
			} else {
				std::cerr << "ERROR: Invalid input for guess, not in dictionary" << std::endl;
			}
		}
		std::cout << "GAME OVER" << std::endl;
	} else if (playType == "Choose_Word") {
		std::string word;
		bool wordSet = false;
		while (!wordSet) {
			std::cout << "Choose a word: ";
			std::cin >> word;
			if (wordleGame.setWord(word)) {
				std::cout << "Word set, ready to begin..." << std::endl;
				wordSet = true;
			} else {
				std::cerr << "ERROR: Invalid secret word, either not in dictionary or too uncommon" << std::endl;
			}
		}
		std::cout << "Enter best word or no: ";
		std::cin >> word;
		wordleGame.makeGuess(word);
		wordleGame.printBoard();
		std::cout << "Thank you, AI time!" << std::endl;
		while (!wordleGame.isOver()) {
			wordleGame.makeGuess();
			wordleGame.printBoard();
		}
		std::cout << "GAME OVER" << std::endl;
	} else if (playType == "Enter_Board") {
		std::string guess;
		std::string score;
		//bool done = false;
		while (true) {
			std::cout << "Enter guess: ";
			std::cin >> guess;
			if (guess == "done") { break; }
			std::cout << "Enter score: ";
			std::cin >> score;
			if (wordleGame.inDictionary(guess)) {
				if (wordleGame.addToBoard(guess, score)) {
					wordleGame.printBoard();
				} else {
					std::cerr << "ERROR: Score is not correct with rest of board" << std::endl;
				}
			} else {
				std::cerr << "ERROR: Invalid guess, not in dictionary" << std::endl;
			}
		}
		wordleGame.printValidGuesses();

	} else if (playType == "Choose_Word_No_Trust") {
		
	} else if (playType == "Find_Good_Words") {
		wordleGame.outputBestWords();
	} else {
		std::cout << "Goodbye" << std::endl;
	}
}