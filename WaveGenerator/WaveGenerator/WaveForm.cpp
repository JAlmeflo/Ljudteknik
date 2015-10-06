#include "StdAfx.h"
#include "WaveForm.h"
#include "portsf.h"
#include <math.h>

WaveForm::WaveForm(unsigned long sampleRate, double freq, double duration, double ampfac, int waveFormType, char* filename)
{
	this->sampleRate = sampleRate;
	this->freq = freq;
	this->duration = duration;
	this->ampfac = ampfac;
	this->waveFormType = waveFormType;
	this->filename = filename;
	osc = new Oscillator(sampleRate);
	nsamps = (unsigned long)(duration * sampleRate);
}

WaveForm::~WaveForm()
{
	if (osc != NULL)
	{
		delete osc;
	}
}

void WaveForm::setFilename(char* filename)
{
	this->filename = filename;
}

int WaveForm::generateToText()
{
	//Open output file
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) 
	{
		cout << "Unable to open file '" << filename << "'" << endl;
		return 1;
	}

	//Oscillator loop
	for (unsigned long i = 0; i < nsamps; i++)
	{
		double samp = tick();
		fprintf(fp,"%.21f\n",samp);
	}

	//Close file
	fclose(fp);

	return 0;
}

int WaveForm::generateToWav()
{
	PSF_PROPS props;
	props.srate = sampleRate;
	props.chans = 1;
	props.samptype = PSF_SAMP_IEEE_FLOAT;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	portsf psf;
	
	int ofd = psf.psf_sndCreate(filename, &props, 1, 0, PSF_CREATE_RDWR);
	if (ofd < 0)
	{
		cout << "Unable to open file '" << filename << "'" << endl;
		return 1;
	}

	//Oscillator loop
	for (unsigned long i = 0; i < nsamps; i++)
	{
		double samp = tick();
		
		float* frame = (float*) malloc(props.chans * sizeof(float));
		frame[0] = (float)samp;
			
		psf.psf_sndWriteFloatFrames(ofd, frame, 1);
	}

	//Close file
	psf.psf_sndClose(ofd);

	return 0;
}

double WaveForm::tick()
{
	double samp = 0;

	if (waveFormType == SINE) samp = tick_sine();
	if (waveFormType == SQUARE) samp = tick_square();
	if (waveFormType == SAWD) samp = tick_sawd();
	if (waveFormType == SAWU) samp = tick_sawu();
	if (waveFormType == TRI) samp = tick_tri();

	samp = samp * ampfac;

	return samp;
}

double WaveForm::tick_sine()
{
	double val;
	
	//Samp calculation
	val = sin(osc->curPhase);
	//

	if (osc->curFreq != freq)
	{
		osc->curFreq = freq;
		osc->incr = osc->twoPI_ovr_sr * freq;
	}
	osc->curPhase += osc->incr;
	if (osc->curPhase >= TWOPI)
	{
		osc->curPhase -= TWOPI;
	}
	if (osc->curPhase < 0.0)
	{
		osc->curPhase += TWOPI;
	}

	return val;
}

double WaveForm::tick_square()
{
	double val;

	if (osc->curFreq != freq)
	{
		osc->curFreq = freq;
		osc->incr = osc->twoPI_ovr_sr * freq;
	}

	//Samp calculation
	if (osc->curPhase <= M_PI)
	{
		val = 1.0;
	}
	else
	{
		val = -1.0;
	}
	//

	osc->curPhase += osc->incr;
	if (osc->curPhase >= TWOPI)
	{
		osc->curPhase -= TWOPI;
	}
	if (osc->curPhase < 0.0)
	{
		osc->curPhase += TWOPI;
	}

	return val;
}

double WaveForm::tick_sawd()
{
	double val;

	if (osc->curFreq != freq)
	{
		osc->curFreq = freq;
		osc->incr = osc->twoPI_ovr_sr * freq;
	}

	//Samp calculation
	val = 1.0 - 2.0 * (osc->curPhase * (1.0 / TWOPI));
	//

	osc->curPhase += osc->incr;
	if (osc->curPhase >= TWOPI)
	{
		osc->curPhase -= TWOPI;
	}
	if (osc->curPhase < 0.0)
	{
		osc->curPhase += TWOPI;
	}

	return val;
}

double WaveForm::tick_sawu()
{
	double val;

	if (osc->curFreq != freq)
	{
		osc->curFreq = freq;
		osc->incr = osc->twoPI_ovr_sr * freq;
	}

	//Samp calculation
	val = (2.0 * (osc->curPhase * (1.0 / TWOPI))) - 1.0;
	//

	osc->curPhase += osc->incr;
	if (osc->curPhase >= TWOPI)
	{
		osc->curPhase -= TWOPI;
	}
	if (osc->curPhase < 0.0)
	{
		osc->curPhase += TWOPI;
	}

	return val;
}

double WaveForm::tick_tri()
{
	double val;

	if (osc->curFreq != freq)
	{
		osc->curFreq = freq;
		osc->incr = osc->twoPI_ovr_sr * freq;
	}

	//Samp calculation
	val = (2.0 * (osc->curPhase * (1.0 / TWOPI))) - 1.0;
	if (val < 0.0)
	{
		val = -val;
	}
	val = 2.0 * (val - 0.5);
	//

	osc->curPhase += osc->incr;
	if (osc->curPhase >= TWOPI)
	{
		osc->curPhase -= TWOPI;
	}
	if (osc->curPhase < 0.0)
	{
		osc->curPhase += TWOPI;
	}

	return val;
}
