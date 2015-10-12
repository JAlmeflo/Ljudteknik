#include "stdafx.h"
#include "BrkLoader.h"
#include <iostream>
#include <fstream>


BrkLoader::BrkLoader()
{
}

std::vector<EnvelopeLine> BrkLoader::LoadFile(std::string p_filename)
{
	std::vector<EnvelopeLine> envs = std::vector<EnvelopeLine>();
	std::string line;
	ifstream file(p_filename);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			auto env = Split(line);
			envs.push_back(env);
		}
		file.close();
	}
	envs = NormalizeVec(envs);
	return envs;
}

EnvelopeLine BrkLoader::Split(std::string p_line)
{
	std::string firstString = "";
	std::string secondString = "";
	EnvelopeLine env;


	bool first = true;
	for(unsigned int i = 0; i < p_line.length(); i++)
	{
		if (first)
		{
			if (p_line[i] == ' ' || p_line[i] == '\t')
			{
				first = false;
			}
			else
			{
				firstString += p_line[i];
			}
		}
		else
		{
			if (p_line[i] != ' ' || p_line[i] != '\t')
			{
				secondString += p_line[i];
			}
		}
	}

	env.time = atof(firstString.c_str());
	env.value = atof(secondString.c_str());
	return env;
}

std::vector<EnvelopeLine> BrkLoader::NormalizeVec(std::vector<EnvelopeLine> p_envs)
{
	double min = p_envs[0].value;
	double max = p_envs[0].value;

	for (unsigned int i = 0; i < p_envs.size(); i++)
	{
		if (min > p_envs[i].value)
		{
			min = p_envs[i].value;
		}
		if (max < p_envs[i].value)
		{
			max = p_envs[i].value;
		}
	}
	// vec: value / length
	double length = max - min;
	bool neg = min < 0.0;

	for (unsigned int i = 0; i < p_envs.size(); i++)
	{
		if (neg)
		{
			p_envs[i].value = (p_envs[i].value + abs(min)) / length;
		}
		else
		{
			p_envs[i].value = p_envs[i].value / length;
		}
	}

	return p_envs;
}