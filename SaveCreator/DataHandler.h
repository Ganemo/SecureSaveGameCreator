#pragma once

#include <map>
#include <string>
#include <vector>

struct DataTransformContainer
{
	DataTransformContainer* location = NULL;
	DataTransformContainer* rotation = NULL;

public:
	DataTransformContainer() {};
	DataTransformContainer(DataTransformContainer* loc, DataTransformContainer* rot)
	{
		location = loc;
		rotation = rot;
	};

	virtual std::string GetStringRepresentation()
	{
		return "{" + (location != NULL ? location->GetStringRepresentation() : "~") + "|" + (rotation != NULL ? rotation->GetStringRepresentation() : "~") + "}";
	};
};

struct DataLocation : public DataTransformContainer
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

struct DataLocation2D : public DataTransformContainer
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

struct DataRotation : public DataTransformContainer
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

struct DataRotation2D : public DataTransformContainer
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
		for (auto it = properties->begin(); it != properties->end(); ++it)
		{
			returner.append(it->first + "|" + it->second);
		}
		returner.append("}");

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

