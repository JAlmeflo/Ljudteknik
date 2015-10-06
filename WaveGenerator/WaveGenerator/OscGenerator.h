#ifndef __OSCGENERATOR_H__
#define __OSCGENERATOR_H__

#include "StdAfx.h"
#include <vector>
#include "Oscillator.h"

/*
	Skapar en syntetiserad ljudvåg av valfri typ och valfritt antal
	oscillatorer (harmonier).

	author: Johan Hagelbäck (jhg@bth.se)
*/
class OscGenerator
{
private:
	//Parameters
	unsigned long sampleRate;
	double freq;
	double duration;
	double ampfac;
	char* filename;

	//Additional variables and methods
	vector<Oscillator*> oscs;
	double* oscamps;
	double* oscfreqs;
	int waveFormType;
	void init();

	unsigned long nsamps;
	double tick_sine(Oscillator* osc, double cFreq);
	

public:
	OscGenerator(unsigned long sampleRate, double freq, double duration, double ampfac, int noOscillators, int waveFormType, char* filename);
	~OscGenerator();

	int generateToText();
	int generateToWav();

	void setFilename(char* filename);
};

#endif


	