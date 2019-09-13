#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include "../read_write_wav.h"
using namespace std;


float midi2freq(int m)
{
    return 440*pow(2,(m-69.0f)/12.0f);
}

float freq2midi(float freq)
{
    return 12*log2( freq/440) + 69;
}



class Oscillator   
{
    public:

        virtual void process(float*, int)
        {
        }
};


class SineOscillator : public Oscillator
{
    public: 

        float freq;
        float amp;
        float Fs;


        SineOscillator(int nota_midi, float _amp, float _Fs)
        {
            freq = midi2freq(nota_midi);
            amp = _amp;
            Fs = _Fs;
        }

        ~SineOscillator()
        {
        }   

        void process(float* audio_buffer, int buffer_len) 
        {
            float c=0;
            int ramp_size = 1000;
            cout << "sine class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;
            for (int n=0; n<buffer_len; n++)
            {

                audio_buffer[n] += (c/ramp_size)*amp * sin(2*M_PI*freq*(((float)n)/Fs)); 
                if (n<ramp_size-1) c++; 
                if (n>=buffer_len-ramp_size) c--;
            }
        }

};

class MusicNote
{
    public:
        float start_time; // pos in seconds
        float end_time; // pos in seconds
        Oscillator* osc;   

    MusicNote(Oscillator *o, float s, float e):osc(o), start_time(s), end_time(e)
    {
    }
};



int main(int c, char** argv)
{
    const float duration = 30; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    float tempo=0;

    int array_midi[]={
        55,55,57,55,60,59,59,
        55,55,57,55,62,60,60,
        55,55,67,65,64,60,59,
        65,65,64,60,62,60,60
        };
    float times[]={
        1,1,2,1,1,1,1,
        1,1,2,1,1,1,1,
        1,1,2,1,1,1,1,
        1,1,2,1,1,1,2};
    
    for (int i=0; i<(int) sizeof(array_midi)/sizeof(int); i++)
    {
        float end = tempo + ((float) times[i]) * .3;
        SineOscillator* s1 = new SineOscillator(array_midi[i], 0.8, Fs);
        MusicNote m1(s1, tempo,end);
        notes.push_back(m1);
        tempo=end;
    }

    //===============================
    // get max signal duration
    int maxPos = 0;
    for (int k=0; k<notes.size(); k++)
    {
        maxPos = std::max((float)maxPos, (float)round(notes[k].end_time*Fs));
    }
    audio_buffer = new float[maxPos];
    memset(audio_buffer, 0, maxPos);

    cout << "maxPos: " << maxPos <<  endl;
    // write the notes into the audio buffer
    for (int k=0; k<notes.size(); k++)
    {
        int startPos = notes[k].start_time*Fs;
        int endPos = notes[k].end_time*Fs;
        cout << "startPos: " << startPos << endl;
        cout << "endPos: " << endPos << endl;
        cout << "opa" << endl;
        notes[k].osc->process(audio_buffer + startPos, endPos-startPos);
    }

    // ============================
    // save output wave
    string wav_name = "melody_rand.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, maxPos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}



