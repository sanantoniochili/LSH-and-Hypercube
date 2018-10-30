#include <fstream>
#include <string.h>
#include "Point.h"
#include "Hashtable.h"
#include "Hashlist.h"

int k = 4;
int L = 5;

double euclidean(Point *, Point *);
double cosine_similarity(Point *,Point *);

int main(int argc, char const *argv[])
{	
	
	for (int i=0 ; i<argc ; i++) {
		if( strcmp(argv[i],"L")==0 ){
			L = stoi(argv[i]);
		}
		if( strcmp(argv[i],"k")==0 ){
			k = stoi(argv[i]);
		}
    }
	string repeat;
	do {
	/*	string filename,line;
		cout << "Enter path of dataset:" << endl;
		cin >> filename;
	*/
	  	string filename = "../siftsmall/final_small";
	  	ifstream infile (filename);
	  	if ( !(infile.is_open()) ) {
	  		cout << "Unable to open file." << endl;
	        return -1;
	  	}

	  	Hashlist Hl(L,2.0); // create list of hashtables and fill with points
	  	vector<Point *> vec;

		string delimiter = " ";
		string number,line,metric;
		
		getline(infile,metric);

	    for (int i = 0; i < 100; ++i)
	    {
	    	getline(infile,line);
	    	size_t pos = 0;
	    	int * array = new int[Point::d];

	    	int count = 0;
	    	string name;
			while (((pos = line.find(delimiter)) != std::string::npos) && count <= Point::d) {
			    number = line.substr(0, pos);
			    if( count==0 ){
			    	name = number;
			    } else{
			    	array[count-1] = stoi(number);
			    }
			    count++;
			    line.erase(0, pos + delimiter.length());
			}

			Point_int * p = new Point_int(array,name); // creating point
			vec.push_back(p); // adding to vector of points

	    }
	    infile.close();


	    for ( auto iter = Hl.list.begin(); iter != Hl.list.end(); ++iter ) // for each hashtable
	    {
	    	(*iter)->fill(vec);
	    }

	    	string query_file, output_file;
		/*cout << "Enter path of query file:" << endl;
		cin >> query_file;*/
		cout << "Enter path of output file:" << endl;
		cin >> output_file;
		
	  	query_file = "../siftsmall/final_query";
	  	ifstream qinfile (query_file);
	  	if ( !(qinfile.is_open()) ) {
	  		cout << "Unable to open file." << endl;
	        return -1;
	  	}
	 	vector<Point *> queries;

		string qnumber,radius;
		
		getline(qinfile,radius);
		cout << radius << endl;

	    for (int i = 0; i < 100; ++i)
	    {
	    	getline(qinfile,line);
	    	size_t pos = 0;
	    	int * array = new int[Point::d];

	    	int count = 0;
	    	string name;
			while (((pos = line.find(delimiter)) != std::string::npos) && count <= Point::d) {
			    number = line.substr(0, pos);
			    if( count==0 ){
			    	name = number;
			    } else{
			    	array[count-1] = stoi(number);
			    }
			    count++;
			    line.erase(0, pos + delimiter.length());
			}

			Point_int * p = new Point_int(array,name); // creating point
			queries.push_back(p); // adding to vector of points

	    }

	    if( metric.compare("cosine")==0 ) {
		    for (int i = 0; i < queries.size(); ++i)
		    {
		    	Hl.NN(queries[i],&cosine_similarity);
		    }
		} else {
			for (int i = 0; i < queries.size(); ++i)
		    {
		    	Hl.NN(queries[i],&cosine_similarity);
		    }
		}
	    


	 	// free space
	 	for (int i = 0; i < vec.size(); ++i)
	 	{
	 		delete vec[i];
	 		delete queries[i];
	 	}

	 	cout << "Do you want to continue?(y or n)" << endl;
		cin >> repeat;
	}while( repeat.compare("y")==0 );
	return 0;
}

int Hashtable::w = 4;
int Hashtable::k = 5;
int Hashtable::d = 20; //128
int Point::d = 20; //128

double euclidean(Point * p1, Point * p2) {
	int * coords1 = (int *)p1->get_coords();
	int * coords2 = (int *)p2->get_coords();
	
	double sum = 0;
	for (int i = 0; i < Point::d; ++i)
	{
		sum += pow(coords1[i]-coords2[i],2);
	}

	return sum;
}

double cosine_similarity(Point * p1, Point * p2) {
	int * coords1 = (int *)p1->get_coords();
	int * coords2 = (int *)p2->get_coords();

	double product = 0;
	for (int i = 0; i < Point::d; ++i)
	{
		product += (coords1[i])*(coords2[i]);
	}

	return 1-(product/((p1->abs())*(p2->abs())));	
}