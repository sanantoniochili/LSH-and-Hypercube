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
  	string line;

  	ifstream infile (filename);
  	if ( !(infile.is_open()) ) {
  		cout << "Unable to open file." << endl;
        return -1;
  	}

	string delimiter = " ";
	string number;
    for (int i = 0; i < 2; ++i)
    {
    	getline(infile,line);
    	size_t pos = 0;
		while ((pos = line.find(delimiter)) != std::string::npos) {
		    number = line.substr(0, pos);
		    //std::cout << number << std::endl;
		    line.erase(0, pos + delimiter.length());
		}

    }
    infile.close();

	int * array = new int[Point::d];
	array[0] = 1;
	for (int i = 1; i < Hashtable::d; ++i)
	{
		array[i] = array[i-1]+10;	}
	
	Point_int p1(array);
	Point_int p2(array);
	Point_int p3(array);
	Point_int p4(array);
	Point_int p5(array);
	vector<Point *> vec;
	vec.push_back(&p1);
	vec.push_back(&p2);
	vec.push_back(&p3);
	vec.push_back(&p4);
	vec.push_back(&p5);
	Hashtable * Htable = new Hashtable();
	Htable->fill(vec);
	Htable->print_table();
 

	return 0;
}

int Hashtable::w = 4;
int Hashtable::k = 5;
int Hashtable::d = 10;
int Point::d = 10; 