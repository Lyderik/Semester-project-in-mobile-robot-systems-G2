#include "TestBuffer.h"
#include <iostream>
#include <iomanip> 

int TestBuffer::sampleRate = 44100;
const int TestBuffer::DTMF_FREQ[8] = { 697, 770, 852, 941, 1209, 1336, 1447, 1633 };

TestBuffer::TestBuffer()
{
	sampleBufferSize = 256;
	bufferSize = 4 * sampleBufferSize;
	buffer = std::vector<float>(bufferSize);
	fftBuffer = new std::complex<float>[bufferSize];
	fftOut = new std::complex<float>[bufferSize];
	tempcounter = 0;

}

TestBuffer::~TestBuffer()
{

}

bool TestBuffer::open(PaDeviceIndex index)
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
		&TestBuffer::paCallback,
		this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
	);


	if (err != paNoError)
	{
		/* Failed to open stream to device !!! */
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
	(void)outputBuffer;
	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;

	const float * in = (const float*)inputBuffer;
	int endStarFrame = 3 * sampleBufferSize;
	for (int i = 0; i < endStarFrame; i++)
		fftBuffer[i] = fftBuffer[i + sampleBufferSize];
	for (int i = 0; i<framesPerBuffer; i++)
		fftBuffer[endStarFrame + i] = *(in + i);
	for (int i = 0; i < bufferSize; i++)
		fftOut[i] = fftBuffer[i];
	
	fft2(fftOut, bufferSize);
	int frqz[8] = { 16,18,20,22,28,31,34,38 };
	std::cout << std::fixed; 
	for (int f = 0; f < 8; f++) {
		std::cout << std::setw(10) << std::setprecision(2) << abs(fftOut[frqz[f]]);
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
int TestBuffer::paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
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

	

std::vector<float> TestBuffer::goertzel_mag() {
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

// separate even/odd elements to lower/upper halves of array respectively.
// Due to Butterfly combinations, this turns out to be the simplest way 
// to get the job done without clobbering the wrong elements.
void TestBuffer::separate(std::complex<float>* a, int n) {
	std::complex<float>* b = new std::complex<float>[n / 2];  // get temp heap storage
	for (int i = 0; i<n / 2; i++)    // copy all odd elements to heap storage
		b[i] = a[i * 2 + 1];
	for (int i = 0; i<n / 2; i++)    // copy all even elements to lower-half of a[]
		a[i] = a[i * 2];
	for (int i = 0; i<n / 2; i++)    // copy all odd (from heap) to upper-half of a[]
		a[i + n / 2] = b[i];
	delete[] b;                 // delete heap storage
}

// N must be a power-of-2, or bad things will happen.
// Currently no check for this condition.
//
// N input samples in X[] are FFT'd and results left in X[].
// Because of Nyquist theorem, N samples means 
// only first N/2 FFT results in X[] are the answer.
// (upper half of X[] is a reflection with no new information).
void TestBuffer::fft2(std::complex<float>* X, int N) {
	if (N < 2) {
		// bottom of recursion.
		// Do nothing here, because already X[0] = x[0]
	}
	else {
		separate(X, N);      // all evens to lower half, all odds to upper half
		fft2(X, N / 2);   // recurse even items
		fft2(X + N / 2, N / 2);   // recurse odd  items
								  // combine results of two half recursions
		for (int k = 0; k<N / 2; k++) {
			std::complex<float> e = X[k];   // even
			std::complex<float> o = X[k + N / 2];   // odd
												// w is the "twiddle-factor"
			std::complex<float> w = exp(std::complex<float>(0, -2.*M_PI*k / N));
			X[k] = e + w * o;
			X[k + N / 2] = e - w * o;
		}
	}
}