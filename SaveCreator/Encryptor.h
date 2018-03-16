#pragma once

#include <string>

class Encryptor
{
public:
	Encryptor();
	~Encryptor();

	std::string EncryptInputWithKey(std::string input, std::string key);
	std::string DecryptInputWithKey(std::string input, std::string key);
};

