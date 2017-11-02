#include "TestBuffer.h"
#include <iostream>

int TestBuffer::sampleRate = 44100;

TestBuffer::TestBuffer()
{
}


TestBuffer::~TestBuffer()
{
}

bool TestBuffer::open(PaDeviceIndex index)
{
	PaStreamParameters outputParameters;

	outputParameters.device = index;
	if (outputParameters.device == paNoDevice) {
		return false;
	}

	const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
	if (pInfo != 0)
	{
		printf("Output device name: '%s'\n", pInfo->name);
	}

	outputParameters.channelCount = 1;       /* mono output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighInputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	PaError err = Pa_OpenStream(
		&stream,
		&outputParameters,
		0, /* no input */
		sampleRate,
		512,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		&TestBuffer::paCallback,
		this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
	);

	if (err != paNoError)
	{
		/* Failed to open stream to device !!! */
		std::cout << "Failed to open stream to device" << std::endl;
		return false;
	}

	err = Pa_SetStreamFinishedCallback(stream, &TestBuffer::paStreamFinished);

	if (err != paNoError)
	{
		Pa_CloseStream(stream);
		stream = 0;

		return false;
	}

	return true;
}

bool TestBuffer::close()
{
	if (stream == 0)
		return false;

	PaError err = Pa_CloseStream(stream);
	stream = 0;

	return (err == paNoError);
}


bool TestBuffer::start()
{
	if (stream == 0)
		return false;

	PaError err = Pa_StartStream(stream);

	return (err == paNoError);
}

bool TestBuffer::stop()
{
	if (stream == 0)
		return false;

	PaError err = Pa_StopStream(stream);

	return (err == paNoError);
}
int TestBuffer::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags)
{
	float *in = (float*)inputBuffer;
	unsigned long i;

	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;
	(void)outputBuffer;

	for (i = 0; i<framesPerBuffer; i++)
	{
		std::cout << in[i] << std::endl;
	}

	return paContinue;

}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int TestBuffer::paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	std::cout << "paCallback" << std::endl;
	/* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
	we called Pa_OpenStream with 'this' for userData */
	return ((TestBuffer*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
		framesPerBuffer,
		timeInfo,
		statusFlags);
}


void TestBuffer::paStreamFinishedMethod()
{
	printf("Stream Completed: %s\n", message);
}

/*
* This routine is called by portaudio when playback is done.
*/
void TestBuffer::paStreamFinished(void* userData)
{
	return ((TestBuffer*)userData)->paStreamFinishedMethod();
}
