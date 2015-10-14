#include "StdAfx.h"
#include "OscGenerator.h"
#include "portsf.h"
#include <math.h>

OscGenerator::OscGenerator(unsigned long sampleRate, double freq, double duration, double ampfac, int noOscillators, int waveFormType, char* filename)
{
	this->sampleRate = sampleRate;
	this->freq = freq;
	this->duration = duration;
	this->ampfac = ampfac;
	this->waveFormType = waveFormType;
	this->filename = filename;

	for (int i = 0; i < noOscillators; i++)
	{
		oscs.push_back(new Oscillator(sampleRate));
	}
	oscamps = (double*) malloc(noOscillators * sizeof(double));
	oscfreqs = (double*) malloc(noOscillators * sizeof(double));

	nsamps = (unsigned long)(duration * sampleRate);

	init();
}

void OscGenerator::init()
{
	double ampfac = 1.0;
	double freqfac = 1.0;
	double ampadjust = 0.0;
	double phase = 0.0;

	if (waveFormType == TRI)
	{
		for (int i = 0; i < (int)oscs.size(); i++)
		{
			ampfac = 1.0 / (freqfac*freqfac);
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 2.0; /* odd harmonics only */
			ampadjust += ampfac;
		}
		phase = 0.25;
	}
	if (waveFormType == SQUARE)
	{
		for (int i = 0; i < (int)oscs.size(); i++)
		{
			ampfac = 1.0 / freqfac;
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 2.0;
			ampadjust += ampfac;
		}
	}
	if (waveFormType == SAWU || waveFormType == SAWD)
	{
		for (int i = 0; i < (int)oscs.size(); i++)
		{
			ampfac = 1.0 / freqfac;
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 1.0;
			ampadjust += ampfac;
		}
		if (waveFormType == SAWU)
		{
			ampadjust = -ampadjust; /* inverts the waveform */
		}
	}

	/* Rescale amplitudes so they add to 1.0 */
	/* Set correct phase (sine/cos) for different wavetypes */
	for (int i = 0; i < (int)oscs.size(); i++)
	{
		oscamps[i] /= ampadjust;
		oscs.at(i)->curPhase = phase;
	}
}

OscGenerator::~OscGenerator()
{
	for (int i = 0; i < (int)oscs.size(); i++)
	{
		delete oscs.at(i);
	}
}

void OscGenerator::setFilename(char* filename)
{
	this->filename = filename;
}

int OscGenerator::generateToText()
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
		double val = 0.0;
		for (int k = 0; k < (int)oscs.size(); k++)
		{
			val += oscamps[k] * tick_sine(oscs[k], freq*oscfreqs[k]);
		}
		float samp = (float)(val * ampfac);

		fprintf(fp,"%.21f\n",samp);
	}

	//Close file
	fclose(fp);

	return 0;
}

int OscGenerator::generateToWav(std::string p_ampEnv, std::string p_freqEnv)
{
	bool useAmpEnv = p_ampEnv != "";
	bool useFreqEnv = p_freqEnv != "";
	BrkLoader loader = BrkLoader();
	std::vector<EnvelopeLine> ampEnvs;
	std::vector<EnvelopeLine> freqEnvs;
	if (useAmpEnv) ampEnvs = loader.LoadFile(p_ampEnv, true);
	if (useFreqEnv) freqEnvs = loader.LoadFile(p_freqEnv, false);


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

	for (unsigned long i = 0; i < nsamps; i++)
	{
		double val = 0.0;
		double time = (double)i / (double)sampleRate;
		for (int k = 0; k < (int)oscs.size(); k++)
		{
			freq = useFreqEnv ? GetEnvelopeValue(freqEnvs, time, false) : freq;
			val += oscamps[k] * tick_sine(oscs[k], freq*oscfreqs[k]);
		}
		
		double ampValue = useAmpEnv ? GetEnvelopeValue(ampEnvs, time, true) : 1.0;
		float samp = (float)(val * ampfac * ampValue);

		float* frame = (float*) malloc(props.chans * sizeof(float));
		frame[0] = samp;
		psf.psf_sndWriteFloatFrames(ofd, frame, 1);
	}

	//Close file
	psf.psf_sndClose(ofd);

	return 0;
}

