#include "autoWah.h"



autoWah::autoWah() : 
	yLowpass(0.0f), yBandpass(0.0f), yHighpass(0.0f), yFilter(&yBandpass),
	minFreq(0), maxFreq(0), fc(0)
{
}


autoWah::~autoWah()
{
}

float autoWah::runEffect(float x)
{
	float xL = x;
	if (xL < 0.0f) xL = -xL; // xL = abs(x)

	float yL = levelDetector(xL);

	fc = yL * (maxFreq - minFreq) + minFreq;
	f = 2 * std::sin(pi*fc / fs);

	float xF = fc;
	float yF = stateVariableFilter(xF);


	return yF;
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
