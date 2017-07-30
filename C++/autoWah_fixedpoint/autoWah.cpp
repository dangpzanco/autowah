#include "autoWah.h"



autoWah::autoWah() : 
	yLowpass(0.0f), yBandpass(0.0f), yHighpass(0.0f), 
	yFilter(&yBandpass), q(0.2f), f(0.0f), fs(44.1e3)
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

float autoWah::runEffect(float x)
{
	Fp::Fp32f<FP32Q> xL = x;
	if (xL.rawVal < 0) xL = -xL; // xL = abs(x)

	Fp::Fp32f<FP32Q> yL = levelDetector((float)xL);

	//fc = yL * (maxFreq - minFreq) + minFreq;
	//f = 2 * std::sin(pi*fc / fs);


	f = 2 * Fp::sin(yL * Fp::Fp32f<FP32Q>(freqBandwidth) + Fp::Fp32f<FP32Q>(minFreq));
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

void autoWah::setAttack(float tauA)
{
	autoWah::alphaA = std::exp(-1.0 / (tauA*fs));
	autoWah::betaA = 1.0f - autoWah::alphaA;
}

void autoWah::setRelease(float tauR)
{
	autoWah::alphaR = std::exp(-1.0 / (tauR*fs));
	autoWah::betaR = 1.0f - autoWah::alphaR;
}

void autoWah::setMinMaxFreq(float minFreq, float maxFreq)
{
	autoWah::freqBandwidth = pi*(2.0f*maxFreq - minFreq)/fs;
	autoWah::minFreq = pi*minFreq/fs;
}

void autoWah::setSampleRate(float fs)
{
	autoWah::fs = fs;
}

void autoWah::setQualityFactor(float Q)
{
	autoWah::q = Q;
}

float autoWah::levelDetector(float x)
{
	static float y1 = 0.0f;
	static float y = 0.0f;
	
	float temp = alphaR * y1 + betaR * x;
	if (x > temp) y1 = x; 
	else          y1 = temp;

	y = alphaA * y + betaA * y1;

	return y;
}

float autoWah::stateVariableFilter(float x)
{
	yHighpass  = x - yLowpass - q * yBandpass;
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
