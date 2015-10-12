#pragma once

#include <vector>

struct EnvelopeLine
{
	double time;
	double value;
};

class BrkLoader
{
public:
	BrkLoader();
	std::vector<EnvelopeLine> LoadFile(std::string file);

private:
	EnvelopeLine Split(std::string p_line);
};

