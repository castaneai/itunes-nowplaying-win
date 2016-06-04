#include <vector>
#include <fstream>
#include <Windows.h>
#include "tempfile.h"

TempFile::TempFile()
{
	char buf[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, buf);
	GetTempFileName(buf, "itunes_win", 0, buf);
	path = buf;
}

TempFile::~TempFile()
{
	std::remove(getPath().c_str());
}

const std::string TempFile::getPath() const
{
	return path;
}

const std::string TempFile::readAllBytes() const
{
	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	auto pos = ifs.tellg();

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return std::string(result.begin(), result.end());
}