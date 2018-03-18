#pragma once

#include "DataHandler.h"

class SaveCreator
{
//public functions
public:
	SaveCreator();
	~SaveCreator();

	bool SaveFile(std::string name, std::string path);
	bool LoadFile(std::string path);

//public member variables
public:
	DataHandler data;

//private functions
private:

//member variables
private:
};

