#include "FileReader.h"

http::FileReader::FileReader(const std::string& filename,bool refreshOnRead):_fileName(filename),_refreshOnRead(refreshOnRead)
{
	_currentFileReadOut = readfile(filename);
}

http::FileReader::~FileReader()
{
}

std::string http::FileReader::getFileAsString()
{
	if (_refreshOnRead)
	{
		refreshFile();
	}
	return _currentFileReadOut;
}

size_t http::FileReader::getSizeOfFile()
{
	return _currentFileReadOut.size();
}

void http::FileReader::refreshFile()
{

	_currentFileReadOut = readfile(_fileName);
}

std::string http::FileReader::readfile(const std::string& fname) const
{
	std::string lineFromFile, ret;
	std::ifstream file;
	file.open(fname);
	while (std::getline(file,lineFromFile))
	{
		ret += lineFromFile;
		ret += '\n';
	}

	return ret;
}
