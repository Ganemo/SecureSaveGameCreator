#pragma once

#include "DataHandler.h"

/// <summary>
/// Class used to create save files.
/// Uses compression and encryption.
/// Stores information in DataHandler "data" member.
/// </summary>
class SaveCreator
{
//public functions
public:
	SaveCreator();
	~SaveCreator();

	bool SaveFile(std::string name, std::string path, std::string extension);
	bool LoadFile(std::string fullpath);
	bool LoadFile(std::string name, std::string folder, std::string extension);

//public member variables
public:
	DataHandler data;
};

