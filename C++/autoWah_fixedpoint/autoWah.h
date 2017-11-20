#pragma once

#include <math.h>
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
	
	void setMinMaxFreq(fp::fix32<FIX32Q> minFreq, fp::fix32<FIX32Q> maxFreq);
	void setSampleRate(float fs);
	void setQualityFactor(fp::fix32<FIX32Q> Q);
	void setMixing(fp::fix32<FIX32Q> alphaMix);

private:
	fp::fix32<FIX32Q> levelDetector(fp::fix32<FIX32Q> x);
	fp::fix32<FIX32Q> lowPassFilter(fp::fix32<FIX32Q> x);
	fp::fix32<FIX32Q> stateVariableFilter(fp::fix32<FIX32Q> x);
	inline fp::fix32<FIX32Q> mixer(fp::fix32<FIX32Q> x, fp::fix32<FIX32Q> y);
	
	// Level Detector parameters
	fp::fix32<FIX32Q> alphaA, alphaR, betaA, betaR;
	fp::fix32<FIX32Q> bufferL[2];
	float fs, Ts;

	// LowPass Filter parameters
	fp::fix32<FIX32Q> bufferLP, gainLP;

	// State Variable Filter parameters
	fp::fix32<FIX32Q> minFreq, freqBandwidth, centerFreq, q;
	fp::fix32<FIX32Q> yHighpass, yBandpass, yLowpass;
	fp::fix32<FIX32Q> *yFilter;

	// Mixer parameters
	fp::fix32<FIX32Q> alphaMix, betaMix;

};

