#include <fstream>
#include "autoWah.h"
#include "fix32.hpp"

int main(int argc, char* argv[])
{
	std::ifstream inputFile;
	std::ofstream outputFile;

	inputFile.open("in.pcm", std::ios::in | std::ios::binary);
	outputFile.open("out.pcm", std::ios::out | std::ios::binary);

	int16_t signalIn;
	autoWah myWah;
	const uint8_t q = 16;

	myWah.setMinMaxFreq(20, 5000);

	const float maxInt16 = 32767.0f / 32768.0f;
	const fp::fix32<q> max16(maxInt16);
	const fp::fix32<q> factor(2*pi);

	float ttt = fp::fix32<FIX32Q>(pi_fix32);

	while (inputFile.read((char*)&signalIn, sizeof(signalIn)))
	{
		//float x = (float)signalIn/32768.0f;
		//float y = myWah.runEffect(x);

		// Saturation
		//if (y > maxInt16) y = maxInt16;
		//else if (y < -1.0f) y = -1.0f;

		//signalIn = y * 32768;
		
		fp::fix32<q> x (signalIn);
		fp::fix32<q> y = myWah.runEffect(x);

		// Saturation
		//if (y > max16) y = max16;
		//else if (y < fp::fix32<q>(-1)) y = fp::fix32<q>(-1);

		signalIn = y;

		outputFile.write((char*)&signalIn, sizeof(signalIn));
	}
	inputFile.close();
	outputFile.close();

	return 0;
}






