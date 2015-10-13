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
	std::vector<EnvelopeLine> LoadFile(std::string file, bool p_normalize);

private:
	EnvelopeLine Split(std::string p_line);
	std::vector<EnvelopeLine> NormalizeVec(std::vector<EnvelopeLine> p_envs);
};

