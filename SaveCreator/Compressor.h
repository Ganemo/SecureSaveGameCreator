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
	static std::string CompressString(std::string input);
	static std::string DecompressString(std::string input);

private:
	static std::string BWT(std::string input, int* index);
	static std::string I_BWT(std::string input, int index);

	static std::string RunLengthEncoding(std::string input);
	static std::string I_RunLengthEncoding(std::string input);
};

