#include "stdafx.h"
#include "DataHandler.h"


DataHandler::DataHandler() {}

DataHandler::~DataHandler() {}

/// <summary>
/// Add data to the header values.
/// These values can be shallow loaded, should contain info you would want on a SaveLoad screen.
/// Only one value can be matched for each header tag.
/// </summary>
/// <param name="name">Tag for the header value</param>
/// <param name="info">Value to add to header</param>
void DataHandler::AddHeader(std::string name, std::string info)
{
	if(!DoesStringContainExcludedCharacters(name) && !DoesStringContainExcludedCharacters(info))
		header[name] = info;
}

/// <summary>
/// Add string data to the data values.
/// Data value names can have multiple data values associated with them.
/// </summary>
/// <param name="name">Tag for the data values</param>
/// <param name="data">Value to add to those data values</param>
void DataHandler::AddData(std::string name, std::string data)
{
	if (!DoesStringContainExcludedCharacters(name) && !DoesStringContainExcludedCharacters(data))
	{
		if (this->data[name] == NULL)
		{
			this->data[name] = new std::vector<std::pair<DataType, std::string>>();
		}

		this->data[name]->push_back(std::pair<DataType, std::string>(DataType::String, data));
	}
}
/// <summary>
/// Add integer data to the data values.
/// Data value names can have multiple data values associated with them.
/// </summary>
/// <param name="name">Tag for the data values</param>
/// <param name="data">Value to add to those data values</param>
void DataHandler::AddData(std::string name, int data)
{
	if (!DoesStringContainExcludedCharacters(name))
	{
		if (this->data[name] == NULL)
		{
			this->data[name] = new std::vector<std::pair<DataType, std::string>>();
		}

		this->data[name]->push_back(std::pair<DataType, std::string>(DataType::String, std::to_string(data)));
	}
}
/// <summary>
/// Add float data to the data values.
/// Data value names can have multiple data values associated with them.
/// </summary>
/// <param name="name">Tag for the data values</param>
/// <param name="data">Value to add to those data values</param>
void DataHandler::AddData(std::string name, float data)
{
	if (!DoesStringContainExcludedCharacters(name))
	{
		if (this->data[name] == NULL)
		{
			this->data[name] = new std::vector<std::pair<DataType, std::string>>();
		}

		this->data[name]->push_back(std::pair<DataType, std::string>(DataType::String, std::to_string(data)));
	}
}

/// <summary>
/// Add an object to the objects map.
/// Objects have associated classpaths, transform information, and associated properties stored as string pairs.
/// </summary>
/// <param name="name">Tag for the objects in object map</param>
/// <param name="classpath">Path to the class (blueprint in UE4) or class identifier helpful for loading</param>
/// <param name="transform">Transform information for the object</param>
/// <param name="properties">Properties relevant to that object</param>
void DataHandler::AddObject(std::string name, std::string classpath, DataTransformContainer* transform, std::vector<std::pair<std::string, std::string>>* properties)
{
	//makes sure input is does not contain excluded characters
	if (!DoesStringContainExcludedCharacters(name) && !DoesStringContainExcludedCharacters(classpath))
	{
		for (auto it = properties->begin(); it != properties->end(); ++it)
		{
			if (DoesStringContainExcludedCharacters(it->first) || DoesStringContainExcludedCharacters(it->second))
				return;
		}

		if (objects[name] == NULL)
		{
			objects[name] = new std::vector<ObjectContainer*>();
		}

		objects[name]->push_back(new ObjectContainer(classpath, transform, properties));
	}
}
/// <summary>
/// Add an object to the objects map.
/// Objects have associated classpaths, transform information, and associated properties stored as string pairs.
/// </summary>
/// <param name="name">Tag for the objects in object map</param>
/// <param name="container">The object to add to the objects map</param>
void DataHandler::AddObject(std::string name, ObjectContainer* container)
{
	//makes sure input is does not contain excluded characters
	if (!DoesStringContainExcludedCharacters(name) && !DoesStringContainExcludedCharacters(container->classpath))
	{
		for (auto it = container->properties->begin(); it != container->properties->end(); ++it)
		{
			if (DoesStringContainExcludedCharacters(it->first) || DoesStringContainExcludedCharacters(it->second))
				return;
		}

		if (objects[name] == NULL)
		{
			objects[name] = new std::vector<ObjectContainer*>();
		}

		objects[name]->push_back(container);
	}
}

