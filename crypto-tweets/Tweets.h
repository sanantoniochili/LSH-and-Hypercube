#ifndef _TWEETS_
#define _TWEETS_

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include "Lexicon.h"

using namespace std;

struct Tweet{
	int uid;
	int tid;
	Text * text;
	double score;
	std::vector<int> coins;

	Tweet() { text = new Text(); }
	~Tweet() { delete(text); }
};

struct User{
	int id;
	std::vector<double> coin_sents;

	User(int nid, int no_of_coins) { 
		this->id = nid;
		coin_sents.reserve(no_of_coins); 
	}
};

class TweetsList{
private:
	std::multimap<int,Tweet *> tweets; // map tweets to users
	std::vector<User *> users;

public:
	TweetsList() {}
	~TweetsList();
	void add_tweet(int,int,Text&);
	void calculate_scores(Lexicon *,double);
	void print_scores();
	void assign_cryptos(Cryptos&);	
	void print_tweets();
	void coins_per_user(int);
	void print_user_scores();
};

typedef std::multimap<int, Tweet *>::iterator Titer;

#endif