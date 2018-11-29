#include <fstream>
#include <string.h>
#include <chrono>
#include "Cluster.h"
#include "../LSH-and-Hypercube/Point.h"

#define K 10

int Point::d = 20; //128

int main (int argc, char const *argv[]) {
	string filename;

	for (int i=0 ; i<argc ; i++) {
		if( strcmp(argv[i],"-i")==0 ){
			filename = argv[++i];
			cout << "defined dataset:" << filename << "\t";
		}    }
	cout << endl;
	string delimiter = ",";
	string number,line,metric;
		
	// read name of metric used
	//getline(infile,metric);

	/*if( filename.compare("")==0 ) {
		cout << "Enter path of dataset:" << endl;
		cin >> filename;
	}*/
filename = "twitter_dataset_small_v2.csv";	
	ifstream infile (filename);
	if ( !(infile.is_open()) ) {
		cout << "Unable to open file." << endl;
	    return -1;
	}

	Clusters * Cs = new Clusters(K); // create new structure containing points and clusters' centres

	int countlines = 0;
int test = 0;
	while (getline(infile,line)){

	   	size_t pos = 0;
	   	if( countlines==0 ) { // first read to count dimensions
	   		int count = 0;
	   		while (((pos = line.find(delimiter)) != std::string::npos)) {
			    number = line.substr(0, pos);
			    count++;
			    line.erase(0, pos + delimiter.length());
			}
			Point::d = count-1;
			countlines++;
	   	}
	   	// creating a vector of input points
	   	pos = 0;
	   	double * array = new double[Point::d]; // every point includes an array of coordinates
	   	int count = 0;
	   	string name;
		while (((pos = line.find(delimiter)) != std::string::npos) && count <= Point::d) {
		    number = line.substr(0, pos);
		    if( count==0 ){
		    	name = number; // first string is point's id
		    } else{
		    	array[count-1] = stod(number);
		    }
		    count++;
		    line.erase(0, pos + delimiter.length());
		}
		Point_double * p = new Point_double(array,name); // creating point
		Cs->add_point(p); // adding to vector of points
	}
	infile.close();

	//Cs->print_points();
	Cs->rand_centr();
	Cs->print_centroids();
	Cs->kmeanspp();

	delete Cs;
	return 0;
}