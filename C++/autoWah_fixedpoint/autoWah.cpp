#include "autoWah.h"



autoWah::autoWah() : 
	yLowpass(0.0f), yBandpass(0.0f), yHighpass(0.0f), 
	yFilter(&yBandpass), q(0.2f), f(0.0f), fs(44.1), Ts(1.0/44.1)
{
	autoWah::setAttack(40e-3);
	autoWah::setRelease(2e-3);
	autoWah::setMinMaxFreq(20, 2500);
}


autoWah::~autoWah()
{
}

//float autoWah::runEffect(float x)
//{
//	float xL = x;
//	if (xL < 0.0f) xL = -xL; // xL = abs(x)
//
//	float yL = levelDetector(xL);
//
//	//fc = yL * (maxFreq - minFreq) + minFreq;
//	//f = 2 * std::sin(pi*fc / fs);
//	
//
//	f = 2.0f * autoWah::sin(yL * freqBandwidth + minFreq);
//	//f = 2.0f * std::sin(yL * freqBandwidth + minFreq);
//
//	return stateVariableFilter(x);
//	//return f;
//}

float autoWah::runEffect(fp::fix32<FIX32Q> x)
{
	fp::fix32<FIX32Q> xL = x;
	if (xL.rawVal < 0) xL = -xL; // xL = abs(x)

	fp::fix32<FIX32Q> yL = levelDetector(xL);

	//fc = yL * (maxFreq - minFreq) + minFreq;
	//f = 2 * std::sin(pi*fc / fs);


	f = 2 * fp::sin(yL * fp::fix32<FIX32Q>(freqBandwidth) + fp::fix32<FIX32Q>(minFreq));
	//f = 2.0f * std::sin(yL * freqBandwidth + minFreq);

	return stateVariableFilter(x);
	//return f;
}

void autoWah::setFilterType(FilterType type)
{
	switch(type) {
	case FilterType::Lowpass:
		yFilter = &yLowpass;
		break;
	case FilterType::Bandpass:
		yFilter = &yBandpass;
		break;
	case FilterType::Highpass:
		yFilter = &yHighpass;
		break;
	}
}

void autoWah::setAttack(fp::fix32<FIX32Q> tauA)
{
	//autoWah::alphaA = fp::exp<FIX32Q>(-fp::fixinv<FIX32Q>((tauA*fp::fix32<FIX32Q>(fs)).rawVal));
	//autoWah::alphaA = exp(-1.0f/((float)tauA*fs));
	autoWah::alphaA = exp(-1e-3f * Ts / (float)tauA);
	autoWah::betaA = 1 - autoWah::alphaA;
}

void autoWah::setRelease(fp::fix32<FIX32Q> tauR)
{
	//autoWah::alphaR = fp::exp<FIX32Q>(-fp::fixinv<FIX32Q>((tauR*fp::fix32<FIX32Q>(fs)).rawVal));
	autoWah::alphaR = exp(-1e-3f * Ts / (float)tauR);
	autoWah::betaR = 1 - autoWah::alphaR;
}

void autoWah::setMinMaxFreq(fp::fix32<FIX32Q> minFreq, fp::fix32<FIX32Q> maxFreq)
{
	autoWah::freqBandwidth = pi_fix32 * fp::fix32<FIX32Q>(2*maxFreq - minFreq) / 1000 * fp::fix32<FIX32Q>(Ts);
	autoWah::minFreq = pi_fix32 * fp::fix32<FIX32Q>(minFreq) / 1000 * fp::fix32<FIX32Q>(Ts);
	//autoWah::freqBandwidth = pi * (2 * maxFreq - minFreq) / fs;
	//autoWah::minFreq = pi * minFreq / fs;
	//autoWah::freqBandwidth = pi * (2 * maxFreq - minFreq) / (1000 * fs);
	//autoWah::minFreq = pi * minFreq / (1000 * fs);
}

void autoWah::setSampleRate(float fs)
{
	autoWah::fs = fs;
	autoWah::Ts = 1.0f/fs;
}

void autoWah::setQualityFactor(fp::fix32<FIX32Q> Q)
{
	autoWah::q = Q;
}

fp::fix32<FIX32Q> autoWah::levelDetector(fp::fix32<FIX32Q> x)
{
	static fp::fix32<FIX32Q> y1 = 0.0f;
	static fp::fix32<FIX32Q> y = 0.0f;
	
	fp::fix32<FIX32Q> temp = fp::fix32<FIX32Q>(alphaR) * y1 + fp::fix32<FIX32Q>(betaR) * fp::fix32<FIX32Q>(x);
	if (fp::fix32<FIX32Q>(x) > temp) y1 = x;
	else          y1 = temp;

	y = fp::fix32<FIX32Q>(alphaA) * y + fp::fix32<FIX32Q>(betaA) * y1;

	return y;
}

fp::fix32<FIX32Q> autoWah::stateVariableFilter(fp::fix32<FIX32Q> x)
{
	yHighpass  = x - yLowpass - (q * yBandpass);
	yBandpass += f * yHighpass;
	yLowpass  += f * yBandpass;

	return *yFilter;
}

float autoWah::sin(float x)
{
	static float factor3 = -1.0f / 6.0f;
	return x * (1.0f + factor3*x*x);
}

float autoWah::precisionSin(float x)
{
	static float factor3 = -1.0f / 6.0f;
	static float factor5 = 1.0f / 120.0f;
	float x2 = x*x;
	float x4 = x2*x2;
	return x*(1.0f + factor3*x2 + factor5*x4);
}