/// <summary>
/// Retrieve information from header
/// </summary>
/// <param name="name">Header tag</param>
/// <returns></returns>
std::string DataHandler::GetHeader(const std::string name)
{
	return header[name];
}
/// <summary>
/// Retrieve information from data
/// </summary>
/// <param name="name">Data tag</param>
/// <returns></returns>
std::vector<std::pair<DataType, std::string>>* DataHandler::GetData(const std::string name)
{
	return data[name];
}
/// <summary>
/// Retrieve information from objects
/// </summary>
/// <param name="name">Object tag</param>
/// <returns></returns>
std::vector<ObjectContainer*>* DataHandler::GetObjects(const std::string name)
{
	return objects[name];
}

/// <summary>
/// Get the string representation of the stored database
/// </summary>
/// <returns>The database in string form</returns>
std::string DataHandler::GetStringRespresentation()
{
	std::string returner = "";

	//add header string
	returner.append(GetHeaderString());
	//add data string
	returner.append(GetDataString());
	//add objects string
	returner.append(GetObjectsString());

	return returner;
}

/// <summary>
/// Interpret a string to form a database
/// </summary>
/// <param name="input">The string to be interpreted</param>
/// <returns>Success</returns>
bool DataHandler::InterpretStringData(std::string input)
{
	int size = input.length();

	//if string is too small or starts with a wrong character
	//it is invalid
	if (size < 6)
		return false;
	if (input[0] != '{')
		return false;

	int offset = 0, count = 0, openbrackets = 0, infocount = 0;
	while (offset + count < size)
	{
		//counting the number of open and closed brackets
		if (input[offset + count] == '}')
			openbrackets--;
		else if (input[offset + count] == '{')
			openbrackets++;

		//moving the character counter to the right
		count++;

		if (openbrackets == 0)
		{
			//enter info into the right portion of the database
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

	//if the database didn't contain all 3 database sections, it is invalid
	if (infocount < 2)
		return false;
	else
		return true;
}

/// <summary>
/// Check if input string contains special characters used exclusively for interpretation
/// </summary>
/// <param name="input">The string being checked</param>
/// <returns>Whether string is valid</returns>
bool DataHandler::DoesStringContainExcludedCharacters(const std::string input)
{
	if (input.find('{') != std::string::npos)
		return true;
	else if (input.find('}') != std::string::npos)
		return true;
	else if (input.find('~') != std::string::npos)
		return true;
	else if (input.find('|') != std::string::npos)
		return true;
	else
		return false;
}

/// <summary>
/// Returns string representation of header
/// </summary>
/// <returns>The string representation of the header</returns>
std::string DataHandler::GetHeaderString()
{
	std::string returner = "{";

	for (auto it = header.begin(); it != header.end();)
	{
		returner.append(it->first + "|" + it->second);
		
		it++;

		if(it != header.end())
			returner.append("~");
	}

	returner.append("}");
	return returner;
}
/// <summary>
/// Returns string representation of data
/// </summary>
/// <returns>The string representaion of datas</returns>
std::string DataHandler::GetDataString()
{
	std::string returner = "{";

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		returner.append(it->first + "{");

		for (auto it2 = it->second->begin(); it2 != it->second->end(); )
		{
			returner.append(std::to_string(it2->first) + "|" + it2->second);

			it2++;

			if (it2 != it->second->end())
				returner.append("~");
		}

		returner.append("}");
	}

	returner.append("}");
	return returner;
}
/// <summary>
/// Returns string representation of objects
/// </summary>
/// <returns>The string representation of objects</returns>
std::string DataHandler::GetObjectsString()
{
	std::string returner = "{";

	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		returner.append(it->first + "{");

		for (auto it2 = it->second->begin(); it2 != it->second->end(); )
		{
			returner.append((*it2)->GetStringRepresentation());
			it2++;

			if (it2 != it->second->end())
				returner += '~';
		}

		returner.append("}");
	}

	returner.append("}");
	return returner;
}

