#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include "../c++/read_write_wav.h"
using namespace std;



float midi2freq(int m)
{
    return 440*pow(2,(m-69.0f)/12.0f);
}

float freq2midi(float freq)
{
    return 12*log2( freq/440) + 69;
}



class SoundProcessor  
{
    public:

        virtual void process(float*, int)
        {
        }
};


class SamplerPlayback : public SoundProcessor
{
    public: 

        float freq;
        float amp;
        int Fs;
        float* wav_buffer;
        int n_channels;
        int wav_len;

        SamplerPlayback(int nota_midi, float _amp, string wavFileName)
        {
            freq = midi2freq(nota_midi);
            amp = _amp;
            cout << "reading wav file..." << endl;
            float* buffer;
            n_channels = read_wave_file(wavFileName.c_str(), buffer, &wav_len, &Fs);
            wav_buffer = (float*) malloc(wav_len/n_channels*sizeof(float));
            // copy left channel to internal buffer
            for (int i=0; i<wav_len; i+=n_channels)
            {
                 wav_buffer[i/n_channels] = buffer[i]; 
            }
            // update the length since we are using only left channel
            wav_len = wav_len/n_channels;
            delete [] buffer;
        }

        ~SamplerPlayback()
        {
        }   

        void process(float* audio_buffer, int buffer_len) 
        {
            float c=0;
            int ramp_size = 1000;
            cout << "sampler class -> call process: "<<  buffer_len << " amp: "<<  amp << " freq: "<< freq << "  Fs: " << Fs << endl;
            for (int n=0; n<buffer_len; n++)
            {
               //TODO hop size for adjust pitch
                float wavSample = wav_buffer[ n % wav_len];
                audio_buffer[n] += (c/ramp_size)*amp * wavSample; 
                // fadein/fadeout
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
        SoundProcessor* sampler;   

    MusicNote(SoundProcessor *o, float s, float e):sampler(o), start_time(s), end_time(e)
    {
    }
};



int main(int c, char** argv)
{
    const float duration = 3.5; //seconds
    const float Fs = 44100; //sample rate (samples /second)
    const int buffer_len = round(duration*Fs); // samples
    float *audio_buffer;
    vector<MusicNote> notes;

    std::vector<std::string> samples = {
        // "./samples/A5.mp3",
        // "./samples/D5.mp3",
        // "./samples/E5.mp3",
        // "./samples/G5.mp3",
        // "./samples/s.mp3",
        "./samples/s.wav",
        "./samples/tenniscourt.wav",
        "./samples/tuffbreak.wav",
        "./samples/s.wav",
        "./samples/tenniscourt.wav",
        "./samples/tenniscourt.wav",
        "./samples/tenniscourt.wav",
        "./samples/tuffbreak.wav",
    };

    float currentTime = 0.5;

    //===============================
    for (std::string sample : samples) {
        SamplerPlayback* s1 = new SamplerPlayback(60, 0.8, sample);
        auto const endTime = currentTime + 1;
        MusicNote m1(s1, currentTime, endTime);
        currentTime = endTime;
        notes.push_back(m1);
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
        notes[k].sampler->process(audio_buffer + startPos, endPos-startPos);
    }

    // ============================
    // save output wave
    string wav_name = "task2.wav";
    write_wave_file (wav_name.c_str(), audio_buffer, maxPos, Fs);
    cout << "done." << endl;
    delete [] audio_buffer;
    return 0;
}



