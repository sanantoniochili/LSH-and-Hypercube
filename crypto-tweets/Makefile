CC = g++
CFLAGS = -c -std=c++11

all : recommendation

recommendation : main.o Cluster.o Point.o Hashlist.o Lexicon.o Tweets.o
	$(CC) main.o Cluster.o Point.o Hashlist.o Hashtable.o Lexicon.o Tweets.o -o recommendation

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

Tweets.o : Tweets.h Tweets.cpp
	$(CC) $(CFLAGS) Tweets.cpp

Lexicon.o : Lexicon.h Lexicon.cpp
	$(CC) $(CFLAGS) Lexicon.cpp

Cluster.o : ../K-means/Cluster.cpp ../K-means/Cluster.h
	$(CC) $(CFLAGS) ../K-means/Cluster.cpp

Hashlist.o : ../LSH-and-Hypercube/Hashlist.cpp ../LSH-and-Hypercube/Hashlist.h Hashtable.o
	$(CC) $(CFLAGS) ../LSH-and-Hypercube/Hashlist.cpp

Hashtable.o : ../LSH-and-Hypercube/Hashtable.cpp ../LSH-and-Hypercube/Hashtable.h 
	$(CC) $(CFLAGS) ../LSH-and-Hypercube/Hashtable.cpp

Point.o : ../LSH-and-Hypercube/Point.cpp ../LSH-and-Hypercube/Point.h 
	$(CC) $(CFLAGS) ../LSH-and-Hypercube/Point.cpp

clean : 
	rm -rf *.o recommendation
