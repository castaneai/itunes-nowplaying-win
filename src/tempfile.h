#pragma once
#include <string>

class TempFile
{
public:
	TempFile();
	~TempFile();
	const std::string getPath() const;
	const std::string readAllBytes() const;

private:
	std::string path;
};