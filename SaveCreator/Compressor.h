#pragma once

#include <string>

/// <summary>
/// unit used for suffix indexing for BWT
/// </summary>
struct SuffixUnit
{
	int index;
	std::string suffix;
};
/// <summary>
/// node used in Inverse BWT for index sorting
/// </summary>
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

/// <summary>
/// Compression functions used to compress save files.
/// Implements Burrows-Wheeler Transform (BWT) and Run Length Encoding
/// </summary>
class Compressor
{
//public static functions
public:
	static std::string CompressString(std::string input);
	static std::string DecompressString(std::string input);

//private static functions
private:
	static std::string BWT(std::string input, int* index);
	static std::string I_BWT(std::string input, int index);

	static std::string RunLengthEncoding(std::string input);
	static std::string I_RunLengthEncoding(std::string input);
};

