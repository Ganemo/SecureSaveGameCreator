#include "stdafx.h"
#include "DataHandler.h"


DataHandler::DataHandler()
{
}


DataHandler::~DataHandler()
{
}

void DataHandler::AddHeader(std::string name, std::string info)
{
	header[name] = info;
}

void DataHandler::AddData(std::string name, std::string data)
{
	if (this->data[name] == NULL)
	{
		this->data[name] = new std::vector<std::pair<DataType, std::string>>();
	}

	this->data[name]->push_back(std::pair<DataType, std::string>(DataType::String,data));
}
void DataHandler::AddData(std::string name, int data)
{
	if (this->data[name] == NULL)
	{
		this->data[name] = new std::vector<std::pair<DataType, std::string>>();
	}

	this->data[name]->push_back(std::pair<DataType, std::string>(DataType::String, std::to_string(data)));
}
void DataHandler::AddData(std::string name, float data)
{
	if (this->data[name] == NULL)
	{
		this->data[name] = new std::vector<std::pair<DataType, std::string>>();
	}

	this->data[name]->push_back(std::pair<DataType, std::string>(DataType::String, std::to_string(data)));
}

void DataHandler::AddObject(std::string name, std::string classpath, DataTransformContainer* transform, std::vector<std::pair<std::string, std::string>>* properties)
{
	if (objects[name] == NULL)
	{
		objects[name] = new std::vector<ObjectContainer*>();
	}

	objects[name]->push_back(new ObjectContainer(classpath, transform, properties));
}
void DataHandler::AddObject(std::string name, ObjectContainer* container)
{
	if (objects[name] == NULL)
	{
		objects[name] = new std::vector<ObjectContainer*>();
	}

	objects[name]->push_back(container);
}


std::string DataHandler::GetHeader(const std::string name)
{
	return header[name];
}
std::vector<std::pair<DataType, std::string>>* DataHandler::GetData(const std::string name)
{
	return data[name];
}
std::vector<ObjectContainer*>* DataHandler::GetObjects(const std::string name)
{
	return objects[name];
}


std::string DataHandler::GetStringRespresentation()
{
	std::string returner = "";

	returner.append(GetHeaderString());
	returner.append(GetDataString());
	returner.append(GetObjectsString());

	return returner;
}

bool DataHandler::InterpretStringData(std::string input)
{
	int size = input.length();

	if (size < 6)
		return false;
	if (input[0] != '}')
		return false;

	int offset = 0, count = 0, openbrackets = 0, infocount = 0;
	while (offset + count < size)
	{
		if (input[offset + count] == '}')
			openbrackets--;
		else if (input[offset + count] == '{')
			openbrackets++;

		count++;

		if (openbrackets == 0)
		{
			//enter header info
			switch (infocount)
			{
			case 0:
				if (!ReadHeaderString(input.substr(offset, count)))
					return false;
				break;
			case 1:
				if (!ReadDataString(input.substr(offset, count)))
					return false;
				break;
			case 2:
				if (!ReadObjectsString(input.substr(offset, count)))
					return false;
				break;
			}
			//updating info bracket
			infocount++;
			//set new starting offset
			offset += count;
			//reset count
			count = 0;
		}
	}

	if (infocount < 2)
		return false;
	else
		return true;
}

bool DataHandler::DoesStringContainExcludedCharacters(const std::string input)
{
	if (input.find('{') == std::string::npos)
		return true;
	else if (input.find('}') == std::string::npos)
		return true;
	else if (input.find('~') == std::string::npos)
		return true;
	else if (input.find('|') == std::string::npos)
		return true;
	else
		return false;
}

std::string DataHandler::GetHeaderString()
{
	std::string returner = "{";

	for (auto it = header.begin(); it != header.end(); )
	{
		returner.append(it->first + "|" + it->second);
		
		it++;

		if(it != header.end())
			returner.append("~");
	}

	returner.append("}");
	return returner;
}
std::string DataHandler::GetDataString()
{
	std::string returner = "{";

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		returner.append(it->first + "{");

		for (auto it2 = it->second->begin(); it2 != it->second->end(); )
		{
			returner.append(it2->first + "|" + it2->second);

			it2++;

			if (it2 != it->second->end())
				returner.append("~");
		}

		returner.append("}");
	}

	returner.append("}");
	return returner;
}
std::string DataHandler::GetObjectsString()
{
	std::string returner = "{";

	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		returner.append(it->first + "{");

		for (auto it2 = it->second->begin(); it2 != it->second->end(); ++it2)
		{
			returner.append((*it2)->GetStringRepresentation());
		}

		returner.append("}");
	}

	returner.append("}");
	return returner;
}



bool DataHandler::ReadHeaderString(const std::string input)
{
	int size = input.length();

	if (size < 2)
		return false;

	if (input[0] != '{')
		return false;

	int offset = 1, count = 0;
	std::string name;
	while (offset + count < size)
	{
		if (input[offset + count] == '|')
		{
			name.append(input.substr(offset, count));

			offset += count + 1;
			count = 0;
		}
		else if (input[offset + count] == '~')
		{
			AddHeader(name, input.substr(offset, count));

			name.empty();

			offset += count + 1;
			count = 0;
		}
		else if (input[offset + count] == '}')
		{
			AddHeader(name, input.substr(offset, count));

			return true;
		} else
			count++;
	}

	return false;
}
bool DataHandler::ReadDataString(const std::string input)
{
	int size = input.length();

	if (size < 2)
		return false;
	if (input[0] != '{')
		return false;

	int offset = 1, count = 0;
	std::string name, dt;
	while (offset + count < size)
	{
		if (input[offset + count] == '{')
		{
			name.append(input.substr(offset, count));

			offset += count + 1;
			count = 0;
		}
		else if (input[offset + count] == '|')
		{
			dt.append(input.substr(offset, count));

			offset += count + 1;
			count = 0;
		}
		else if (input[offset + count] == '~' || input[offset + count] == '}')
		{
			switch (std::stoi(dt))
			{
			case 0: AddData(name, std::stoi(input.substr(offset, count)));
				break;
			case 1: AddData(name, std::stof(input.substr(offset, count)));
				break;
			case 2: AddData(name, input.substr(offset, count));
				break;
			default: return false;
			}

			offset += count + 1;
			count = 0;
			dt.empty();

			if (input[offset + count] == '}')
			{
				name.empty();
			}
		}
		else
			count++;
	}
}
bool DataHandler::ReadObjectsString(const std::string input)
{

}