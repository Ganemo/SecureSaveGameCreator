#include "stdafx.h"
#include "Encryptor.h"

/// <summary>
/// Encrypt an input string with a given key
/// </summary>
/// <param name="input">The string to encrypt</param>
/// <param name="key">The key</param>
/// <returns>The encrypted string</returns>
std::string Encryptor::EncryptInputWithKey(std::string input, const std::string key)
{
	int size = input.length(), keysize = key.length();
	for (int x = 0; x < size; ++x)
	{
		input[x] = input[x] ^ key[x % keysize];
	}

	return input;
}

/// <summary>
/// Decrypt the input string with a given key
/// </summary>
/// <param name="input">The input to decrypt</param>
/// <param name="key">The key to decrypt with</param>
/// <returns>The decrypted string</returns>
std::string Encryptor::DecryptInputWithKey(std::string input, const std::string key)
{
	int size = input.length(), keysize = key.length();
	for (int x = 0; x < size; ++x)
	{
		input[x] = input[x] ^ key[x % keysize];
	}

	return input;
}