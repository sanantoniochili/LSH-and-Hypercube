#include <fstream>
#include <string>
#include "test.h"

int main(int argc, char const *argv[])
{	
/*	string filename,line;
	cout << "Enter path of dataset:" << endl;
	cin >> filename;
*/
  	string filename = "../siftsmall/input_small";
  	ifstream infile (filename);
  	if ( !(infile.is_open()) ) {
  		cout << "Unable to open file." << endl;
        return -1;
  	}

  	Hashlist Hl(2,2.0); // create list of hashtables and fill with points
  	vector<Point *> vec;


	string delimiter = " ";
	string number,line;
    for (int i = 0; i < 100; ++i)
    {
    	getline(infile,line);
    	size_t pos = 0;
    	int * array = new int[Point::d];

    	int count = 0;
		while (((pos = line.find(delimiter)) != std::string::npos) && count < Point::d) {
		    number = line.substr(0, pos);
		    array[count++] = stoi(number);
		    line.erase(0, pos + delimiter.length());
		}

		Point_int * p = new Point_int(array); // creating point
		vec.push_back(p); // adding to vector of points

    }
    infile.close();


    for ( auto iter = Hl.list.begin(); iter != Hl.list.end(); ++iter ) // for each hashtable
    {
    	(*iter)->fill(vec);
    	//(*iter)->print_table();
    }
   
	/*Hashlist Hl(2,2.0); // create list of hashtables and fill with points
	int count = 13;
	for ( auto iter = Hl.list.begin(); iter != Hl.list.end(); ++iter ) // for each hashtable
	{
		vector<Point *> vec;
	    for (int j = 0; j < 100; ++j)
	    {
	    	int * array = new int[Point::d];
			array[0] = 1;
			for (int i = 1; i < Hashtable::d; ++i)
			{
				array[i] = array[i-1]+j;	}
			
			Point_int * p1 = new Point_int(array); // creating point
			vec.push_back(p1); // adding to vector of points
	    }
	    (*iter)->fill(vec);
	}

	for ( auto iter = Hl.list.begin(); iter != Hl.list.end(); ++iter )
	{
	    (*iter)->print_table();
	}

 
 	/*for (int i = 0; i < vec.size(); ++i)
 	{
 		delete vec[i];
 	}*/
	return 0;
}

int Hashtable::w = 4;
int Hashtable::k = 5;
int Hashtable::d = 20; //128
int Point::d = 20; //128
