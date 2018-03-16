#include "stdafx.h"
#include "SaveCreator.h"

#include "Encryptor.h"
#include "Compressor.h"

SaveCreator::SaveCreator()
{
	
}

SaveCreator::~SaveCreator()
{
}

bool SaveCreator::SaveFile(std::string path)
{
	Compressor* comp = new Compressor();
	Encryptor* enc = new Encryptor();


	std::string str = data.GetStringRespresentation();
	str = comp->CompressString(str);
	str = enc->EncryptInputWithKey(str, "213lkascv90dafsna0fid");


	str = enc->DecryptInputWithKey(str, "213lkascv90dafsna0fid");
	str = comp->DecompressString(str);


	data.InterpretStringData(str);

	return true;
}

bool SaveCreator::LoadFile(std::string path)
{
	return false;
}