#include <stdio.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include "WaveBuffer.h"
#include "InputBuffer.h"


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


int main(void)
{
	
	/*int err = Pa_Initialize();
	const   PaDeviceInfo *deviceInfo;
	int numDevices = Pa_GetDeviceCount();
	for (int i = 0; i<numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		std::cout << deviceInfo->name << std::endl;
		std::cout << deviceInfo->maxInputChannels << std::endl;
	}
	system("PAUSE");*/
	
	ScopedPaHandler paInit;
	if (paInit.result() == paNoError) {
		std::cout << "Maaaaah" << std::endl;
		InputBuffer IBuffer;
		WaveBuffer wBuffer(100);

		if (IBuffer.open(Pa_GetDefaultInputDevice()) && wBuffer.open(Pa_GetDefaultOutputDevice()))
		{
			if (IBuffer.start() && wBuffer.start()) 
			{
				while (true) {
					std::cout << "type in exit to exit:\n";
					std::string tones;
					std::getline(std::cin, tones);
					if (tones == "exit") break;
					for (int i = 0; i < tones.length(); i++) {
						unsigned char t = (unsigned char)tones[i];
						wBuffer.put(t % 16);
						wBuffer.put(t / 16);
					}
				}
				IBuffer.stop();
				wBuffer.stop();
			}
			IBuffer.close();
			wBuffer.close();
		}

		printf("Test finished.\n");
		//system("PAUSE");
	}
	/*
	WaveBuffer wBuffer(100);

	ScopedPaHandler paInit;
	if (paInit.result() == paNoError) {

		if (wBuffer.open(Pa_GetDefaultOutputDevice()))
		{
			if (wBuffer.start())
			{
				while (true) {
					std::cout << "type in random shit to generate tones from:\n";
					std::string tones;
					std::getline(std::cin, tones);
					if (tones == "exit") break;
					for (int i = 0; i < tones.length(); i++) {
						unsigned char t = (unsigned char)tones[i];
						wBuffer.put(t % 16);
						wBuffer.put(t / 16);
					}
				}
				wBuffer.stop();
			}

			wBuffer.close();
		}
	}*/
	else {
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", paInit.result());
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(paInit.result()));

		system("PAUSE");
	}
	return 0;
}
