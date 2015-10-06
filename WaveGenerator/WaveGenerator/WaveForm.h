#ifndef __WAVEFORM_H__
#define __WAVEFORM_H__

#include "StdAfx.h"
#include "Oscillator.h"

/*
	Skapar en ljudvåg av valfri typ och en oscillator (ingen syntes).

	author: Johan Hagelbäck (jhg@bth.se)
*/
class WaveForm
{
private:
	//Parameters
	unsigned long sampleRate;
	double freq;
	double duration;
	double ampfac;
	int waveFormType;
	char* filename;
	Oscillator* osc;

	//Additional variables and methods
	unsigned long nsamps;
	double tick();
	double tick_sine();
	double tick_square();
	double tick_sawd();
	double tick_sawu();
	double tick_tri();

public:
	WaveForm(unsigned long sampleRate, double freq, double duration, double ampfac, int waveFormType, char* filename);
	~WaveForm();

	int generateToText();
	int generateToWav();

	void setFilename(char* filename);
};

#endif


	