#include "d_array.hpp"
#include "llpp.hpp"
#include "s_array.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

template <typename T>
void run_and_time(long long insert_time, const std::string &arg1,
                  const std::string &arg2) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 100);
	const size_t ID_SIZE = 1ULL << 20;
	std::uniform_int_distribution<> rand_id(0, ID_SIZE);

	T ds;

	// --- Insert ---
	auto startTime = std::chrono::steady_clock::now();
	std::cout << "--- insert start ---\n";
	for ( long long i = 0; i < insert_time; i++ ) {
		ds.insert(rand_id(gen), distrib(gen));
	}
	std::cout << "--- insert finish ---\n";
	auto insertFinishTime = std::chrono::steady_clock::now();

	// --- Search ---
	std::cout << "--- search start ---\n";
	long long search_accumulator = 0;
	for ( long long i = 0; i < 100000; i++ ) {
		std::vector<int> temp = ds.search(rand_id(gen));
		search_accumulator += temp.size();
	}
	std::cout << "search_accumulator: " << search_accumulator << "\n";
	std::cout << "--- search finish ---\n";
	auto searchFinishTime = std::chrono::steady_clock::now();

	// --- Sum ---
	std::cout << "--- sum start ---\n";
	long long total = ds.sum();
	std::cout << "Sum: " << total << "\n";
	std::cout << "--- sum finish ---\n";
	auto sumFinishTime = std::chrono::steady_clock::now();

	// --- Timing and Logging ---
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
	ofs.open("result.log", std::ios_base::app); // Append to log
	if ( !ofs.is_open() ) {
		std::cout << "Failed to open file.\n";
		return;
	}
	ofs << arg1 << " " << arg2 << " " << duration.count() << " "
	    << duration1.count() << " " << duration2.count() << "\n";
	ofs.close();
}

int main(int argc, char *argv[]) {

	if ( argc != 3 ) {
		std::cout << "ERROR ARGC NUMBER\n";
		return 0;
	}

	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);

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

	if ( type == D_ARRAY ) {
		std::cout << "Testing Dynamic Array\n";
		run_and_time<dynamic_array>(insert_time, arg1, argv[2]);
	}
	else if ( type == S_ARRAY ) {
		std::cout << "Testing Static Array\n";
		run_and_time<static_array>(insert_time, arg1, argv[2]);
	}
	else if ( type == LLPP ) {
		std::cout << "Testing Linked List++\n";
		run_and_time<linked_listpp>(insert_time, arg1, argv[2]);
	}

	return 0;
}
