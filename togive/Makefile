CC = g++
CFLAGS = -c -std=c++11

all : lsh cube

cube : main_cube.o Hashtable.o Point.o Hashlist.o
	$(CC) main_cube.o Hashtable.o Point.o Hashlist.o -o cube

lsh : main_lsh.o Hashtable.o Point.o Hashlist.o
	$(CC) main_lsh.o Hashtable.o Point.o Hashlist.o -o lsh

main_cube.o : main_cube.cpp
	$(CC) $(CFLAGS) main_cube.cpp

main_lsh.o : main_lsh.cpp
	$(CC) $(CFLAGS) main_lsh.cpp

Hashlist.o : Hashlist.cpp Hashlist.h
	$(CC) $(CFLAGS) Hashlist.cpp

Hashtable.o : Hashtable.cpp Hashtable.h 
	$(CC) $(CFLAGS) Hashtable.cpp

Point.o : Point.cpp Point.h 
	$(CC) $(CFLAGS) Point.cpp


clean : 
	rm lsh cube