double OscGenerator::tick_sine(Oscillator* osc, double cFreq)
{
	double val;

	//Samp calculation
	val = sin(osc->curPhase);
	//

	if (osc->curFreq != cFreq)
	{
		osc->curFreq = cFreq;
		osc->incr = osc->twoPI_ovr_sr * cFreq;
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

double OscGenerator::GetEnvelopeValue(std::vector<EnvelopeLine> p_envs, double p_time, bool p_useLinearInterpolate)
{
	for (unsigned int i = 0; i < p_envs.size() - 1; i++)
	{
		if (p_time >= p_envs[i].time && p_time <= p_envs[i + 1].time)
		{
			if (p_time >= 0.9)
			{
				int a = 0;
			}
			if (!p_useLinearInterpolate)
			{
				return p_envs[i].value;
			}
			double time1 = p_envs[i].time;
			double time2 = p_envs[i + 1].time;
			double timeRatio = p_time != 0 ? (p_time - time1) / (time2 - time1) : 0.0;

			double val1 = p_envs[i].value;
			double val2 = p_envs[i + 1].value;
			double valDiff = val2 - val1;

			return val1 + valDiff * timeRatio;
		}
	}

	return p_envs[p_envs.size() - 1].value;
}

void OscGenerator::InitializeLUT()
{
	lutSize = 64;
	lutStep = TWOPI / lutSize;
	lookUpTable = (double*)malloc((lutSize + 1) * sizeof(double));

	for (int i = 0; i < lutSize; i++)
	{
		double val = sin(lutStep * i);
		// Square
		//if (val < 0.0)
		//{
			//val = -1.0;
		//}
		//else
		//{
			//val = 1.0;
		//}
		lookUpTable[i] = val;
	}

	lookUpTable[lutSize] = lookUpTable[0];
}

int OscGenerator::generateToWavWithLUT()
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

	for (unsigned long i = 0; i < nsamps; i++)
	{
		double val = 0.0;
		for (int k = 0; k < (int)oscs.size(); k++)
		{
			val += oscamps[k] * tick_LUT(oscs[k], freq*oscfreqs[k]);
		}
		float samp = (float)(val * ampfac);

		float* frame = (float*)malloc(props.chans * sizeof(float));
		frame[0] = samp;
		psf.psf_sndWriteFloatFrames(ofd, frame, 1);

	}
	//Close file
	psf.psf_sndClose(ofd);

	return 0;
}

double OscGenerator::tick_LUT(Oscillator* p_osc, double p_cFreq)
{
	double val = 0.0;

	//Samp calculation
	val = GetLUTValue(p_osc->curPhase);

	//cout << val << endl;
	if (p_osc->curFreq != p_cFreq)
	{
		p_osc->curFreq = p_cFreq;
		p_osc->incr = p_osc->twoPI_ovr_sr * p_cFreq;
	}
	p_osc->curPhase += p_osc->incr;
	if (p_osc->curPhase >= TWOPI)
	{
		p_osc->curPhase -= TWOPI;
	}
	if (p_osc->curPhase < 0.0)
	{
		p_osc->curPhase += TWOPI;
	}

	return val;
}

double OscGenerator::GetLUTValue(double p_phase)
{
	for (int i = 1; i < lutSize; i++)
	{
		double highValue = i * lutStep;
		double lowValue = (i - 1) * lutStep;

		if (p_phase == lowValue)
		{
			return lookUpTable[i - 1];
		}
		else if (p_phase == highValue)
		{
			return lookUpTable[i];
		}
		else if (p_phase > lowValue && p_phase < highValue)
		{
			return lookUpTable[i - 1] + (lookUpTable[i] - lookUpTable[i - 1]) * (p_phase - lowValue) / (highValue - lowValue);
		}
	}

	return 0.0;
}