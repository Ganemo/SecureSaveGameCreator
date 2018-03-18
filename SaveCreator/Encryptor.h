#pragma once

#include <string>

class Encryptor
{
public:
	static std::string EncryptInputWithKey(std::string input, std::string key);
	static std::string DecryptInputWithKey(std::string input, std::string key);
};

