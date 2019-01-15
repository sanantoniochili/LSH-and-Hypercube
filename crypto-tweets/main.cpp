#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include "Lexicon.h"
#include "Tweets.h"
#include "../LSH-and-Hypercube/Hashlist.h"
#define ALPHA 15

int Point::d = 20; //128
int Hashtable::k = 10;
int Hashtable::w = 4;
int Hashtable::d = 20; //128
int L = 10;

double euclidean(Point *, Point *);
double cosine_similarity(Point *, Point *);

int main (int argc, char const *argv[]) {
	string input_filename,out_filename,lex_filename = "vader_lexicon.csv";
	string crypto_filename = "coins_queries.csv";
	string line,delimiter = ",";
	int iter = 0;

	for (int i=0 ; i<argc ; i++) {
		if( strcmp(argv[i],"-d")==0 ){
			input_filename = argv[++i];
			cout << "defined input file: " << input_filename << "\t";
		}    
		if( strcmp(argv[i],"-o")==0 ){
			out_filename = argv[++i];
			cout << "defined output file:" << out_filename << "\t";
		}
	}

	cout << endl;
		
	ifstream lexfile (lex_filename);
	if ( !(lexfile.is_open()) ) {
		cout << "Unable to open file." << endl;
	    return -1;
	}

	// create lexicon
	Lexicon * Lex = new Lexicon();

	while( getline(lexfile,line) ) {
		std::stringstream ss(line);
		
		std::vector<string> buf;
		string token;
		while (ss >> token)
	    {
	    	buf.push_back(token);
	        if (ss.peek() == ',')
	            ss.ignore();
	    }
	    double val = stod(buf[buf.size()-1]);
	    Lex->add_phrase(buf[0],val);

	}
	lexfile.close();

	ifstream infile (input_filename);
	if ( !(infile.is_open()) ) {
		cout << "Unable to open file." << endl;
	    return -1;
	}

	// reading tweets
	TweetsList TuitL;

	getline(infile,line); // get first line
	int uid,tid,neighs = stoi(line.substr(0, line.find(delimiter)));

	while( getline(infile,line) ) {
		std::vector<string> buf;
		string token;
		std::string delimiter = ",";

		size_t pos = 0;
		int count = 0;
		while ((pos = line.find(delimiter)) != std::string::npos) {
		    token = line.substr(0, pos);
		    line.erase(0, pos + delimiter.length());
		    if( count==0 ) uid = stoi(token);
		    else if( count==1 ) tid = stoi(token);
		    else buf.push_back(token);
		    count++;
		}
	    Text phrase;
	    for (int i = 0; i < buf.size()-1; ++i)
	    {
	   		phrase.push_back(buf[i]);
	    }
	    TuitL.add_tweet(uid,tid,phrase);	   

	}
	infile.close();


	// reading coins
	ifstream cyfile (crypto_filename);
	if ( !(cyfile.is_open()) ) {
		cout << "Unable to open file." << endl;
	    return -1;
	}

	Cryptos Cs;

	while( getline(cyfile,line) ) {

		std::vector<string> buf;
		string token;
		std::string delimiter = "\t";

		size_t pos = 0;
		while ((pos = line.find(delimiter)) != std::string::npos) {
		    token = line.substr(0, pos);
		    line.erase(0, pos + delimiter.length());
		    buf.push_back(token);
		}
	    Cs.add_coin(buf);	   

	}
	cyfile.close();

	//Cs.print_coins();
	TuitL.assign_cryptos(Cs);
	//TuitL.print_tweets();
	TuitL.calculate_scores(Lex,ALPHA);
	//TuitL.print_scores();
	TuitL.coins_per_user(Cs.coin_no());
	//TuitL.print_user_scores();

	Point:: d = Cs.coin_no();
	Hashtable::d = Cs.coin_no();

	//TuitL.print_user_scores();
	std::vector<Point *> * vec = new vector<Point *>();
	Hashlist * Hl = TuitL.LSH_fill_wrapper(vec,L);
	TuitL.predict_user(Hl,*vec,&cosine_similarity,neighs);
	TuitL.recommend5(Cs,Hl,vec,&cosine_similarity,out_filename);

	TuitL.LSH_destroy(Hl);
	delete(vec);
	delete(Lex);
    return 0;
}

double euclidean(Point * p1, Point * p2) {
	double * coords1 = (double *)p1->get_coords();
	double * coords2 = (double *)p2->get_coords();

	double sum = 0;
	for (int i = 0; i < Point::d; ++i)
	{
		sum += pow(coords1[i]-coords2[i],2);
	}
	sum = sqrt(sum);
	return sum;
}

double cosine_similarity(Point * p1, Point * p2) {
	double * coords1 = (double *)p1->get_coords();
	double * coords2 = (double *)p2->get_coords();

	double product = 0;
	for (int i = 0; i < Point::d; ++i)
	{
		product += (coords1[i])*(coords2[i]);
	}

	return (double)1-(product/(double)((p1->abs())*(p2->abs())));	
}

