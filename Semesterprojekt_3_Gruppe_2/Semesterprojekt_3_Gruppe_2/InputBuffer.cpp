#include "InputBuffer.h"
#include <iostream>
#include <iomanip> 

int InputBuffer::sampleRate = 8000;
const int InputBuffer::DTMF_FREQ[8] = { 669, 770, 852, 941, 1209, 1336, 1447, 1633 };

InputBuffer::InputBuffer()
{
	sampleBufferSize = 120;
	bufferSize = 5* sampleBufferSize;
	buffer = std::vector<float>(bufferSize);
	tempcounter = 0;
	
}

InputBuffer::~InputBuffer()
{

}

bool InputBuffer::open(PaDeviceIndex index)
{
	PaStreamParameters inputParameters;

	inputParameters.device = index;
	if (inputParameters.device == paNoDevice) {
		return false;
	}

	const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
	if (pInfo != 0)
	{

		printf("Output device name: '%s'\n", pInfo->name);
		std::cout << pInfo->maxInputChannels << std::endl;
		std::cout << pInfo->maxOutputChannels << std::endl;
		std::cout << pInfo->defaultSampleRate << std::endl;
	}

	inputParameters.channelCount = 1;       /* mono input */
	inputParameters.sampleFormat = paFloat32; /* 32 bit floating point input */
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	PaError err = Pa_OpenStream(
		&stream,
		&inputParameters, 
		NULL, /* no output */
		sampleRate,
		sampleBufferSize,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		&InputBuffer::paCallback,
		this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
	);


	if (err != paNoError)
	{
		/* Failed to open stream to device !!! */
		return false;
	}

	err = Pa_SetStreamFinishedCallback(stream, &InputBuffer::paStreamFinished);

	if (err != paNoError)
	{
		Pa_CloseStream(stream);
		stream = 0;

		return false;
	}

	return true;
}

bool InputBuffer::close()
{
	if (stream == 0)
		return false;

	PaError err = Pa_CloseStream(stream);
	stream = 0;

	return (err == paNoError);
}


bool InputBuffer::start()
{
	if (stream == 0)
		return false;

	PaError err = Pa_StartStream(stream);
	return (err == paNoError);
}

bool InputBuffer::stop()
{
	if (stream == 0)
		return false;

	PaError err = Pa_StopStream(stream);

	return (err == paNoError);
}
int InputBuffer::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags)
{
	(void)outputBuffer;
	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;

	const float * in = (const float*)inputBuffer;
	for (int i = 0; i<framesPerBuffer; i++)
	{	
		//std::cout << *in++ << "\n";
		//buffer[4 * sampleBufferSize + i] = in[i];
		buffer[i] = *(in + i);
	}
	std::vector<float> gm = goertzel_mag();
	for (int f = 0; f < 8; f++) {
		if (gm[f] < 0.015) gm[f] = 0.0;
		std::cout << std::setw(12) << gm[f] << " ";
	}
	std::cout << "\r";
	//if (tempcounter++ > 4) {
	//	int i = 0;
	//}
	return paContinue;

}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int InputBuffer::paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	/* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
	we called Pa_OpenStream with 'this' for userData */
	return ((InputBuffer*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
		framesPerBuffer,
		timeInfo,
		statusFlags);
}


void InputBuffer::paStreamFinishedMethod()
{
	printf("Stream Completed: %s\n", message);
}

/*
* This routine is called by portaudio when playback is done.
*/
void InputBuffer::paStreamFinished(void* userData)
{
	return ((InputBuffer*)userData)->paStreamFinishedMethod();
}


/*float InputBuffer::goertzel_mag(int freq) {
	int     k, i;
	float   floatnumSamples;
	float   omega, sine, cosine, coeff, q0, q1, q2, magnitude, real, imag;

	float  scalingFactor = bufferSize / 2.0;

	floatnumSamples = (float)bufferSize;
	k = (int)(0.5 + ((floatnumSamples * freq) / sampleRate));
	omega = (2.0 * M_PI * k) / floatnumSamples;
	sine = sin(omega);
	cosine = cos(omega);
	coeff = 2.0 * cosine;
	q0 = 0;
	q1 = 0;
	q2 = 0;

	for (i = 0; i<(bufferSize); i++)
	{
		q0 = coeff * q1 - q2 + buffer[i];
		q2 = q1;
		q1 = q0;
		if (sampleBufferSize * 4 > i) 
			buffer[i] = buffer[i + sampleBufferSize];
	}

	// calculate the real and imaginary results
	// scaling appropriately
	real = (q1 - q2 * cosine) / scalingFactor;
	imag = (q2 * sine) / scalingFactor;

	magnitude = sqrt(real*real + imag*imag);
	return magnitude;
}*/

std::vector<float> InputBuffer::goertzel_mag() {
	int     k[8];
	float   floatnumSamples;
	float   omega[8], sine[8], cosine[8], coeff[8], q0[8], q1[8], q2[8];
	float  scalingFactor = bufferSize / 2.0;
	std::vector<float> magnitude(8);

	floatnumSamples = (float)bufferSize;
	for (int f = 0; f < 8; f++) {
		k[f] = (int)(0.5 + ((floatnumSamples * DTMF_FREQ[f]) / sampleRate));
		omega[f] = (2.0 * M_PI * k[f]) / floatnumSamples;
		sine[f] = sin(omega[f]);
		cosine[f] = cos(omega[f]);
		coeff[f] = 2.0 * cosine[f];
		q0[f] = 0;
		q1[f] = 0;
		q2[f] = 0;
	}
	for (int i = 0; i<(bufferSize); i++)
	{
		float d = buffer[i];
		for (int f = 0; f < 8; f++) {
			q0[f] = coeff[f] * q1[f] - q2[f] + d;
			q2[f] = q1[f];
			q1[f] = q0[f];
		}
		if (sampleBufferSize * 4 > i)
			buffer[i] = buffer[i + sampleBufferSize];
	}

	// calculate the real and imaginary results
	// scaling appropriately
	for (int f = 0; f < 8; f++) {
		float real = (q1[f] - q2[f] * cosine[f]) / scalingFactor;
		float imag = (q2[f] * sine[f]) / scalingFactor;

		magnitude[f] = sqrt(real*real + imag*imag);
	}
	return magnitude;
}
