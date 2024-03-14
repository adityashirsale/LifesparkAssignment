#include <iostream>
#include <Windows.h>
#include <vector>
#include "gnuplot-iostream.h"

#pragma comment(lib, "winmm.lib")

const int NUMPTS = 58000 * 4.5;
short int waveIn[NUMPTS];


void PlotGraph(const std::vector<short>& samples) {
    Gnuplot gp; // Create a gnuplot instance
    gp << "plot '-' with lines\n";
    gp.send1d(samples);
}

void StartRecord() {
    int sampleRate = 58000;

    HWAVEIN hWaveIn;
    MMRESULT result;

    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = WAVE_FORMAT_PCM;
    pFormat.nChannels = 1;
    pFormat.nSamplesPerSec = sampleRate;
    pFormat.nAvgBytesPerSec = sampleRate * 2;
    pFormat.nBlockAlign = 2;
    pFormat.wBitsPerSample = 16;
    pFormat.cbSize = 0;

    result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

    WAVEHDR WaveInHdr;
    WaveInHdr.lpData = (LPSTR)waveIn;
    WaveInHdr.dwBufferLength = NUMPTS * 2;
    WaveInHdr.dwBytesRecorded = 0;
    WaveInHdr.dwUser = 0L;
    WaveInHdr.dwFlags = 0L;
    WaveInHdr.dwLoops = 0L;
    waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

    result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

    result = waveInStart(hWaveIn);

    std::cout << "Recording..." << std::endl;

    Sleep(4.5 * 1000);

    waveInClose(hWaveIn);

    PlayRecord();

    std::vector<short> recordedSamples(waveIn, waveIn + NUMPTS);

    PlotGraph(recordedSamples);
}


int main() {
    StartRecord();
    return 0;
}

