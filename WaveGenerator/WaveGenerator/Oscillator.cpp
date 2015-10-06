#include "StdAfx.h"
#include "Oscillator.h"

Oscillator::Oscillator(unsigned long sampleRate)
{
	this->sampleRate = sampleRate;
	this->curFreq = 0.0;
	curPhase = 0.0;
	incr = 0.0;
	twoPI_ovr_sr = TWOPI / (double)sampleRate;
	incr = 0.0;
}

Oscillator::Oscillator(unsigned long sampleRate, double phase)
{
	this->sampleRate = sampleRate;
	this->curFreq = 0.0;
	curPhase = phase;
	incr = 0.0;
	twoPI_ovr_sr = TWOPI / (double)sampleRate;
	incr = 0.0;
}
