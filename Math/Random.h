#pragma once

#include <random>

class Random
{
public:
	static void init() 
	{
		sRandomEngine.seed(std::random_device()());
	}

	//Return a number between 0 and 1
	static float Float()
	{
		return (float)sDistribution(sRandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 sRandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> sDistribution;
};

