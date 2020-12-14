#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include "MyComplex.h"

class WaveFile
{
private:
    WAVEFORMATEX header;
    char* data;
    int data_size;
    int sample_count;//采样数量，每组左右通道各计为一个

    struct Block
    {
        char sym[4];
        unsigned int data_size;
        char* data;
    };
    std::vector<Block> otherBlocks;
public:
    WaveFile(int in_rate = 44100, int in_channels = 2, int in_bits = 16);
    WaveFile(std::string filename);
    WaveFile(const WaveFile& another);
    ~WaveFile();

    WAVEFORMATEX* GetHeader();
    char* GetData();
    int GetDataSize();

    void Clear();

    //add an another audio to this
    void Add(const WaveFile& another);

    void Add(char* new_data, unsigned int new_data_size);

    std::vector<Complex> GetComplex(int channel, int sample_start, int length);

    void SetFromComplex(std::vector<Complex>& cp, int channel, int group_start, int length);

    //do convert
    void ConvertToMono();
    void CloneMonoToStereo();

    //do resample
    void Resample(int new_sample_rate);

    //repeat
    void Repeat(int times);

    //change the speed
    void ChangeSpeed(double speed);

    void Mute();

    void AddSin(double freq,double amp=1.0,double phase_second=0);

    //High pass or Low pass filter
    void Filter(bool isHighPass, double freq);

    void SaveToFile(std::string newFileName);

    friend std::ostream& operator<<(std::ostream& out, const WaveFile& wav);

    friend class WaveOut;
};

//print out the recording details of a given file
std::ostream& operator<<(std::ostream &out,const WaveFile& wav);
