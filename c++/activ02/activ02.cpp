#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../read_write_wav.h"
using namespace std;

// step2: let's split a stereo wav file into two mono wave files.
int main(int c, char** argv)
{
  
    float* audio_buffer;
    int buffer_len;
    int sample_rate=44100;
    int n_channels = 0;

    if (c!=2)
    {
        cout << "please, use a valid wav file name." << endl;
        return 0;
    }

    cout << "reading wav file..." << endl;
    n_channels = read_wave_file(argv[1], audio_buffer, &buffer_len, &sample_rate);

    cout << "alloc memory for L|R mono audio channels..." << endl;
    float* right = (float*) malloc(buffer_len/2*sizeof(float));
    float* left = (float*) malloc(buffer_len/2*sizeof(float));

    
    cout << "copying stereo into two mono..." << endl;
    if (n_channels!=2)
    {

        cout << "please, use stereo wav file." << endl;
        return 0;
    }
    for (int i=0; i<buffer_len; i+=2)
    {
       left[i/2] = audio_buffer[i]; 
       right[i/2+1] = audio_buffer[i+1]; 
    }

    // create output file names
    string left_file_name(argv[1]);
    left_file_name.replace(left_file_name.length()-4, 9, "_left.wav");
    cout << "left wave file: " << left_file_name << endl;
    // save left channel
    write_wave_file (left_file_name.c_str(), left, buffer_len/2, sample_rate);
    
    string right_file_name = argv[1];
    right_file_name.replace(right_file_name.length()-4, 9, "_right.wav");
    cout << "right wave file: " << left_file_name << endl;
    // save right channel
    write_wave_file (right_file_name.c_str(), right, buffer_len/2, sample_rate);

    cout << "done." << endl;
    return 0;
}

