#include "autoWah.h"


autoWah::autoWah() : 
	yLowpass(0.0f), yBandpass(0.0f), yHighpass(0.0f), 
	yFilter(&yBandpass), fs(44.1), Ts(1.0/44.1),
	bufferL{(int32_t)0, (int32_t)0}, bufferLP((int32_t)0)
{
	// Floating Point implementation
	autoWah::setAttack(40e-3f);
	autoWah::setRelease(2e-3f);
	autoWah::setMinMaxFreq((int32_t)20, (int32_t)3000);
	autoWah::setQualityFactor(1.0f / 5.0f);
	autoWah::setMixing(1.0f);

	// Modifications
	autoWah::setSampleRate(44.1);
	autoWah::setAttack(40e-3f);
	autoWah::setRelease(5e-3f);
	autoWah::setMinMaxFreq((int32_t)20, (int32_t)3000);
	autoWah::setQualityFactor(1.0f / 5.0f);
	autoWah::setMixing(1.0f);
}

autoWah::~autoWah()
{
}

float autoWah::runEffect(fp::fix32<FIX32Q> x)
{
	fp::fix32<FIX32Q> xL = x;
	if (xL.rawVal < 0) xL = -xL; // xL = abs(x)

	fp::fix32<FIX32Q> yL = levelDetector(xL);
	// fp::fix32<FIX32Q> yL = 0.5;

	//fc = yL * (maxFreq - minFreq) + minFreq;
	//f = 2 * std::sin(pi * fc / fs);

	centerFreq = yL * freqBandwidth + minFreq;

	fp::fix32<FIX32Q> xF = lowPassFilter(x);
	// fp::fix32<FIX32Q> xF = x;
	fp::fix32<FIX32Q> yF = stateVariableFilter(xF);

	fp::fix32<FIX32Q> y = mixer(x, yF);
	return y;
	// return yL;
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
	autoWah::betaA = (int32_t)1 - autoWah::alphaA;
}

void autoWah::setRelease(fp::fix32<FIX32Q> tauR)
{
	//autoWah::alphaR = fp::exp<FIX32Q>(-fp::fixinv<FIX32Q>((tauR*fp::fix32<FIX32Q>(fs)).rawVal));
	autoWah::alphaR = exp(-1e-3f * Ts / (float)tauR);
	autoWah::betaR = (int32_t)1 - autoWah::alphaR;
}

void autoWah::setAlphaA(fp::fix32<FIX32Q> alphaA)
{
	autoWah::alphaA = alphaA;
	autoWah::betaA = (int32_t)1 - autoWah::alphaA;
}

void autoWah::setAlphaR(fp::fix32<FIX32Q> alphaR)
{
	autoWah::alphaR = alphaR;
	autoWah::betaR = (int32_t)1 - autoWah::alphaR;
}

void autoWah::setBandwidth(fp::fix32<FIX32Q> bw)
{
	autoWah::freqBandwidth = bw;
}

void autoWah::setMinMaxFreq(fp::fix32<FIX32Q> minFreq, fp::fix32<FIX32Q> maxFreq)
{
	autoWah::freqBandwidth = fp::fix32<FIX32Q>((int32_t)2*maxFreq - minFreq) / (int32_t)1000 * fp::fix32<FIX32Q>(Ts);
	autoWah::minFreq = fp::fix32<FIX32Q>(minFreq) / (int32_t)1000 * fp::fix32<FIX32Q>(Ts);
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
	//autoWah::gainLP = std::sqrt((float)(q >> 2));
	autoWah::gainLP = fp::sqrt(autoWah::q >> (int32_t)2);
}

void autoWah::setMixing(fp::fix32<FIX32Q> alphaMix)
{
	autoWah::alphaMix = alphaMix;
	autoWah::betaMix = fp::fix32<FIX32Q>((int32_t)1) - alphaMix;
}

fp::fix32<FIX32Q> autoWah::levelDetector(fp::fix32<FIX32Q> x)
{
	fp::fix32<FIX32Q> y1 = alphaR * bufferL[1] + betaR * x;
	if (x > y1) bufferL[1] = x;
	else          bufferL[1] = y1;

	bufferL[0] = alphaA * bufferL[0] + betaA * bufferL[1];

	return bufferL[0];
}

fp::fix32<FIX32Q> autoWah::lowPassFilter(fp::fix32<FIX32Q> x)
{
	//float K = std::tan(pi * fc / fs);
	// b1 = b0 = K / (K + 1);
	// a1 = (K - 1) / (K + 1);
	fp::fix32<FIX32Q> K = fp::tan(pi_fix32 * centerFreq);
	fp::fix32<FIX32Q> b0 = K / (K + (int32_t)1);
	//fp::fix32<FIX32Q> a1 = 2 * (b0 - fp::fix32<FIX32Q>(0.5f));
	fp::fix32<FIX32Q> a1 = (b0 - fp::fix32<FIX32Q>(0.5f)) << (int32_t)1;

	fp::fix32<FIX32Q> xh = x - a1 * bufferLP;
	fp::fix32<FIX32Q> y = b0 * (xh + bufferLP);
	bufferLP = xh;

	return gainLP * y;
}

fp::fix32<FIX32Q> autoWah::stateVariableFilter(fp::fix32<FIX32Q> x)
{
	fp::fix32<FIX32Q> f = fp::sin(pi_fix32 * centerFreq) << (int32_t)1;
	yHighpass  = x - yLowpass - (q * yBandpass);
	yBandpass += f * yHighpass;
	yLowpass  += f * yBandpass;

	return *yFilter;
}

inline fp::fix32<FIX32Q> autoWah::mixer(fp::fix32<FIX32Q> x, fp::fix32<FIX32Q> y)
{
	return alphaMix * y + betaMix * x;
}

