#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "../read_write_wav.h"
using namespace std;

// lets generate a oscillator with a single pure sine wave 
int main(int c, char** argv)
{
    const float duration = 3.5; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float audio_buffer[buffer_len];

    // OSCILLATOR
    // sine wave
    float freq1 = 261.62 * 1; // C3 // Midi note 48
    float freq2 = 261.62 * 2; // C4 // Midi note 60
    float freq3 = 261.62 * 3; // C5 // Midi note 72
    float freq4 = 261.62 * 4; // C6 // Midi note 84

    // s1 = sin(2*pi*freq*t);
    float amp1 = 0.05f;    
    float amp2 = 0.25f;    
    float amp3 = 0.2f;    
    float amp4 = 0.05f;    
    for (int n=0; n<buffer_len; n++)
    {
       audio_buffer[n] = amp1 * sin(2*M_PI*freq1*(n/Fs)) + amp2 * sin(2*M_PI*freq2*(n/Fs))  + amp3 * sin(2*M_PI*freq3*(n/Fs)) + amp4 * sin(2*M_PI*freq4*(n/Fs)); 
    }

    string wav_name = "oscillator_" + std::to_string(freq1) + "Hz.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, buffer_len, Fs);
    
    cout << "done." << endl;
    return 0;
}

