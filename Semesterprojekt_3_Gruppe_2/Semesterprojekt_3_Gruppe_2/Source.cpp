#include <stdio.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include "WaveBuffer.h"

#define NUM_SECONDS   (4)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif


class ScopedPaHandler
{
public:
	ScopedPaHandler()
		: _result(Pa_Initialize())
	{
	}
	~ScopedPaHandler()
	{
		if (_result == paNoError)
		{
			Pa_Terminate();
		}
	}

	PaError result() const { return _result; }

private:
	PaError _result;
};


/*******************************************************************/
int main(void)
{

	WaveBuffer sine;

	//printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

	ScopedPaHandler paInit;
	if (paInit.result() != paNoError) goto error;

	if (sine.open(Pa_GetDefaultOutputDevice()))
	{
		if (sine.start())
		{
			while (true) {
				std::cout << "type in random shit to generate tones from:\n";
				std::string tones;
				std::getline(std::cin, tones);
				if (tones == "exit") break;
				for (int i = 0; i < tones.length(); i++) {
					sine.put((signed char) tones[i] % 16);
				}
			}
			sine.stop();
		}

		sine.close();
	}

	printf("Test finished.\n");
	system("PAUSE");
	return paNoError;

error:
	fprintf(stderr, "An error occured while using the portaudio stream\n");
	fprintf(stderr, "Error number: %d\n", paInit.result());
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(paInit.result()));

	system("PAUSE");
	
	return 0;
}