/// <summary>
/// Reads a representation of header information and adds it to the header database
/// </summary>
/// <param name="input">The string to scan</param>
/// <returns>Success</returns>
bool DataHandler::ReadHeaderString(const std::string input)
{
	int size = input.length();

	//if input is too small or starts with the wrong character, return failure
	if (size < 2)
		return false;
	if (input[0] != '{')
		return false;

	int offset = 1, count = 0;
	std::string name;
	while (offset + count < size)
	{
		//check if spacing delimiter is found
		if (input[offset + count] == '|')
		{
			name.append(input.substr(offset, count));

			offset += count + 1;
			count = 0;
		}
		//check if termination delimiter is found
		else if (input[offset + count] == '~')
		{
			AddHeader(name, input.substr(offset, count));

			name.clear();

			offset += count + 1;
			count = 0;
		}
		//check if inal termination delimiter is found
		else if (input[offset + count] == '}')
		{
			AddHeader(name, input.substr(offset, count));

			return true;
		} else
			count++;
	}

	return false;
}
/// <summary>
/// Reads a representation of data information and adds it to the header database
/// </summary>
/// <param name="input">The string to scan</param>
/// <returns>Success</returns>
bool DataHandler::ReadDataString(const std::string input)
{
	int size = input.length();

	//if input is too small or starts with the wrong character, return failure
	if (size < 2)
		return false;
	if (input[0] != '{')
		return false;

	int offset = 1, count = 0;
	std::string name, dt;
	while (offset + count < size)
	{
		//check if next data chunk is starting
		if (input[offset + count] == '{')
		{
			name.clear();
			name.append(input.substr(offset, count));

			offset += count + 1;
			count = 0;
		}
		//check if value spacing delimiter is found
		else if (input[offset + count] == '|')
		{
			dt.append(input.substr(offset, count));

			offset += count + 1;
			count = 0;
		}
		//check if termination delimiter is found
		else if (input[offset + count] == '~' || input[offset + count] == '}')
		{
			if (dt.size() <= 0)
				break;

			//add appropriate value type to data
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
			dt.clear();

			if (input[offset + count] == '}')
			{
				name.clear();
			}
		}
		else
			count++;
	}

	return true;
}

/// <summary>
/// Retrieve substring between brackets
/// </summary>
/// <param name="string">String to scan</param>
/// <param name="startingbracket">Starting position on string</param>
/// <param name="ending">Final position of string</param>
/// <returns>The information between the brackets (inclusive)</returns>
std::string GetBracketContents(const std::string &string, int startingbracket, int* ending)
{
	int size = string.size();

	//if starting position is invalid or starting character isn't correct, return an empty string
	if (startingbracket >= size)
		return "";
	if (string[startingbracket] != '{')
		return "";

	int count = 0, openbrackets = 0;
	while (startingbracket + count < size)
	{
		//record number of open brackets
		if (string[startingbracket + count] == '{')
		{
			openbrackets++;
		}
		//close open brackets
		else if (string[startingbracket + count] == '}')
		{
			openbrackets--;

			//if all brackets have been closed, return the contents between them (inclusive)
			//also se the final offset position
			if (openbrackets == 0)
			{
				*ending = startingbracket + count;
				return string.substr(startingbracket, count+1);
			}
		}

		count++;
	}

	return "";
}
/// <summary>
/// Reads a representation of object information and adds it to the object database
/// </summary>
/// <param name="input">The string to scan</param>
/// <returns>Success</returns>
bool DataHandler::ReadObjectsString(const std::string input)
{
	int size = input.length();
	
	//if size is too small or starting character is invalid, return failure
	if (size < 2)
		return false;
	if (input[0] != '{')
		return false;

	int offset = 1, count = 0, breakcount = 0, openbrackets = 1;
	std::string name, classpth, temp;
	DataTransformContainer* trns = NULL;
	while (offset + count < size)
	{
		//check if open bracket has been hit
		if (input[offset + count] == '{')
		{
			openbrackets++;

			//if it is the 2nd open bracket, then set "name"
			if (openbrackets == 2)
			{
				name = input.substr(offset, count);

				offset += count + 1;
				count = 0;
			}
			//if it is any other number of open brackets, get content between brackets
			//Used to get transform or object properties
			else
			{
				int end;
				temp = GetBracketContents(input, offset + count, &end);

				openbrackets--;

				offset = end + 1;
				count = 0;
			}
		}
		//check if a value break is hit
		else if (input[offset + count] == '|')
		{
			breakcount++;

			//first break is the class path
			if (breakcount == 1)
			{
				classpth = input.substr(offset, count);
			}
			//second break is the transform
			else if (breakcount == 2)
			{
				trns = DataTransformContainer::GetTransform(temp);
			}

			offset += count+1;
			count = 0;
		}
		//check if closing bracket is hit
		else if (input[offset + count] == '}')
		{
			openbrackets--;

			//completed closing brackets meand object is complete and added to database
			if (openbrackets == 1)
			{
				AddObject(name, classpth, trns, ObjectContainer::GetPropertiesFromString(temp));

				//number of breaks is reset
				breakcount = 0;

				//temporary storage values are reset
				classpth.clear();
				temp.clear();
				trns = NULL;
			}


			offset += count + 1;
			count = 0;
		}
		//check if object completion delimiter is hit
		else if (input[offset + count] == '~')
		{
			//object is complete and added to the database
			AddObject(name, classpth, trns, ObjectContainer::GetPropertiesFromString(temp));

			//number of breaks is reset
			breakcount = 0;

			//temporary storage values are reset
			classpth.clear();
			temp.clear();
			trns = NULL;

			offset += count + 1;
			count = 0;
		}
		else
			count++;
	}

	//if there are open brackets left, return failure
	if (openbrackets == 0)
		return true;
	else
		return false;
}