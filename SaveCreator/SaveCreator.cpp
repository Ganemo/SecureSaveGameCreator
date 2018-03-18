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


	std::string base = data.GetStringRespresentation();
	base = comp->CompressString(base);
	base = enc->EncryptInputWithKey(base, "213lkascv90dafsna0fid");

	base = enc->DecryptInputWithKey(base, "213lkascv90dafsna0fid");
	base = comp->DecompressString(base);

	//data.InterpretStringData(base);

	SaveCreator* sv = new SaveCreator();
	sv->data.InterpretStringData(base);

	return true;
}

bool SaveCreator::LoadFile(std::string path)
{
	return false;
}