#include <fstream>
#include "autoWah.h"

int main(int argc, char* argv[])
{
	std::ifstream inputFile;
	std::ofstream outputFile;

	inputFile.open("in.pcm", std::ios::in | std::ios::binary);
	outputFile.open("out.pcm", std::ios::out | std::ios::binary);

}






