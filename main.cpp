#include <fstream>
#include <string>
#include "Point.h"
#include "Hashtable.h"
#include "Hashlist.h"

double euclidean(Point *, Point *);

int main(int argc, char const *argv[])
{	
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

  	Hashlist Hl(2,2.0); // create list of hashtables and fill with points
  	vector<Point *> vec;

	string delimiter = " ";
	string number,line,metric;
	
	getline(infile,metric);
	cout << metric << endl;

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

    /*	string query_file;
	cout << "Enter path of query file:" << endl;
	cin >> query_file;
	*/
  	string query_file = "../siftsmall/final_query";
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

    for ( auto iter = Hl.list.begin(); iter != Hl.list.end(); ++iter ) // for each hashtable
    {
    	//(*iter)->print_table();
    	for (int i = 0; i < queries.size(); ++i)
    	{
    		(*iter)->NN(queries[i],&euclidean);
    	}
    }


 	// free space
 	for (int i = 0; i < vec.size(); ++i)
 	{
 		delete vec[i];
 		delete queries[i];
 	}
	return 0;
}

int Hashtable::w = 4;
int Hashtable::k = 5;
int Hashtable::d = 20; //128
int Point::d = 20; //128

double euclidean(Point * p1, Point * p2) {
	return 0;
}