
// alloc memory based on the maxim length (max time)
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include "read_write_wav.h"
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

class NoteDuration {

public:
    int midiNote;
    float duration;

};


int main(int c, char** argv)
{
    const float duration = 3.5; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    const int _1_SEC = 1;
    float tempoMultiplier = 0.3;
    float tempo = 0;
    int midiShift = -3;
    // Scale midi


    std::vector<NoteDuration> musicNotesAndDuration = {
        {0, 1},
        {0, 2},
        {65, 0.5},
        {65, 0.5},
        
        {69, 1.5},
        {65, 0.5},
        {66, 1},
        {65, 1},

        {57, 1},
        {57, 1},
        {0, 1},
        {65, 0.5},
        {65, 0.5},

        {69, 1.5},
        {65, 0.5},
        {66, 1},
        {65, 1},

        {62, 3},
        {65, 0.5},
        {65, 0.5},               

        {69, 1.5},
        {65, 0.5},
        {66, 1},
        {65, 1},

        {69, 1},
        {65, 1},
        {66, 1},
        {65, 1},

        {69, 1},
        {65, 1},
        {66, 1},
        {69, 1},

        {70, 3},
    };

    for (int i = 0; i < musicNotesAndDuration.size(); i++)
    {

        NoteDuration noteDuration = musicNotesAndDuration[i];
        std::cout << "MIDI:" << noteDuration.midiNote << std::endl;
        
        //===============================
        SineOscillator* s1 = new SineOscillator(noteDuration.midiNote + midiShift, 0.5, Fs);
        float tempoStart = tempo;
        float tempoEnd = tempo + noteDuration.duration * tempoMultiplier;
        MusicNote m1(s1, tempoStart, tempoEnd);
        notes.push_back(m1);

        tempo = tempoEnd;
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
    string wav_name = "task1.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, maxPos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}