#pragma once

#include <cmath>
#include <cstdint>

const double pi = 3.1415926535897932;
const float piFloat = 3.1415926535897932f;

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
	void setMixing(float alphaMix);

private:
	float levelDetector(float x);
	float lowPassFilter(float x);
	float stateVariableFilter(float x);
	inline float mixer(float x, float y);

	float sin(float x);
	float precisionSin(float x);
	float tan(float x);
	float precisionTan(float x);

	// Sin and Tan Constants
	const float sinConst3, sinConst5;
	const float tanConst3, tanConst5;

	// Level Detector parameters
	float alphaA, alphaR, betaA, betaR;
	float bufferL[2];

	// Lowpass filter parameters
	float bufferLP, gainLP;

	// State Variable Filter parameters
	float minFreq, freqBandwidth;
	float q, fs, centerFreq;
	float yHighpass, yBandpass, yLowpass;
	float *yFilter;

	// Mixer parameters
	float alphaMix, betaMix;

};

