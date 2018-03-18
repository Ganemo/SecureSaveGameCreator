#include "stdafx.h"
#include "Compressor.h"

#include <algorithm>
#include <vector>
#include <iostream>

/// <summary>
/// Compresses input string using BWT and Run Length Encoding
/// </summary>
/// <param name="input">The string to be compressed</param>
/// <returns>The compressed string</returns>
std::string Compressor::CompressString(const std::string input)
{
	//the index of the original string after sorting in BWT
	int indexoforiginal;

	//performing the BWT, adding '&' at the end as is required for BWT
	std::string str = BWT(input + "&", &indexoforiginal);

	//performing run length encoding
	str = RunLengthEncoding(str);

	//adding the original index for use in Inverse BWT
	str = std::to_string(indexoforiginal) + "|" + str;

	return str;
}

/// <summary>
/// Decompresses input string using Reverse Run Length Encoding and Inverse BWT 
/// </summary>
/// <param name="input">The string to be decompressed</param>
/// <returns>The decompressed string</returns>
std::string Compressor::DecompressString(std::string input)
{
	//retrieve the index of the original string to use in Inverse BWT
	int count = 1;
	while (input[count] != '|')
	{
		count++;
	}
	int originalindex = std::stoi(input.substr(0, count));

	//reverse Run Length Encoding and perform Inverse BWT
	std::string str = I_BWT(I_RunLengthEncoding(input.substr(count + 1)), originalindex);

	//return the decompressed string with the trailing '&' removed
	return str.substr(0, str.length()-1);
}

/// <summary>
/// Comparative function used to sort suffixes in BWT in ascending order
/// </summary>
/// <param name="a">A suffix</param>
/// <param name="b">A suffix</param>
/// <returns>A suffix is less than B suffix</returns>
bool AscendingSortSuffix(SuffixUnit a, SuffixUnit b)
{
	return a.suffix < b.suffix;
}
/// <summary>
/// Burrows-Wheeler Transform, shifting similar characters together
/// </summary>
/// <param name="input">The string to be transformed</param>
/// <param name="index">Pointer to an int which will contain the index of the original string after sorting</param>
/// <returns>The transformed string</returns>
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

	//sort suffixes lexicographically
	std::sort(suffixes.begin(), suffixes.end(), AscendingSortSuffix);

	//stores the shifted characters into a string
	std::string bwts;
	for (int x = 0; x < inputsize; ++x)
	{
		//finds the original indexed string
		if (suffixes[x].index == 0)
			*index = x;

		int j = suffixes[x].index - 1;
		if (j < 0)
			j = j + inputsize;

		bwts += input[j];
	}

	return bwts;
}

