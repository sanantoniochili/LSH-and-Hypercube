#include "Lexicon.h"

void Lexicon::add_phrase(string word, double val) {
	lex.insert(make_pair(word[0],make_pair(word,val)));
}

void Lexicon::print() {
	  for (std::multimap<char, pair<string,double>, phrasecomp>::iterator it=lex.begin(); it!=lex.end(); ++it) 
	  {
	  	cout << "Key: " << it->first << " Phrase: " << it->second.first << " Value: " << it->second.second << endl;
	  }
}

double Lexicon::search_score(string word) { 
	// It returns a pair representing the range of elements with key equal to 'c'
	std::pair<MMiter, MMiter> result = lex.equal_range(word[0]);

	// Iterate over the range
	for (auto it = result.first; it != result.second; it++){
		if( word.compare(it->second.first)==0 ){ // check if word is in the vocabulary
			return it->second.second; // return value
		}
	}
	return 0.0;
}

Lexicon::~Lexicon() {
	lex.clear();
}

Cryptos::Cryptos() {}
Cryptos::~Cryptos() {
	for (int i = 0; i < coins.size(); ++i)
	{
		delete(coins[i]);
	}
}

void Cryptos::add_coin(Text& text) {
	Text * ntext = new Text();
	for (int i = 0; i < text.size(); ++i)
	{
		ntext->push_back(text[i]);
	}
	coins.push_back(ntext);
}

void Cryptos::print_coins() {
	cout << "Printing coins lexicon..." << endl;
	for (int i = 0; i < coins.size(); ++i)
	{
		Text text = *(coins[i]);
		for (int j = 0; j < text.size(); ++j)
		{
			cout << " " << text[j];
		}
		cout << endl;
	}
}

int Cryptos::find_coin(string word) {
	for (int i = 0; i < coins.size(); ++i)
	{
		Text text = *(coins[i]);
		for (int j = 0; j < text.size(); ++j)
		{
			if( word.compare(text[j])==0 ) {// if word belongs to crypto lexicon
				return i; // return index in vector of cryptocoins
			}
		}
	}
	return -1;
}