#pragma once
#include <random>
using namespace std;
class Random
{
	static std::mt19937 random;
public:
	static int Int(int min, int max);
	static float Float(float min, float max);
};

