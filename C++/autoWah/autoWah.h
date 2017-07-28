#pragma once

#include <cmath>

const double pi = 3.1415926535897932;

enum FilterType {
	Lowpass,
	Bandpass,
	Highpass
};

class autoWah {
public:
	autoWah();
	~autoWah();

	float runEffect(float x);

	void setFilterType(FilterType type);

	void setAttack(float tauA);
	void setRelease(float tauR);
	void setMinMaxFreq(float minFreq, float maxFreq, float fs);
	void setQualityFactor(float Q);

private:
	float levelDetector(float x);
	float stateVariableFilter(float x);
	
	// Level Detector parameters
	float alphaA, alphaR, betaA, betaR;

	// State Variable Filter parameters
	float minFreq, maxFreq, Q, fs;

	// Internal parameters
	float fc, f, q;
	float yHighpass, yBandpass, yLowpass;
	float *yFilter;

};

