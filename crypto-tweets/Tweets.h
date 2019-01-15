#ifndef _TWEETS_
#define _TWEETS_

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cmath>
#include "Lexicon.h"
#include "../LSH-and-Hypercube/Hashlist.h"
#include "../K-means/Cluster.h"


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
	double mean_score;
	std::vector<int> unknown;
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
	Hashlist * LSH_fill_wrapper(std::vector<Point *> *, int);
	void LSH_destroy(Hashlist * Hl);
	void predict_user(Hashlist * Hl, std::vector<Point *> vec, double (*metric_ptr)(Point *,Point *), int P);
	void recommend5(Cryptos& Cs, Hashlist * Hl, std::vector<Point *> * vec, double (*metric_ptr)(Point *,Point *),string);
};

typedef std::multimap<int, Tweet *>::iterator Titer;

#endif