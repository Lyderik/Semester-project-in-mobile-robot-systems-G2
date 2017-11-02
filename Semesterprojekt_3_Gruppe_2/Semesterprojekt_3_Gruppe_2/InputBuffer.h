#pragma once
#include "portaudio.h"
#include <vector>
#include <math.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

class InputBuffer
{
public:
	InputBuffer();
	~InputBuffer();
	PaStream *stream;
	bool open(PaDeviceIndex index);
	bool close();
	bool start();
	bool stop();

private:
	int tempcounter;
	static const int DTMF_FREQ[8];
	int pointer;
	bool filled;
	int sampleBufferSize;
	int bufferSize;
	static int sampleRate;
	std::vector<float> buffer;
	char message[20];
	int paCallbackMethod(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags);
	static int paCallback(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData);
	static void paStreamFinished(void * userData);
	void paStreamFinishedMethod();
	std::vector<float> goertzel_mag();
};

