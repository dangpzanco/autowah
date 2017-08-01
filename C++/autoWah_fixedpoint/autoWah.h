#pragma once

#include <cmath>
#include <cstdint>
#include "fix32.hpp"

#define FIX32Q 16

const double pi = 3.1415926535897932;
const fp::fix32<FIX32Q> pi_fix32 = fp::fix32<FIX32Q>(pi);

enum FilterType {
	Lowpass,
	Bandpass,
	Highpass
};

class autoWah {
public:
	autoWah();
	~autoWah();

	float runEffect(fp::fix32<FIX32Q> x);

	void setFilterType(FilterType type);

	void setAttack(fp::fix32<FIX32Q> tauA);
	void setRelease(fp::fix32<FIX32Q> tauR);
	//void setAttack(int16_t tauA, int16_t maxVal = 1024);
	//void setRelease(int16_t tauR, int16_t maxVal = 1024);
	void setMinMaxFreq(fp::fix32<FIX32Q> minFreq, fp::fix32<FIX32Q> maxFreq);
	void setSampleRate(float fs);
	void setQualityFactor(fp::fix32<FIX32Q> Q);

private:
	fp::fix32<FIX32Q> levelDetector(fp::fix32<FIX32Q> x);
	fp::fix32<FIX32Q> stateVariableFilter(fp::fix32<FIX32Q> x);

	float sin(float x);
	float precisionSin(float x);
	
	// Level Detector parameters
	fp::fix32<FIX32Q> alphaA, alphaR, betaA, betaR;

	// State Variable Filter parameters
	fp::fix32<FIX32Q> minFreq, freqBandwidth;

	// Internal parameters
	fp::fix32<FIX32Q> f, q;
	float fs, Ts;
	fp::fix32<FIX32Q> yHighpass, yBandpass, yLowpass;
	fp::fix32<FIX32Q> *yFilter;

};

