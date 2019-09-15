CC=g++

all: clean task1 task2

task1:
	g++ -std=c++11 -I../c++/sndfile -L../c++/sndfile -I../activ05/ -lsndfile task1.cpp -o task1 

task2:
	g++ -std=c++11 -I../c++/sndfile -L../c++/sndfile -I../activ05/ -lsndfile task2.cpp -o task2

print_midi_table:
	g++ -std=c++11 -I../c++/sndfile -L../c++/sndfile -I../c++/activ04/ -lsndfile ../c++/activ04/print_freq_table.cpp -o ../c++/activ04/print_midi_table

clean:
	rm task1
	rm task2

run_and_play:
	./task1 && vlc task1.wav
run_and_play2:
	./task2 && vlc task2.wav

play: all run_and_play
play2: all run_and_play2
