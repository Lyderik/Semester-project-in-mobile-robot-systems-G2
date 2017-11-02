#pragma once
#include "portaudio.h"
class TestBuffer
{
public:
	TestBuffer();
	~TestBuffer();
	PaStream *stream;
	bool open(PaDeviceIndex index);
	bool close();
	bool start();
	bool stop();
private:
	static int sampleRate;
	char message[20];
	int paCallbackMethod(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags);
	static int paCallback(const void *inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
	static void paStreamFinished(void * userData);
	void paStreamFinishedMethod();
};

