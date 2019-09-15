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
    const float duration = 18; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    float tempo=0;
    // ==========================================================
    // MAKE YOUR MUSIC HERE!!!!!
    // Right Hand
    int i;
    for (i = 0; i < 15; i++)
    {
        SineOscillator* s1 = new SineOscillator(76,0.5,Fs);
        MusicNote m1(s1, i, i + 0.25);
        notes.push_back(m1);

        SineOscillator* s2 = new SineOscillator(75,0.5,Fs);
        MusicNote m2(s2, i + 0.25, i + 0.5);
        notes.push_back(m2);

        SineOscillator* s3 = new SineOscillator(73,0.5,Fs);
        MusicNote m3(s3, i + 0.5, i + 0.75);
        notes.push_back(m3);

        SineOscillator* s4 = new SineOscillator(75,0.5,Fs);
        MusicNote m4(s4, i + 0.75, i + 1);
        notes.push_back(m4);
    }

    SineOscillator* s1 = new SineOscillator(76,0.5,Fs);
    MusicNote m1(s1, i, i + 0.25);
    notes.push_back(m1);

    SineOscillator* s2 = new SineOscillator(75,0.5,Fs);
    MusicNote m2(s2, i + 0.25, i + 0.5);
    notes.push_back(m2);

    SineOscillator* s3 = new SineOscillator(71,0.5,Fs);
    MusicNote m3(s3, i + 0.5, i + 2.5);
    notes.push_back(m3);
    //=============
    // Left Hand
    //=============
    SineOscillator* s4 = new SineOscillator(49,0.5,Fs);
    MusicNote m4(s4, 4, 4.25);
    notes.push_back(m4);

    SineOscillator* s5 = new SineOscillator(56,0.5,Fs);
    MusicNote m5(s5, 4.25, 4.75);
    notes.push_back(m5);

    SineOscillator* s6 = new SineOscillator(64,0.5,Fs);
    MusicNote m6(s6, 4.75, 5);
    notes.push_back(m6);
    //----------
    SineOscillator* s9 = new SineOscillator(63,0.5,Fs);
    MusicNote m9(s9, 5, 5.25);
    notes.push_back(m9);

    SineOscillator* s7 = new SineOscillator(61,0.5,Fs);
    MusicNote m7(s7, 5.25, 5.5);
    notes.push_back(m7);

    SineOscillator* s8 = new SineOscillator(59,0.5,Fs);
    MusicNote m8(s8, 5.5, 8);
    notes.push_back(m8);
    //----------
    SineOscillator* s10 = new SineOscillator(47,0.5,Fs);
    MusicNote m10(s10, 8, 8.75);
    notes.push_back(m10);

    SineOscillator* s11 = new SineOscillator(54,0.5,Fs);
    MusicNote m11(s11, 8.75, 9.5);
    notes.push_back(m11);

    SineOscillator* s12 = new SineOscillator(61,0.5,Fs);
    MusicNote m12(s12, 9.5, 10);
    notes.push_back(m12);

    SineOscillator* s13 = new SineOscillator(45,0.5,Fs);
    MusicNote m13(s13, 10, 10.75);
    notes.push_back(m13);

    SineOscillator* s14 = new SineOscillator(52,0.5,Fs);
    MusicNote m14(s14, 10.75, 11.5);
    notes.push_back(m14);

    SineOscillator* s15 = new SineOscillator(59,0.5,Fs);
    MusicNote m15(s15, 11.5, 12.5);
    notes.push_back(m15);

    SineOscillator* s16 = new SineOscillator(64,0.25,Fs);
    MusicNote m16(s16, 13, 13.25);
    notes.push_back(m16);

    SineOscillator* s17 = new SineOscillator(63,0.5,Fs);
    MusicNote m17(s17, 13.25, 13.5);
    notes.push_back(m17);

    SineOscillator* s18 = new SineOscillator(61,0.5,Fs);
    MusicNote m18(s18, 13.5, 13.75);
    notes.push_back(m18);

    SineOscillator* s19 = new SineOscillator(59,0.5,Fs);
    MusicNote m19(s19, 13.75, 14.75);
    notes.push_back(m19);
    //----------
    SineOscillator* s20 = new SineOscillator(64,0.5,Fs);
    MusicNote m20(s20, 14.75, 15);
    notes.push_back(m16);

    SineOscillator* s21 = new SineOscillator(63,0.5,Fs);
    MusicNote m21(s21, 15, 15.25);
    notes.push_back(m21);

    SineOscillator* s22 = new SineOscillator(61,0.5,Fs);
    MusicNote m22(s22, 15.25, 15.5);
    notes.push_back(m22);

    SineOscillator* s23 = new SineOscillator(59,0.5,Fs);
    MusicNote m23(s23, 15.5, 18);
    notes.push_back(m23);

    // ==========================================================

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
    string wav_name = "lucas_romagnoli_melody.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, maxPos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}
