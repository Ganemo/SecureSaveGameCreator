#include "stdafx.h"
#include "SaveCreator.h"

#include "Encryptor.h"
#include "Compressor.h"

#include <iostream>
#include <fstream>

SaveCreator::SaveCreator() {}
SaveCreator::~SaveCreator() {}

/// <summary>
/// Saves a file with a given name at a given path.
/// The default save format is .sav, but can be different. 
/// Save files are compressed and encrypted
/// </summary>
/// <param name="name">The name of the save file</param>
/// <param name="path">The path where the file will be saved</param>
/// <param name="extension">The file extension (defaults to .sav)</param>
/// <returns>Whether the save was successful</returns>
bool SaveCreator::SaveFile(std::string name, std::string path, std::string extension = ".sav")
{
	//retrieve the string representation of the database
	std::string base = data.GetStringRespresentation();

	//compress the database string
	base = Compressor::CompressString(base);
	//encrypt the database string with key
	base = Encryptor::EncryptInputWithKey(base, "213lkascv90dafsna0fid");

	//write to file, starting with read file size to make loading easier
	std::ofstream output(path + "/" + name + extension, std::ios::binary);
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

/// <summary>
/// Load a file from a fully qualified path directly
/// </summary>
/// <param name="path">The fully qualified path</param>
/// <returns>Whether the load was successful</returns>
bool SaveCreator::LoadFile(std::string fullpath)
{
	//read file from path
	std::ifstream input(fullpath, std::ios::binary);
	std::string base;
	if (input.is_open())
	{
		//retrieve size of savefile
		size_t size;
		input.read(reinterpret_cast<char *>(&size), sizeof(size_t));

		//load save file contents
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

	//decrypt loaded file with password
	base = Encryptor::DecryptInputWithKey(base, "213lkascv90dafsna0fid");
	//decompress loaded file
	base = Compressor::DecompressString(base);

	//interpret information from save file to data
	return data.InterpretStringData(base);
}

/// <summary>
/// Load a file from a folder with a given name and extension
/// </summary>
/// <param name="name">Name of the save file</param>
/// <param name="folder">Folder containing the save file</param>
/// <param name="extension">The save file extension (defaults to .sav)</param>
/// <returns></returns>
bool SaveCreator::LoadFile(std::string name, std::string folder, std::string extension = ".sav")
{
	//Create full path and load file
	return LoadFile(folder + "/" + name + extension);
}