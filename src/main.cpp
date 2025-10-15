#include "d_array.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

	if ( argc != 3 ) {
		std::cout << "ERROR ARGC NUMBER\n";
		return 0;
	}

	enum STRUCT_TYPE { D_ARRAY, S_ARRAY, LLPP };
	STRUCT_TYPE type;
	std::string arg1 = argv[1];
	if ( arg1 == "1" ) {
		type = D_ARRAY;
	}
	else if ( arg1 == "2" ) {
		type = S_ARRAY;
	}
	else if ( arg1 == "3" ) {
		type = LLPP;
	}
	else {
		std::cout << "ERROR ARGUMENTS\n";
		return 0;
	}

	long long insert_time = pow(2, std::stoi(argv[2]));

	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);
	std::random_device rd;  // Non-deterministic random number source
	std::mt19937 gen(rd()); // Seed the engine with a random device

	// Define a distribution (e.g., uniform integer distribution)
	std::uniform_int_distribution<> distrib(0,
	                                        100); // Numbers between 1 and 100
	const size_t ID_SIZE = 1ULL << 20;
	std::uniform_int_distribution<> rand_id(
	    0,
	    ID_SIZE); // Numbers between 1 and 2^20
	std::chrono::steady_clock::time_point startTime =
	    std::chrono::steady_clock::now();
	std::cout << "-- insert start --\n";

	dynamic_array a;
	for ( long long i = 0; i < insert_time; i++ ) {
		a.insert(rand_id(gen), distrib(gen));
		// std::cout << i << std::endl;
	}
	std::cout << "-- insert finish --\n";

	std::chrono::steady_clock::time_point insertFinishTime =
	    std::chrono::steady_clock::now();

	// std::vector<std::vector<int>> temp;
	std::cout << "-- search start --\n";
	long long search_accumulator = 0;
	for ( long long i = 0; i < 100000; i++ ) {
		std::vector<int> temp = a.search(rand_id(gen));
		search_accumulator += temp.size();
	}
	std::cout << "search_accumulator: " << search_accumulator << "\n";
	// std::cout << temp.size() << std::endl;
	std::cout << "-- search finish --\n";

	std::chrono::steady_clock::time_point searchFinishTime =
	    std::chrono::steady_clock::now();

	std::cout << "-- sum start --\n";

	long long total = a.sum();
	std::cout << "Sum: " << total << "\n";
	std::cout << "-- sum finish --\n";

	std::chrono::steady_clock::time_point sumFinishTime =
	    std::chrono::steady_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
	    insertFinishTime - startTime);
	std::cout << "time 1: " << duration.count() << " ms\n";
	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(
	    searchFinishTime - insertFinishTime);
	std::cout << "time 2: " << duration1.count() << " ms\n";
	auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(
	    sumFinishTime - searchFinishTime);
	std::cout << "time 3: " << duration2.count() << " ms\n";

	std::ofstream ofs;

	ofs.open("result.log");
	if ( !ofs.is_open() ) {
		std::cout << "Failed to open file.\n";
		return 1; // EXIT_FAILURE
	}

	ofs << arg1 << " " << std::stoi(argv[2]) << " " << duration.count() << " "
	    << duration1.count() << " " << duration2.count() << "\n";
	ofs.close();

	return 0;
}
