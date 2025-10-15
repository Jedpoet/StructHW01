#include "d_array.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#define INSERT_TIME 11

int main() {
	long long insert_time = pow(2, INSERT_TIME);

	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);
	std::random_device rd;  // Non-deterministic random number source
	std::mt19937 gen(rd()); // Seed the engine with a random device

	// Define a distribution (e.g., uniform integer distribution)
	std::uniform_int_distribution<> distrib(0,
	                                        100); // Numbers between 1 and 100
	std::uniform_int_distribution<> rand_id(0,
	                                        2048); // Numbers between 1 and 100
	std::chrono::steady_clock::time_point startTime =
	    std::chrono::steady_clock::now();
	std::cout << "-- insert start --" << std::endl;

	dynamic_array a;
	for ( long long i = 0; i < insert_time; i++ ) {
		a.insert(rand_id(gen), distrib(gen));
		// std::cout << i << std::endl;
	}
	std::cout << "-- insert finish --" << std::endl;

	std::chrono::steady_clock::time_point firstTime =
	    std::chrono::steady_clock::now();

	// std::vector<std::vector<int>> temp;
	std::cout << "-- search start --" << std::endl;
	for ( long long i = 0; i < 100000; i++ ) {
		std::cout << i;
		std::vector<int> temp = a.search(i);
		for ( auto p = temp.begin(); p != temp.end(); p++ ) {
			std::cout << " " << *p;
		}
		std::cout << '\n';
	}
	// std::cout << temp.size() << std::endl;
	std::cout << "--search finish --" << std::endl;

	std::chrono::steady_clock::time_point endTime =
	    std::chrono::steady_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
	    firstTime - startTime);
	std::cout << "time 1: " << duration.count() << " ms" << std::endl;
	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(
	    endTime - firstTime);
	std::cout << "time 2: " << duration1.count() << " ms" << std::endl;

	return 0;
}
