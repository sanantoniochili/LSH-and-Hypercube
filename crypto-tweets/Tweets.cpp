#include <limits>
#include "Tweets.h"

void TweetsList::add_tweet(int uid, int tid, Text& text) {
	Tweet * t = new Tweet();
	t->uid = uid;
	t->tid = tid;
	for (int i = 0; i < text.size(); ++i)
	{
		(t->text)->push_back(text[i]); // add text of tweet as vector of terms
	}

	tweets.insert(std::pair<int,Tweet *>(uid,t));
}

TweetsList::~TweetsList() {
	for (std::multimap<int,Tweet *>::iterator it=tweets.begin(); it!=tweets.end(); ++it)
	{
		delete(it->second);
	}
	for (int i = 0; i < users.size(); ++i)
	{
		delete(users[i]);
	}
}

void TweetsList::calculate_scores(Lexicon * L, double alpha) {
	double total = 0;

	for (std::multimap<int,Tweet *>::iterator it=tweets.begin(); it!=tweets.end(); ++it)
	{
		Text t = *(it->second->text);
		for (int j = 0; j < t.size(); ++j)
		{
			total += L->search_score(t[j]);
		}
		total = total / sqrt(pow(total,2.0) + alpha);
		it->second->score = total; // save sentiment result of tweet
	}
	
}

void TweetsList::print_scores() {
	for (std::multimap<int,Tweet *>::iterator it=tweets.begin(); it!=tweets.end(); ++it)
	{
		cout << it->second->score << endl;
	}
}

void TweetsList::assign_cryptos(Cryptos& Cs) {
	for (std::multimap<int,Tweet *>::iterator it=tweets.begin(); it!=tweets.end(); ++it)
	{
		Text text = *(it->second->text);
		for (int i = 0; i < text.size(); ++i)
		{
			int index = Cs.find_coin(text[i]);
			if( index>=0 ) {
				it->second->coins.push_back(index);
			}
		}
	}
}

void TweetsList::print_tweets() {
	for( Titer it = tweets.begin(), end = tweets.end(); it != end; it = tweets.upper_bound(it->first))
	{
		cout << "User: " << it->first << endl;
	    std::pair <Titer, Titer> ret;
	    ret = tweets.equal_range(it->first);
	    for (Titer iter=ret.first; iter!=ret.second; ++iter)
		{
			cout << " Tweet id: " << iter->second->tid;
			Text text = *(iter->second->text);
			for (int i = 0; i < text.size(); ++i)
			{
				cout << " " << text[i];
			}
			cout << endl;
		}
	}
}

void TweetsList::coins_per_user(int no_of_coins) {
	for( Titer it = tweets.begin(), end = tweets.end(); it != end; it = tweets.upper_bound(it->first)) { // for each user
		User * u = new User(it->first,no_of_coins);
		for (int i = 0; i < no_of_coins; ++i) // set infinity for initial value of unknown sentiment
		{
			u->coin_sents.push_back(std::numeric_limits<double>::infinity());
		}
		std::pair <Titer, Titer> ret = tweets.equal_range(it->first); // search tweets per user
	    for (Titer iter=ret.first; iter!=ret.second; ++iter)
		{
			Tweet * tuit = iter->second;
			for (int i = 0; i < tuit->coins.size(); ++i) // for each mentioned coin of each tweet of user 
			{
				int index = tuit->coins[i];
				if( u->coin_sents[index]==std::numeric_limits<double>::infinity() ){
					u->coin_sents[index] = tuit->score; // set firstvalue
				}
				else { 
					u->coin_sents[index] += tuit->score; // add sentiment
				}
			}
		}
		users.push_back(u);
	}
}

void TweetsList::print_user_scores() {
	for (int ui = 0; ui < users.size(); ++ui){ // for each user
		User * u = users[ui];

		cout << "User id: " << u->id << " Totals: "; 
		for (int i = 0; i < u->coin_sents.size(); ++i)
		{
			if( u->coin_sents[i]!=std::numeric_limits<double>::infinity() )
				cout << " " << u->coin_sents[i]; 

		}
		cout << endl;
	}
}