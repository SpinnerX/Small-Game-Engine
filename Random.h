#pragma once
#include <random>


class Random{
public:
	static void init(){
		randomEngine.seed(std::random_device()());
	}

	static float Float(){
		return (float)distribution(randomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
private:
	static std::mt19937 randomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> distribution;
};
