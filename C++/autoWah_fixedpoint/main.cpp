#include <fstream>
#include "autoWah.h"
#include "Fp32f.hpp"

int main(int argc, char* argv[])
{
	std::ifstream inputFile;
	std::ofstream outputFile;

	inputFile.open("in.pcm", std::ios::in | std::ios::binary);
	outputFile.open("out.pcm", std::ios::out | std::ios::binary);

	int16_t signalIn;
	autoWah myWah;
	const uint8_t q = 16;

	const float maxInt16 = 32767.0f / 32768.0f;
	const Fp::Fp32f<q> max16(maxInt16);

	while (inputFile.read((char*)&signalIn, sizeof(signalIn)))
	{
		//float x = (float)signalIn/32768.0f;
		//float y = myWah.runEffect(x);

		// Saturation
		//if (y > maxInt16) y = maxInt16;
		//else if (y < -1.0f) y = -1.0f;

		//signalIn = y * 32768;

		
		Fp::Fp32f<q> x (signalIn);

		x *= 0.5;

		// Saturation
		if (x > max16) x = max16;
		else if (x < Fp::Fp32f<q>(-1)) x = Fp::Fp32f<q>(-1);

		float y = x;

		signalIn = x;

		outputFile.write((char*)&signalIn, sizeof(signalIn));
	}
	inputFile.close();
	outputFile.close();

	return 0;
}






