#include <fstream>
#include <string.h>
#include <chrono>
#include "Point.h"
#include "Hashtable.h"
#include "Hashlist.h"

using namespace std::chrono; 

int Hashtable::k = 100;
int Hashtable::w = 10;
int Hashtable::d = 20; //128
int Point::d = 20; //128
int Mp = 10;
int probes = 10;

double euclidean(Point *, Point *);
double cosine_similarity(Point *,Point *);
DPnt NN_exhaustive(Point *,double(*metric)(Point *,Point *),double,vector<DPnt>&,vector<Point *>&);

int main(int argc, char const *argv[])
{	
	string filename = "";
	string repeat,query_file = "";
	string output_file = "";

	// reading flags
	for (int i=0 ; i<argc ; i++) {
		if( strcmp(argv[i],"-M")==0 ){
			Mp = stoi(argv[++i]);
			cout << "defined M:" << Mp << "\t";
		}
		if( strcmp(argv[i],"-probes")==0 ){
			probes = stoi(argv[++i]);
			cout << "defined probes:" << probes << "\t";
		}
		if( strcmp(argv[i],"-k")==0 ){
			Hashtable::k = stoi(argv[++i]);
			cout << "defined k:" << Hashtable::k << "\t";
		}
		if( strcmp(argv[i],"-d")==0 ){
			filename = argv[++i];
			cout << "defined dataset:" << filename << "\t";
		}
		if( strcmp(argv[i],"-q")==0 ){
			query_file = argv[++i];
			cout << "defined query_file:" << query_file << "\t";
		}
		if( strcmp(argv[i],"-o")==0 ){
			output_file = argv[++i];
			cout << "defined output_file:" << output_file << "\t";
		}
    }
    cout << endl;
	do {
		//filename = "../siftsmall/final_small";
		if( filename.compare("")==0 ) {
			cout << "Enter path of dataset:" << endl;
			cin >> filename;
		}
	
	  	ifstream infile (filename);
	  	if ( !(infile.is_open()) ) {
	  		cout << "Unable to open file." << endl;
	        return -1;
	  	}

	  	vector<Point *> vec;

		string delimiter = " ";
		string number,line,metric;
		
		getline(infile,metric);

		int countlines = 0;
	    while (getline(infile,line))
	    {

	    	size_t pos = 0;
	    	if( countlines==0 ) {
	    		int count = 0;
	    		while (((pos = line.find(delimiter)) != std::string::npos)) {
				    number = line.substr(0, pos);
				    count++;
				    line.erase(0, pos + delimiter.length());
				}
				Point::d = count-1;
				Hashtable::d = count-1;
				countlines++;
	    	}

	    	pos = 0;
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
	    filename = "";

	  //  Cube * C = new Cube((float)vec.size()/(float)Hashtable::k);
	    Cube * C = new Cube((float)vec.size()/(float)pow(2.0,Hashtable::k));
	    C->fill(vec,metric);
     
	    // ask for files if not provided
	    if( query_file.compare("")==0 ){
			cout << "Enter path of query file:" << endl;
			cin >> query_file;
		}
		if( output_file.compare("")==0 ){
			cout << "Enter path of output file:" << endl;
			cin >> output_file;
		}
		
	  	ifstream qinfile (query_file);
	  	if ( !(qinfile.is_open()) ) {
	  		cout << "Unable to open query file." << endl;
	        return -1;
	  	}
	 	vector<Point *> queries;
		string qnumber,radius,radline;
		size_t pos = 0;

		getline(qinfile,radline);
		for (int i = 0; i < 2; ++i)
		{
		 	pos = radline.find(delimiter);
			radius = radline.substr(0, pos);
			radline.erase(0, pos + delimiter.length());
		}
		double R = stod(radius);
		cout << "Radius: " << R << endl;

		// get query points
	    while (getline(qinfile,line))
	    {
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
	    qinfile.close();
	    query_file = "";

	    std::ofstream outfile (output_file);
	    std::vector<DPnt> nns,e_nns;
	    DPnt nn,e_nn;
	    clock_t begin,end,begin_e,end_e;

	   	// default metric is euclidean
	    double (*metric_ptr)(Point *,Point *) = &euclidean;
	    if ( metric[0]=='c' ) // for cosine similarity
	    {
	    	metric_ptr = &cosine_similarity;
	    }
	    for (int i = 0; i < queries.size(); ++i)
	    {
	    	// searching for nearest neighbours
  			begin = clock();
			nn = C->NN(queries[i],metric,metric_ptr,probes,Mp,R,nns);
			end = clock();

			begin_e = clock();
			e_nn = NN_exhaustive(queries[i],metric_ptr,R,e_nns,vec);
			end_e = clock();
			// printing to output file
			outfile << "Query: " << queries[i]->get_name() << std::endl;
			outfile << "R-near neighbors:" << std::endl;
			for (int i = 0; i < nns.size(); ++i)
			{
				if( nns[i].first!=NULL ) outfile << nns[i].first->get_name() << std::endl;
			}
			if( nn.first!=NULL ) {
				outfile << "distanceCube: " << (double)nn.second << endl; 
				outfile << "Nearest neighbor: " << nn.first->get_name() << endl;
			}
			if( e_nn.first!=NULL ) outfile << "distanceTrue: " << (double)e_nn.second << endl;
			outfile << "tCube: " << double(end - begin) / CLOCKS_PER_SEC << endl;
			outfile << "tTrue: " << double(end_e - begin_e) / CLOCKS_PER_SEC << endl;
	    }
		outfile.close();
		output_file = "";

	 	// free space
	 	delete C;
	 	for (int i = 0; i < vec.size(); ++i)
	 	{
	 		delete vec[i];
	 	}
	 	for (int i = 0; i < queries.size(); ++i)
	 	{
	 		delete queries[i];
	 	}

	 	cout << "Do you want to continue?(y or n)" << endl;
		cin >> repeat;
	}while( repeat.compare("y")==0 );
	return 0;
}

double euclidean(Point * p1, Point * p2) {
	int * coords1 = (int *)p1->get_coords();
	int * coords2 = (int *)p2->get_coords();
	
	double sum = 0;
	for (int i = 0; i < Point::d; ++i)
	{
		sum += pow(coords1[i]-coords2[i],2);
	}
	sum = sqrt(sum);
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
	return (product/(double)((p1->abs())*(p2->abs())));	
}

DPnt NN_exhaustive(Point * query,double(*metric)(Point *,Point *),double R,vector<DPnt>& nns,vector<Point *>& points) {
	DPnt min(NULL,-1);
	for (int i = 0; i < points.size(); i++){
	    double temp = metric(query,points[i]);
	    if( i==0 ) {
	    	min.first = points[i];
	    	min.second = temp;
	    } else {
		    if( temp<min.second ) {
		    	min.first = points[i];
		    	min.second = temp;
		    	nns.push_back(min);
		    }
		    if( temp<=R ) {
		    	nns.push_back(make_pair(points[i],temp));
		    }
		}
		/*cout << "Distance " << query->get_name();
		cout << "-" << neighbour->get_name();
		cout << ": " << temp << endl;*/
	}

	/*if( min.first ) {
		cout << "NN of bucket: " << min.first->get_name();
		min.first->print_coords();
	} */
	return min;

}
