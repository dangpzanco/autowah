#pragma once

#include <cmath>
#include <cstdint>

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
	//void setAttack(int16_t tauA, int16_t maxVal = 1024);
	//void setRelease(int16_t tauR, int16_t maxVal = 1024);
	void setMinMaxFreq(float minFreq, float maxFreq);
	void setSampleRate(float fs);
	void setQualityFactor(float Q);

private:
	float levelDetector(float x);
	float stateVariableFilter(float x);

	float sin(float x);
	float precisionSin(float x);
	
	// Level Detector parameters
	float alphaA, alphaR, betaA, betaR;

	// State Variable Filter parameters
	float minFreq, freqBandwidth;

	// Internal parameters
	float f, q, fs;
	float yHighpass, yBandpass, yLowpass;
	float *yFilter;

};

