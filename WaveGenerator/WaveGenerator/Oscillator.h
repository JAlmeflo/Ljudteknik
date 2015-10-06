#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

#include "StdAfx.h"

/*
	Hanterar en oscillator som används av OscGenerator/WaveForm för att
	generera en ljudvåg.

	author: Johan Hagelbäck (jhg@bth.se)
*/
class Oscillator
{
private:
	
public:
	Oscillator(unsigned long sampleRate);
	Oscillator(unsigned long sampleRate, double phase);

	double twoPI_ovr_sr;

	unsigned long sampleRate;
	double curFreq;
	double curPhase;
	double incr;
};

#endif