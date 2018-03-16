#pragma once

#include <string>

struct SuffixUnit
{
	int index;
	std::string suffix;
};
struct IBWTNode
{
	int data;
	IBWTNode* next = NULL;

	IBWTNode()
	{
		data = 0;
	}
	IBWTNode(int i)
	{
		data = i;
	}
};

class Compressor
{
public:
	Compressor();
	~Compressor();

	std::string CompressString(std::string input);
	std::string DecompressString(std::string input);

private:
	std::string BWT(std::string input, int* index);
	std::string I_BWT(std::string input, int index);

	std::string RunLengthEncoding(std::string input);
	std::string I_RunLengthEncoding(std::string input);


};

