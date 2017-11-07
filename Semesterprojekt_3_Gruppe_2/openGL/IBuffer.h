#pragma once
#include "portaudio.h"
#include <vector>
#include <math.h>
#include <complex>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

class IBuffer
{
public:
	IBuffer();
	IBuffer(int bz);
	~IBuffer();
	PaStream *stream;
	bool open(PaDeviceIndex index);
	bool close();
	bool start();
	bool stop();
	std::vector<float> getFFT();

private:
	std::vector<float> fft;
	std::complex<float>* fftBuffer;
	std::complex<float>* fftOut;
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
	void separate(std::complex<float>* a, int n);
	void fft2(std::complex<float>* X, int N);
};


