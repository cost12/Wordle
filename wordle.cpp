#include "wordle.h"

Wordle::Wordle() {
	dictionary = std::unordered_set<std::string>();
	stillValid = std::unordered_set<std::string>();
	theWord = "";
	guesses = std::vector<std::string>();
}

Wordle::Wordle(const std::unordered_set<std::string> &dict) {
	for (std::unordered_set<std::string>::const_iterator itr = dict.begin(); itr != dict.end(); itr++) {
		dictionary.insert(toUpperCase(*itr));
		stillValid.insert(toUpperCase(*itr));
	}
	theWord = "";
	guesses = std::vector<std::string>();
}

Wordle::Wordle(const std::unordered_set<std::string> &dict, const std::unordered_set<std::string> &valid) {
	for (std::unordered_set<std::string>::const_iterator itr = dict.begin(); itr != dict.end(); itr++) {
		dictionary.insert(toUpperCase(*itr));
	}
	for (std::unordered_set<std::string>::const_iterator itr = valid.begin(); itr != valid.end(); itr++) {
		stillValid.insert(toUpperCase(*itr));
	}
	theWord = "";
	guesses = std::vector<std::string>();
}

bool Wordle::setWord(const std::string &w) {
	std::string word = toUpperCase(w);
	if (stillValid.find(word) != stillValid.end()) {
		guesses = std::vector<std::string>();
		scores =  std::vector<std::string>(); 
		theWord = word;
		return true;
	}
	return false;
}

void Wordle::setWord() {
	std::random_device r;
  	std::default_random_engine seeded_random_generator(r());
  	unsigned int rand = seeded_random_generator() % (stillValid.size());

	guesses = std::vector<std::string>();
	scores =  std::vector<std::string>();
	
	std::unordered_set<std::string>::const_iterator itr = stillValid.begin();
	unsigned int i = 0;
	while (i < rand) {
		itr++;
		i++;
	}
	theWord = *itr;
}

bool Wordle::makeGuess(const std::string &w) {
	std::string word = toUpperCase(w);
	if (dictionary.find(word) == dictionary.end() || guesses.size() >= maxGuesses) {
		return false;
	}
	addGuess(word);
	return true;
}

void Wordle::addGuess(const std::string &g) {
	std::string guess = toUpperCase(g);
	guesses.push_back(guess);
	std::string score = scoreGuess(guess);
	scores.push_back(score);

	std::unordered_set<std::string>::iterator itr = stillValid.begin();
	while (itr != stillValid.end()) {
		if (scoreGuess(guess, *itr) == score) {
			itr++;
		} else {
			itr = stillValid.erase(itr);
		}
	}
}

bool byValue(std::pair<std::string, unsigned int> a, std::pair<std::string, unsigned int> b) {
	return a.second < b.second;
}

void Wordle::outputBestWords() const {
	std::vector<std::pair<std::string, unsigned int>> scores;

	for (std::unordered_set<std::string>::const_iterator itr = dictionary.begin(); itr != dictionary.end(); itr++) {
		std::unordered_map<std::string, unsigned int> partitions;
		for (std::unordered_set<std::string>::const_iterator itr2 = stillValid.begin(); itr2 != stillValid.end(); itr2++) {
			partitions[scoreGuess(*itr, *itr2)]++;
		}
		unsigned int worstCase = 0;
		for (std::unordered_map<std::string, unsigned int>::const_iterator maxItr = partitions.begin(); maxItr != partitions.end(); maxItr++) {
			if (maxItr->second > worstCase) {
				worstCase = maxItr->second;
			}
		}
		scores.push_back(std::pair<std::string, unsigned int>(*itr, worstCase));
	}
	std::sort(scores.begin(), scores.end(), byValue);
	for (unsigned int i = 0; i < scores.size(); i++) {
		std::cout << scores[i].first << " " << scores[i].second << std::endl;
	}
}

