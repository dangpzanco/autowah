#include <fstream>
#include "autoWah.h"

int main(int argc, char* argv[])
{
	std::ifstream inputFile;
	std::ofstream outputFile;

	inputFile.open("in.pcm", std::ios::in | std::ios::binary);
	outputFile.open("out.pcm", std::ios::out | std::ios::binary);

	int16_t signalIn;
	autoWah myWah;

	const float maxInt16 = 32767.0f / 32768.0f;

	while (inputFile.read((char*)&signalIn, sizeof(signalIn)))
	{
		float x = (float)signalIn/32768.0f;

		float y = myWah.runEffect(x);

		// Saturation
		if (y > maxInt16) y = maxInt16;
		else if (y < -1.0f) y = -1.0f;

		signalIn = y * 32768;
		outputFile.write((char*)&signalIn, sizeof(signalIn));
	}
	inputFile.close();
	outputFile.close();

	return 0;
}






