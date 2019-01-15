#include <fstream>
#include <string.h>
#include <chrono>
#include <sstream>
#include "Cluster.h"
#include "../LSH-and-Hypercube/Point.h"

#define K 10

int Point::d = 20; //128
int Hashtable::k = 10;
int Hashtable::w = 4;
int Hashtable::d = 20; //128
int L = 10;

double euclidean(Point *, Point *);
double cosine_similarity(Point *, Point *);


int main (int argc, char const *argv[]) {
	string filename,metric;
	int iter = 0;

	for (int i=0 ; i<argc ; i++) {
		if( strcmp(argv[i],"-i")==0 ){
			filename = argv[++i];
			cout << "defined dataset:" << filename << "\t";
		}    
		if( strcmp(argv[i],"-d")==0 ){
			metric = argv[++i];
			cout << "defined metric:" << metric << "\t";
		}
		if( strcmp(argv[i],"-c")==0 ){
			iter = stoi(argv[++i]);
			cout << "defined iterations:" << iter << "\t";
		} 
	}
	cout << endl;
	string delimiter = ",";
	string number,line;

	// default metric is euclidean
	double (*metric_ptr)(Point *,Point *) = &euclidean;
	if ( metric[0]=='c' ) // for cosine similarity
	{
	  	metric_ptr = &cosine_similarity;
	}
		
	ifstream infile (filename);
	if ( !(infile.is_open()) ) {
		cout << "Unable to open file." << endl;
	    return -1;
	}

	Clusters * Cs = new Clusters(K); // create new structure containing points and clusters' centres

	getline(infile,line); // get first line to count dimensions
	std::stringstream ss(line);
	std::vector<double> v;
	double no;
	while (ss >> no)
    {
        v.push_back(no);

        if (ss.peek() == ',')
            ss.ignore();
    }
    Point::d = v.size();
    double * array = new double[Point::d]; 
    for (int i = 0; i < Point::d; ++i)
    {
    	array[i] = v[i];
    }
    Point_double * p = new Point_double(array,"1"); // creating point   
    Cs->add_point(p); // adding to vector of points

    int countlines = 2; // rest of points
	while (getline(infile,line)){

	   	// creating a vector of input points
	   	double * array = new double[Point::d]; // every point includes an array of coordinates
	   	std::stringstream ss(line);
		double no;
		int count = 0;
		while (ss >> no)
	    {
	        array[count++] = no;

	        if (ss.peek() == ',')
	            ss.ignore();
	    }
		
		Point_double * p = new Point_double(array,to_string(countlines++)); // creating point
		Cs->add_point(p); // adding to vector of points
	}
	infile.close();
	Cs->N = countlines-1;

	// ----------- LSH Config

	// ----------- Hypercube Config



	clock_t begin,end,begin_e,end_e;
	//Cs->iter2(iter,metric_ptr);
	//Cs->print_points();

	std::vector<Point *> * vec = new vector<Point *>;
	Cs->kmeanspp(metric_ptr);
	Hashlist * Hl = Cs->LSH_fill(vec,L,metric);
	Cs->LSH_assign(Hl,metric,metric_ptr);



	delete Cs;
	delete Hl;
	delete vec;
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

