#include "stdafx.h"
#include "Encryptor.h"


Encryptor::Encryptor()
{
}


Encryptor::~Encryptor()
{
}


std::string Encryptor::EncryptInputWithKey(std::string input, const std::string key)
{
	int size = input.length(), keysize = key.length();
	for (int x = 0; x < size; ++x)
	{
		input[x] = input[x] ^ key[x % keysize];
	}

	return input;
}

std::string Encryptor::DecryptInputWithKey(std::string input, const std::string key)
{
	int size = input.length(), keysize = key.length();
	for (int x = 0; x < size; ++x)
	{
		input[x] = input[x] ^ key[x % keysize];
	}

	return input;
}