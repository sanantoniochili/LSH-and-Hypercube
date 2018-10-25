#include <fstream>
#include <string>
#include "test.h"

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
	string number,line;
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
    	//(*iter)->print_table();
    	//cout << (*iter)->get_bucketn() << endl;
    }
    for ( auto iter = Hl.list.begin(); iter != Hl.list.end(); ++iter ) // for each hashtable
    {
    	(*iter)->print_table();
    }
 
 	for (int i = 0; i < vec.size(); ++i)
 	{
 		delete vec[i];
 	}
	return 0;
}

int Hashtable::w = 4;
int Hashtable::k = 5;
int Hashtable::d = 20; //128
int Point::d = 20; //128
