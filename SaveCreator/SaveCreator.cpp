#include "stdafx.h"
#include "SaveCreator.h"

#include "Encryptor.h"
#include "Compressor.h"

#include <iostream>
#include <fstream>

SaveCreator::SaveCreator()
{
	
}

SaveCreator::~SaveCreator()
{
}

bool SaveCreator::SaveFile(std::string name, std::string path)
{
	std::string base = data.GetStringRespresentation();

	base = Compressor::CompressString(base);
	base = Encryptor::EncryptInputWithKey(base, "213lkascv90dafsna0fid");

	std::ofstream output(path + "/" + name + ".sav", std::ios::binary);
	if (output.is_open()) {
		size_t size = base.size();
		output.write(reinterpret_cast<char *>(&size), sizeof(size_t));
		output.write(base.c_str(), size);
		output.close();
	}
	else
		return false;

	return true;
}

bool SaveCreator::LoadFile(std::string path)
{
	std::ifstream input(path, std::ios::binary);
	std::string base;
	if (input.is_open())
	{
		size_t size;
		input.read(reinterpret_cast<char *>(&size), sizeof(size_t));

		char c;
		while (size > 0)
		{
			input.get(c);
			base += c;
			size--;
		}
		
		input.close();
	}
	else
		return false;

	base = Encryptor::DecryptInputWithKey(base, "213lkascv90dafsna0fid");
	base = Compressor::DecompressString(base);

	return data.InterpretStringData(base);;
}