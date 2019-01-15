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

		// find mean sentiment score
		double mean = 0;
		int count = 0;
		int count_0 = 0;
		for (; count < u->coin_sents.size(); ++count)
		{
			if( u->coin_sents[count]==std::numeric_limits<double>::infinity() )
				u->unknown.push_back(count);
			else if( u->coin_sents[count]==0 )
				count_0++;
			else
				mean += u->coin_sents[count];	
		}
		if( u->unknown.size()==u->coin_sents.size() || count==count_0 ) // skip user if all coin sentiments are unknown
			continue;

		// calculate mean value
		mean /= (double)count;
		u->mean_score = mean;

		// assign mean value to unknown coins' score
		for (int i = 0; i < u->unknown.size(); ++i)
		{
			u->coin_sents[u->unknown[i]] = mean;
		}

		// normalization
		for (int i = 0; i < u->coin_sents.size(); ++i)
		{
			u->coin_sents[i] = u->coin_sents[i] / mean;
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


Hashlist * TweetsList::LSH_fill_wrapper(std::vector<Point *> * vec, int L) {
	// fill a vector of user-points
	for (int i = 0; i < users.size(); ++i)
	{
		double * array = new double[Point::d];
		for (int j = 0; j < Point::d; ++j)
		{
			array[j] = users[i]->coin_sents[j];
		}
		Point_double * p = new Point_double(array,to_string(users[i]->id));
		vec->push_back(p);
	}
	double loadfactor = (float)vec->size()/(float)pow(2.0,Hashtable::k);
	Hashlist * Hl = new Hashlist(L,loadfactor);


	// initialize hashtables
	for ( auto iter = Hl->list.begin(); iter != Hl->list.end(); ++iter ) // for each hashtable
	{
	  	(*iter)->fill(*vec,"cosine");
	}
	return Hl;
}

void TweetsList::predict_user(Hashlist * Hl, std::vector<Point *> vec, double (*metric_ptr)(Point *,Point *), int P) {
	for (int i = 0; i < users.size(); ++i) // for each user
	{
		std::vector<DPnt> nns;
		double R = 0.1;
		if( users[i]->unknown.size()!=0 ){ // if there are unknown values
			do {
				Hl->NN(vec[i],"cosine",metric_ptr,R,nns); // getting points in <=R distance
				R *= 10;
			}while( nns.size()<P );

			// predict uknown values
			for (int j = 0; j < users[i]->unknown.size(); ++j)
			{
				int index = users[i]->unknown[j];
				double value = 0;
				for (int n = 0; n < nns.size(); ++n)
				{
					Point * neigh = nns[n].first;
					double dist = metric_ptr(vec[i],neigh); 
					double * array = (double *)neigh->get_coords(); // get coordinates of point

					value += dist*array[index] / (double) P;
				}
				users[i]->coin_sents[index] = value;
			}

		}
	}

}

std::vector<int> maxes5(std::vector<DPnt> nns) {
	std::vector<int> coins;
	std::vector<double> vals;

	for (int i = 0; i < 5; ++i)
	{
		coins.push_back(0);
		vals.push_back(std::numeric_limits<double>::max());
	}

	for (int i = 0; i < nns.size(); ++i)
	{
		Point * neigh = nns[i].first;
		double * array = (double *)neigh->get_coords(); // get coordinates of point
		
		for (int j = 0; j < Point::d; ++j) // check all point values with all saved maxes
		{
			for (int maxi = 0; maxi < 5; ++maxi) // check if value is  higher than some already stored
			{
				int found =0;
				// check that coin is not already in max vextor
				for (int nm = 0; nm < coins.size(); ++nm)
				{
					if( coins[nm]==j ) found =1;
				}
				if( array[j]>vals[maxi] && !found ){
					vals[maxi] = array[j];
					coins[maxi] = j;
				}
			}
		}
	}

	return coins;
}

void TweetsList::recommend5(Cryptos& Cs, Hashlist * Hl, std::vector<Point *> * vec, double (*metric_ptr)(Point *,Point *), string file) {
	for (int i = 0; i < users.size(); ++i) // for each user
	{
		std::vector<Point *> nvec;
		for (int u = 0; u < vec->size(); ++u)
		{
			if( u!=i ) // every other user
				nvec.push_back((*vec)[u]);
		}

		ofstream outfile;
	  	outfile.open (file);

		std::vector<DPnt> nns;
		Hl->NN(nvec[i],"cosine",metric_ptr,1,nns); // getting points in <=R distance
		std::vector<int> maxes = maxes5(nns);
		outfile << users[i]->id;	
		for (int m = 0; m < maxes.size(); ++m)
		{
			outfile << Cs.get_name(maxes[m]);
		}
		outfile << endl;
		outfile.close();	
		
	}
}


void TweetsList::LSH_destroy(Hashlist * Hl) {
	delete(Hl);
}



