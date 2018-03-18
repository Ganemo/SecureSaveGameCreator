#pragma once

#include <map>
#include <string>
#include <vector>

struct DataTransformStruct
{
	virtual std::string GetStringRepresentation() = 0;
};

struct DataLocation : public DataTransformStruct
{
	float x = 0;
	float y = 0;
	float z = 0;

public:
	DataLocation(float x_loc, float y_loc, float z_loc)
	{
		x = x_loc;
		y = y_loc;
		z = z_loc;
	};

	std::string GetStringRepresentation() override
	{
		return std::to_string(x) + "|" + std::to_string(y) + "|" + std::to_string(z);
	};
};

struct DataLocation2D : public DataTransformStruct
{
	float x = 0;
	float y = 0;

public:
	DataLocation2D(float x_loc, float y_loc)
	{
		x = x_loc;
		y = y_loc;
	};

	std::string GetStringRepresentation() override
	{
		return std::to_string(x) + "|" + std::to_string(y);
	};
};

struct DataRotation : public DataTransformStruct
{
	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;

public:
	DataRotation(float pitch, float roll, float yaw)
	{
		this->pitch = pitch;
		this->roll = roll;
		this->yaw = yaw;
	};

	std::string GetStringRepresentation() override
	{
		return std::to_string(pitch) + "|" + std::to_string(roll) + "|" + std::to_string(yaw);
	};
};

struct DataRotation2D : public DataTransformStruct
{
	float rotation = 0.0f;

public:
	DataRotation2D(float rot)
	{
		rotation = rot;
	}

	std::string GetStringRepresentation() override
	{
		return std::to_string(rotation);
	};
};

struct DataTransformContainer
{
	DataTransformStruct* location = NULL;
	DataTransformStruct* rotation = NULL;

public:
	DataTransformContainer() {};
	DataTransformContainer(DataTransformStruct* loc, DataTransformStruct* rot)
	{
		location = loc;
		rotation = rot;
	};

	virtual std::string GetStringRepresentation()
	{
		return "{" + (location != NULL ? location->GetStringRepresentation() : "") + "~" + (rotation != NULL ? rotation->GetStringRepresentation() : "") + "}";
	};

	static DataTransformContainer* GetTransform(const std::string input)
	{
		int size = input.length();

		if (size < 4)
			return NULL;
		if (input[0] != '{')
			return NULL;

		int offset = 1, count = 0, openbrackets = 1;
		std::vector<std::string> vals;
		DataTransformStruct* pos = NULL;
		while (offset + count < size)
		{
			if (input[offset + count] == '|')
			{
				vals.push_back(input.substr(offset, count));

				offset += count + 1;
				count = 0;
			}
			else if (input[offset + count] == '~')
			{
				switch (vals.size())
				{
				case 0:	break;
				case 2:
					pos = new DataLocation2D(std::stof(vals[0]), std::stof(vals[1]));
					break;
				case 3:
					pos = new DataLocation(std::stof(vals[0]), std::stof(vals[1]), std::stof(vals[2]));
					break;
				default: return NULL;
					break;
				}

				vals.clear();
				offset += count + 1;
				count = 0;
			}
			else if (input[offset + count] == '}')
			{

				vals.push_back(input.substr(offset, count));

				switch (vals.size())
				{
				case 0:	return new DataTransformContainer(pos, NULL);
				case 1:
					return new DataTransformContainer(pos, new DataRotation2D(std::stof(vals[0])));
				case 3:
					return new DataTransformContainer(pos, new DataRotation(std::stof(vals[0]), std::stof(vals[1]), std::stof(vals[2])));
				default: return NULL;
					break;
				}
			}
			else
				count++;
		}

		return NULL;
	}
};

struct ObjectContainer
{
	std::string classpath;
	DataTransformContainer* transform;
	std::vector<std::pair<std::string, std::string>>* properties;

public:
	ObjectContainer(std::string classpath, DataTransformContainer* transform, std::vector<std::pair<std::string, std::string>>* properties)
	{
		this->classpath = classpath;
		this->transform = transform;
		this->properties = properties;
	}
	~ObjectContainer()
	{
		delete transform;
		delete properties;
	}

	std::string GetClasspath() { return classpath; }
	DataTransformContainer* GetTransform() { return transform; }
	std::vector<std::pair<std::string, std::string>>* GetProperties() { return properties; }

	std::string GetStringRepresentation()
	{
		std::string returner = classpath + "|";

		returner.append(transform->GetStringRepresentation() + "|");
		returner.append("{");
		for (auto it = properties->begin(); it != properties->end(); )
		{
			returner.append(it->first + "|" + it->second);
			it++;

			if (it != properties->end())
				returner.append("~");
		}
		returner.append("}");

		return returner;
	}

	static std::vector<std::pair<std::string, std::string>>* GetPropertiesFromString(std::string input)
	{
		int size = input.length();

		if (size < 2)
			return NULL;
		if (input[0] != '{')
			return NULL;

		std::vector<std::pair<std::string, std::string>>* returner = new std::vector<std::pair<std::string, std::string>>();

		if (size == 2)
			return returner;

		int offset = 1, count = 0;
		std::string str;
		while (offset + count < size)
		{
			if (input[offset + count] == '|')
			{
				str.append(input.substr(offset, count));

				offset += count + 1;
				count = 0;
			}
			else if (input[offset + count] == '~' || input[offset + count] == '}')
			{
				returner->push_back(std::pair<std::string, std::string>(str, input.substr(offset, count)));
				offset += count + 1;
				count = 0;
			}
			else
				count++;
		}

		return returner;
	}
};

enum DataType
{
	Integer,
	Float,
	String
};

class DataHandler
{
//public functions
public:
	DataHandler();
	~DataHandler();

	void AddHeader(std::string name, std::string info);
	
	void AddData(std::string name, std::string data);
	void AddData(std::string name, int data);
	void AddData(std::string name, float data);

	void AddObject(std::string name, std::string classpath, DataTransformContainer* transform, std::vector<std::pair<std::string, std::string>>* properties);
	void AddObject(std::string name, ObjectContainer* container);

	std::string GetHeader(const std::string name);
	std::vector<std::pair<DataType, std::string>>* GetData(const std::string name);
	std::vector<ObjectContainer*>* GetObjects(const std::string name);

	std::string GetStringRespresentation();
	bool InterpretStringData(std::string input);

//private functions
private:
	bool DoesStringContainExcludedCharacters(const std::string input);

	std::string GetHeaderString();
	std::string GetDataString();
	std::string GetObjectsString();

	bool ReadHeaderString(const std::string input);
	bool ReadDataString(const std::string input);
	bool ReadObjectsString(const std::string input);

//member variables
private:
	std::map<std::string, std::string> header;
	std::map<std::string, std::vector<std::pair<DataType, std::string>>*> data;
	std::map<std::string, std::vector<ObjectContainer*>*> objects;
};

