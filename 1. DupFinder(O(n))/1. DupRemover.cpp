// DupRemover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <chrono>
#include <numeric>

// approach 1
std::hash<int> IntHasher;
size_t GetModuloHash(int value, size_t size, size_t offset)
{
	//return offset + 1 + IntHasher(value) % (size - offset - 1);
	return offset + 1 + value % (size - offset - 1);
}
void DupRemover1(std::vector<int>& data, size_t offset) 
{
	if (data.size() - offset < 2) return;

	int sentinel = data[offset];
	for (size_t index = offset + 1; index < data.size();)
	{
		if (data[index] == sentinel) {
			index++;
			continue;
		}

		auto hash = GetModuloHash(data[index], data.size(), offset);
		if (index == hash) {
			index++;
			continue;
		}

		if (data[index] == data[hash]) {
			data[index] = sentinel;
			index++;
			continue;
		}

		if (data[hash] == sentinel) {
			std::swap(data[hash], data[index]);
			index++;
			continue;
		}

		auto hashHash = GetModuloHash(data[hash], data.size(), offset);
		if (hashHash != hash)
		{
			std::swap(data[index], data[hash]);
			if (hash < index)
				index++;
		}
		else 
		{
			index++;
		}
	}


	size_t swapPos = offset + 1;
	for (size_t i = offset + 1; i < data.size(); ++i)
	{
		if (data[i] != sentinel)
			if (i == GetModuloHash(data[i], data.size(), offset))
			{
				std::swap(data[i], data[swapPos++]);
			}
	}

	size_t sentinelPos = data.size();
	for (size_t i = swapPos; i < sentinelPos;) 
	{
		if (data[i] == sentinel)
		{
			std::swap(data[i], data[--sentinelPos]);
		}
		else 
		{
			++i;
		}
	}

	data.erase(data.begin() + sentinelPos, data.end());
	DupRemover1(data, swapPos);
}

// approach 2
void DupRemover2(std::vector<int>& data)
{
	std::sort(data.begin(), data.end());
	data.erase(std::unique(data.begin(), data.end()), data.end());
}

int _tmain(int argc, _TCHAR* argv[])
{
	//for (int MAXVALUE = 500000; MAXVALUE < 10000000 * 100; MAXVALUE *= 1.5)
	//{
		const int N = 10000000;
		const int MAXVALUE = 1*N;
		std::vector<int> data(N);
		//std::vector<int> data({ 2, 3, 6, 4, 4, 1, 7 });

		// generate N random numbers in the range [1, MAXVALUE]
		std::random_device rndDevice;
		std::mt19937 eng(rndDevice());
		std::uniform_int_distribution<int> dist(1, MAXVALUE);
		auto gen = std::bind(dist, eng);
		std::generate(begin(data), end(data), gen);
		
		std::vector<int> datacopy(data);

		// run algorithms
		auto start = std::chrono::steady_clock::now();
		DupRemover1(data, 0);
		auto stop = std::chrono::steady_clock::now();
		int time1 = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

		start = std::chrono::steady_clock::now();
		DupRemover2(datacopy);
		stop = std::chrono::steady_clock::now();
		int time2 = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

		//std::cout << (std::set<int>(datacopy.begin(), datacopy.end()) == std::set<int>(data.begin(), data.end()));

		std::cout << "Algorithm 1: " << time1 << " ms" << std::endl;
		std::cout << "Algorithm 2: " << time2 << " ms" << std::endl;
		double dupfactor = 100.0 * (N - data.size()) / N;
		std::cout.precision(2);
		std::cout << "Number of duplicates: " << dupfactor << "%" << std::endl;
		double speedup = (double)time2 / time1;
		std::cout << "Speedup: " << speedup << std::endl;
		//std::cout << dupfactor << " " << speedup << std::endl;
	//}
	return 0;
}

