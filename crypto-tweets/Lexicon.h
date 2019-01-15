#ifndef _LEXICON_
#define _LEXICON_

#include <iostream>
#include <map>
#include <utility>   
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

typedef std::vector<string> Text;
typedef std::multimap<char, pair<string,double>>::iterator MMiter;

struct phrasecomp {
  bool operator() (const char& lhs, const char& rhs) const
  { return lhs<rhs; }
};

class Lexicon{
private:
	std::multimap<char, pair<string,double>, phrasecomp> lex;
public:
	Lexicon() {}
	~Lexicon();
	void add_phrase(string, double);
	void print();
	double search_score(string);	
};

class Cryptos{
private:
	std::vector<Text *> coins;
public:
	Cryptos();
	~Cryptos();
	int coin_no() { return coins.size(); }
	void add_coin(Text&);
	void print_coins();	
	int find_coin(string);
	string get_name(int index) { return (*coins[index])[0]; }
};

#endif