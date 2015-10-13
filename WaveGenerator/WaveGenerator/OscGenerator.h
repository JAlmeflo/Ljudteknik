#ifndef __OSCGENERATOR_H__
#define __OSCGENERATOR_H__

#include "StdAfx.h"
#include <vector>
#include "Oscillator.h"
#include "BrkLoader.h"

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

	//Lookup Table
	int lutSize;
	double lutStep;
	double* lookUpTable;

	unsigned long nsamps;
	double tick_sine(Oscillator* osc, double cFreq);
	double tick_LUT(Oscillator* p_osc, double p_cFreq);
	double GetLUTValue(double p_phase);
	double GetEnvelopeValue(std::vector<EnvelopeLine> p_envs, double p_time, bool p_useLinearInterpolate);

public:
	OscGenerator(unsigned long sampleRate, double freq, double duration, double ampfac, int noOscillators, int waveFormType, char* filename);
	~OscGenerator();

	int generateToText();
	int generateToWav(std::string p_ampEnv = "", std::string p_freqEnv = "");
	void InitializeLUT();
	int generateToWavWithLUT();

	void setFilename(char* filename);
};

#endif


	