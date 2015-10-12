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
			std::cout << line << "\n";
		}
		file.close();
	}
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
			if (p_line[i] == ' ')
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
			if (p_line[i] == ' ')
			{
				first = false;
			}
			else
			{
				secondString += p_line[i];
			}
		}
	}

	env.time = atof(firstString.c_str());
	env.value = atof(secondString.c_str());
	return env;
}