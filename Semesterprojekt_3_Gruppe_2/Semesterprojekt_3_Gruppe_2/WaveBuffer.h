#pragma once

#include <vector>
#include <math.h>
#include "portaudio.h"

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

class WaveBuffer
{
public:
	WaveBuffer();
	~WaveBuffer();
	int getBufferLength();
	bool put(int b);
	double  getNext();
	PaStream *stream;
	bool open(PaDeviceIndex index);
	bool close();
	bool start();
	bool stop();

	
	
private:
	int iPointer;
	int oPointer;
	int sPointer;
	int waveNumOfSamples;
	int bufferSize;
	double phase;
	static int sampleRate;
	static double piProduct(int freq);
	static int findLoDTMFFreq(int d);
	static int findHiDTMFFreq(int d);
	const static double piProducts[16][2];
	std::vector<int> buffer;
	char message[20];
	int paCallbackMethod(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags);
	static int paCallback(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData);
	static void paStreamFinished(void * userData);
	void paStreamFinishedMethod();
};

