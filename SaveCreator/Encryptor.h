#pragma once

#include <string>

/// <summary>
/// Encryption functions for encrypting saves files.
/// Encryption is fast but not very secure, used to deter unknowledgable users from changing files
/// </summary>
class Encryptor
{
//public member variables
public:
	static std::string EncryptInputWithKey(std::string input, std::string key);
	static std::string DecryptInputWithKey(std::string input, std::string key);
};

