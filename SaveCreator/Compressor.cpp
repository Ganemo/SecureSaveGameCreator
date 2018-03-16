#include "stdafx.h"
#include "Compressor.h"

#include <algorithm>
#include <vector>
#include <iostream>

Compressor::Compressor()
{
}


Compressor::~Compressor()
{
}

std::string Compressor::CompressString(const std::string input)
{
	int indexoforiginal;
	std::string str = BWT(input + "&", &indexoforiginal);
	str = std::to_string(indexoforiginal) + "|" + str;

	//std::cout << "Input Size After BWT:\t" << str.length() << std::endl;

	//str = RunLengthEncoding(str);

	//std::cout << "Input Size After RLE:\t" << str.length() << std::endl;

	return str;
}
std::string Compressor::DecompressString(std::string input)
{
	int count = 1;
	while (input[count] != '|')
	{
		count++;
	}

	int originalindex = std::stoi(input.substr(0, count));
	std::string str = I_BWT(input.substr(count + 1), originalindex);

	return str.substr(0, str.length()-1);
}

int AscendingSortSuffix(SuffixUnit a, SuffixUnit b)
{
	return a.suffix < b.suffix;
}
std::string Compressor::BWT(const std::string input, int* index)
{
	int inputsize = input.length();
	std::vector<SuffixUnit> suffixes(inputsize);

	//get array of suffixes and their original indexes
	for (int x = 0; x < inputsize; x++)
	{
		suffixes[x].index = x;
		suffixes[x].suffix = input.substr(x);
	}

	//sort suffixes by character
	std::sort(suffixes.begin(), suffixes.end(), AscendingSortSuffix);
	
	std::vector<int> suffixarr(inputsize);
	for (int x = 0; x < inputsize; ++x)
	{
		suffixarr[x] = suffixes[x].index;
	}

	std::string bwts;
	for (int x = 0; x < inputsize; ++x)
	{
		if (suffixarr[x] == 0)
			*index = x;

		int j = suffixarr[x] - 1;
		if (j < 0)
			j = j + inputsize;

		bwts += input[j];
	}
	//add termination character
	//bwts += '\0';

	return bwts;
}


bool AscendingSortChar(const char &a, const char &b)
{
	return a < b;
}
void AddNodeAtEnd(IBWTNode** head, IBWTNode* next)
{
	if (*head == NULL)
		(*head) = next;
	else
	{
		IBWTNode* temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = next;
	}
}
void ComputeLShift(IBWTNode** head, int index, std::vector<int>* lshift)
{
	(*lshift)[index] = (*head)->data;
	(*head) = (*head)->next;
}
std::string Compressor::I_BWT(std::string input, int index)
{
	int size = input.length();
	std::vector<int> leftshift(size);

	std::string sorted = input;
	std::sort(sorted.begin(), sorted.end(), AscendingSortChar);

	std::vector<IBWTNode*> nodes(128);

	for (int x = 0; x < size; ++x)
	{
		AddNodeAtEnd(&nodes[input[x]], new IBWTNode(x));
	}

	for (int x = 0; x < size; ++x)
	{
		ComputeLShift(&nodes[sorted[x]], x, &leftshift);
	}

	std::string returner;

	for (int x = 0; x < size; ++x)
	{
		index = leftshift[index];
		returner += input[index];
	}

	return returner;
}

std::string ReturnRunLengthString(std::string &input, const int offset, int* ending)
{
	int count = 0, size = input.length();

	if (offset >= size)
	{
		*ending = offset;
	}

	char startingchar = input[offset];
	while (offset + count < size && input[offset+count] == startingchar)
	{
		count++;
	}

	*ending = offset + count;


	if (count > 4)
	{
		std::string output;
		output += startingchar;
		output += 1;
		output += std::to_string(count);
		output += 1;

		return output;
	}
	else
		return input.substr(offset, count);
}

std::string Compressor::RunLengthEncoding(std::string input)
{
	int size = input.length();
	std::string returner;

	int offset = 0;
	while (offset < size-1)
	{
		int end = offset;
		returner.append(ReturnRunLengthString(input, offset, &end));
		offset = end;
	}

	return returner;
}
std::string Compressor::I_RunLengthEncoding(std::string input)
{
	return NULL;
}