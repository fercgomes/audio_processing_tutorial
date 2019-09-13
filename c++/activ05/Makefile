CC=g++
CFLAGS=-I../sndfile/ -std=c++11
LDFLAGS= -lsndfile -L../sndfile/

all:
	$(CC) -o melody melody.cpp $(CFLAGS) $(LDFLAGS)
	$(CC) -o melody_scale melody_scale.cpp $(CFLAGS) $(LDFLAGS)
	$(CC) -o random_melody random_melody.cpp $(CFLAGS) $(LDFLAGS)

