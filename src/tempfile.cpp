#include <sstream>
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
	std::ifstream ifs(getPath());
	std::stringstream buf;
	buf << ifs.rdbuf();
	return buf.str();
}