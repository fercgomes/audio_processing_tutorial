#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../read_write_wav.h"
using namespace std;

// lets play with a audio effect
int main(int c, char** argv)
{

    float* audio_buffer;
    int buffer_len;
    int sample_rate=44100;
    int n_channels = 0;

    int channel=0;

    if (c!=2)
    {
        cout << "please, use a valid wav file name." << endl;
        return 0;
    }

    cout << "reading wav file..." << endl;
    n_channels = read_wave_file(argv[1], audio_buffer, &buffer_len, &sample_rate);

    for (int k=0; k<n_channels; k++)
    {

        cout << "alloc memory for channel: " << k << endl;
        float* buffer = (float*) malloc(buffer_len/n_channels*sizeof(float));

        // global amplitude 
        float amplitude = 0.7f;
        for (int i=0; i<buffer_len; i+=n_channels)
        {
            buffer[i/n_channels] = audio_buffer[i] * amplitude; 
        }

        int ch_buffer_len = buffer_len/n_channels;


        // =========================================================
        // reverb/delay (feeback delay)
        int delayMilliseconds = 500; // half a second
        int delaySamples = 
            (int)((float)delayMilliseconds * 44.1f); // assumes 44100 Hz sample rate
        //float decay = 0.2f;
        float decay = 0.8f;
        for (int i = 0; i < ch_buffer_len - delaySamples; i++)
        {
            // WARNING: overflow potential
            buffer[i + delaySamples] += ((float)buffer[i] * decay);
        }

        // =========================================================
        // create output file names
        string ch_file_name(argv[1]);
        ch_file_name.replace(ch_file_name.length()-4, 4, "_ch_");
        ch_file_name += std::to_string(k)  + ".wav";
        cout << "ch wave file: " << ch_file_name << endl;
        // save left channel
        write_wave_file (ch_file_name.c_str(), buffer, ch_buffer_len, sample_rate);

    }

    cout << "done." << endl;
    return 0;
}