void Wordle::makeGuess() {
	if (stillValid.size() == 1) {
		addGuess(*stillValid.begin());
		return;
	}
	unsigned int bestYet = dictionary.size();
	std::string bestWord = *stillValid.begin();
	for (std::unordered_set<std::string>::const_iterator itr = dictionary.begin(); itr != dictionary.end(); itr++) {
		std::unordered_map<std::string, unsigned int> partitions;
		for (std::unordered_set<std::string>::const_iterator itr2 = stillValid.begin(); itr2 != stillValid.end(); itr2++) {
			partitions[scoreGuess(*itr, *itr2)]++;
		}
		unsigned int worstCase = 0;
		for (std::unordered_map<std::string, unsigned int>::const_iterator maxItr = partitions.begin(); maxItr != partitions.end(); maxItr++) {
			if (maxItr->second > worstCase) {
				worstCase = maxItr->second;
			}
		}
		if (worstCase < bestYet) {
			bestYet = worstCase;
			bestWord = *itr;
		} else if (worstCase == bestYet && stillValid.find(*itr) != stillValid.end()) {
			bestYet = worstCase;
			bestWord = *itr;
		}
	}
	addGuess(bestWord);
}

bool Wordle::isOver() const {
	return gameWon() || guesses.size() >= maxGuesses;
}

bool Wordle::gameWon() const {
	if (theWord == "") { return false; }
	return (guesses.size() > 0 && guesses.back() == theWord) || (scores.size() > 0 && scores.back() == "ggggg");
}

bool Wordle::inDictionary(const std::string &s) const {
	std::string str = toUpperCase(s);
	return dictionary.find(str) != dictionary.end();
}

// need to implement
bool Wordle::addToBoard(const std::string &guess, const std::string &score) {
	guesses.push_back(guess);
	scores.push_back(score);
	std::unordered_set<std::string>::iterator itr = stillValid.begin();
	while (itr != stillValid.end()) {
		if (scoreGuess(guess, *itr) == score) {
			itr++;
		} else {
			itr = stillValid.erase(itr);
		}
	}
	if (stillValid.size() == 0) {
		return false;
	}
	return true;
}

void Wordle::printValidGuesses() const {
	std::cout << "There are " << stillValid.size() << " valid words" << std::endl;
	std::unordered_set<std::string>::const_iterator itr = stillValid.begin();
	for (unsigned int i = 0; i < 10 && i < stillValid.size(); i++) {
		std::cout << *itr << std::endl;
		itr++;
	}
}

void Wordle::printBoard() const {
	std::cout << "Guesses (" << guesses.size() << "/" << maxGuesses << ")" << "(" << stillValid.size() << " left)" /*<< "(word is " << theWord << " ;)"*/ << std::endl;
	for (unsigned int i = 0; i < guesses.size(); i++) {
		std::cout << guesses[i] << "\t" << scores[i] << std::endl;
	}
	if (guesses.size() >= maxGuesses) {
		std::cout << theWord << std::endl;
	}
	std::cout << std::endl;
}


std::string Wordle::scoreGuess(const std::string &g, const std::string &sW) const {
	std::string guess = toUpperCase(g);
	std::string secretWord = toUpperCase(sW);

	std::unordered_map<char, unsigned int> extras;

	std::string score = "_____";
	for (unsigned int i = 0; i < guess.size(); i++) {
		if (guess[i] == secretWord[i]) {
			score[i] = 'g';
		} else {
			extras[secretWord[i]]++;
		}
	}
	for (unsigned int i = 0; i < guess.size(); i++) {
		if (score[i] != 'g') {
			if (extras[guess[i]] > 0) {
				score[i] = 'y';
				extras[guess[i]]--;
			}
		}
	}
	return score;
}

std::string Wordle::scoreGuess(const std::string &g) const {
	return scoreGuess(g, theWord);
}

std::string toUpperCase(const std::string &s) {
	std::string retStr = "";
	for (unsigned int i = 0; i < s.size(); i++) {
		retStr += toupper(s[i]);
	}
	return retStr;
}