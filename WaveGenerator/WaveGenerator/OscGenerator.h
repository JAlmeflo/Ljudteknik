#ifndef __OSCGENERATOR_H__
#define __OSCGENERATOR_H__

#include "StdAfx.h"
#include <vector>
#include "Oscillator.h"
#include "BrkLoader.h"

/*
	Skapar en syntetiserad ljudv�g av valfri typ och valfritt antal
	oscillatorer (harmonier).

	author: Johan Hagelb�ck (jhg@bth.se)
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
	double GetEnvelopeValue(std::vector<EnvelopeLine> p_envs, double p_time, bool p_useLinearInterpolate);

public:
	OscGenerator(unsigned long sampleRate, double freq, double duration, double ampfac, int noOscillators, int waveFormType, char* filename);
	~OscGenerator();

	int generateToText();
	int generateToWav(std::string p_ampEnv = "", std::string p_freqEnv = "");

	void setFilename(char* filename);
};

#endif


	