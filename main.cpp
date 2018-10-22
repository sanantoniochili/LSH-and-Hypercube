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


	Hashtable * Htable = new Hashtable();
	Htable->print_params();

	int * array = new int[Point::d];
	array[0] = 1;
	for (int i = 1; i < Hashtable::d; ++i)
	{
		array[i] = array[i-1]+10;	}
	
	Point_int p(array);
	Htable->hash(p);

	return 0;
}

int Hashtable::w = 4;
int Hashtable::k = 5;
int Hashtable::d = 10;
int Point::d = 10; 