/// <summary>
/// Comparative function used for Invers BWT that compares two characters in ascending order 
/// </summary>
/// <param name="a">A char</param>
/// <param name="b">A char</param>
/// <returns>If A is less than B</returns>
bool AscendingSortChar(const char &a, const char &b)
{
	return a < b;
}
/// <summary>
/// Add a node to the end of an IBWT linked list
/// </summary>
/// <param name="head">Pointer to the head of an IBWT linked list</param>
/// <param name="next">The Node to be added</param>
void AddNodeAtEnd(IBWTNode** head, IBWTNode* next)
{
	//set the new node as the head if there is no head
	//else, add the node to the end of the linked list
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
/// <summary>
/// Computes the left shift needed to Invert the shifts from BWT
/// Stores the indexes after IBWT reverse sorting
/// Removes head of linked list and sets next node as new head
/// </summary>
/// <param name="head">The head</param>
/// <param name="index">The index of the left shifted string</param>
/// <param name="lshift">The storage container for the correct ordering of indexes</param>
void ComputeLShift(IBWTNode** head, int index, std::vector<int>* lshift)
{
	//Sets the index of left shifts to the index stored in the head of the linked list
	(*lshift)[index] = (*head)->data;
	//pops head and makes next node the new head
	(*head) = (*head)->next;
}
/// <summary>
/// Inverse Burrows-Wheeler Transform, putting characters back in their original positions
/// </summary>
/// <param name="input">The string which will be inversed</param>
/// <param name="index">The index of the original string in the BWT</param>
/// <returns>The string after Inverse Burrows-Wheeler transform</returns>
std::string Compressor::I_BWT(std::string input, int index)
{
	int size = input.length();
	std::vector<int> leftshift(size);

	//sort string's characters into ascending order
	std::string sorted = input;
	std::sort(sorted.begin(), sorted.end(), AscendingSortChar);

	//instantiate the IBWT nodes, making an array of 128 characters for sizeof(char)
	std::vector<IBWTNode*> nodes(128);

	//insert nodes into array
	for (int x = 0; x < size; ++x)
	{
		AddNodeAtEnd(&nodes[input[x]], new IBWTNode(x));
	}

	//find the left shift of nodes
	for (int x = 0; x < size; ++x)
	{
		ComputeLShift(&nodes[sorted[x]], x, &leftshift);
	}

	std::string returner;
	//add characters in the order before performing BWT
	for (int x = 0; x < size; ++x)
	{
		index = leftshift[index];
		returner += input[index];
	}

	return returner;
}

/// <summary>
/// Compressing repeated characters from starting offset in input string
/// </summary>
/// <param name="input">The input strung</param>
/// <param name="offset">The starting offset</param>
/// <param name="ending">A pointer to the ending offset</param>
/// <returns>Return the string representing number of repeating characters in a row</returns>
std::string ReturnRunLengthString(std::string &input, const int offset, int* ending)
{
	int count = 0, size = input.length();

	//error handling for invalid offset
	if (offset >= size)
	{
		*ending = offset;
	}

	//set the character focused on
	char startingchar = input[offset];
	//find number of repeating characters in a row
	while (offset + count < size && input[offset+count] == startingchar)
	{
		count++;
	}

	//set the ending index of search
	*ending = offset + count;

	//only return character count if there are more than 4 repeating characters
	//if there are 4 or less, then compressing them will make the output greater
	if (count > 4)
	{
		std::string output;
		output += startingchar;
		//delimiter character
		output += 1;
		output += std::to_string(count);
		//delimiter character
		output += 1;

		return output;
	}
	else
		return input.substr(offset, count);
}
/// <summary>
/// Compressing input string by representing repeated characters by a number
/// </summary>
/// <param name="input">The string to be compressed</param>
/// <returns>The compressed string</returns>
std::string Compressor::RunLengthEncoding(std::string input)
{
	int size = input.length();
	std::string returner;

	int offset = 0;
	while (offset < size)
	{
		int end = offset;
		returner.append(ReturnRunLengthString(input, offset, &end));
		offset = end;
	}

	return returner;
}
/// <summary>
/// Decompressing the Run Length Encoding Compression by expanding out characters being represented by numbers
/// </summary>
/// <param name="input">The string to be decompressed</param>
/// <returns>The decompressed string</returns>
std::string Compressor::I_RunLengthEncoding(std::string input)
{
	int size = input.length();
	std::string returner;

	int offset = 0, count = 0, charamount = 0;
	char focus;
	while (offset + count < size)
	{
		if (input[offset + count] == 1)
		{
			//add all characters before repeat
			returner.append(input.substr(offset, count - 1));

			//get character and amount
			focus = input[offset + count-1];
			//move character pointer to next character
			count++;
			//move offset to start at next character
			offset = offset + count;
			count = 0;

			while (offset + count < size && input[offset + count] != 1)
			{
				count++;
			}
			
			charamount = std::stoi(input.substr(offset, count));

			//add characters to string
			while (charamount > 0)
			{
				returner += focus;
				charamount--;
			}

			//move offset to next character and set count to 0
			offset = offset + count + 1;
			count = 0;
		}
		else
			count++;
	}

	//add trailing characters
	if (count > 0)
		returner.append(input.substr(offset));

	return returner;
}