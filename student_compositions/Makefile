CC=g++

all: clean task1

task1:
	g++ -std=c++11 -I../sndfile -L../sndfile -I../activ05/ -lsndfile task1.cpp -o task1 

print_midi_table:
	g++ -std=c++11 -I../sndfile -L../sndfile -I../activ04/ -lsndfile ../activ04/print_freq_table.cpp -o ../activ04/print_midi_table

clean:
	rm task1

run_and_play:
	./task1 && vlc task1.wav

play: clean task1 run_and_play
