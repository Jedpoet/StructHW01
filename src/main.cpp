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
void run_and_time(long long insert_count, const std::string &struct_type_arg,
                  const std::string &k_arg,
                  std::uniform_int_distribution<long long> &id_dist,
                  const std::string &log_filename) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> score_dist(0, 100);

	T ds;

	// --- Insert ---
	auto startTime = std::chrono::steady_clock::now();
	std::cout << "--- insert start ---\n";
	for ( long long i = 0; i < insert_count; i++ ) {
		ds.insert(id_dist(gen), score_dist(gen));
	}
	std::cout << "--- insert finish ---\n";
	auto insertFinishTime = std::chrono::steady_clock::now();

	// --- Search ---
	std::cout << "--- search start ---\n";
	long long search_accumulator = 0;
	for ( long long i = 0; i < 100000; i++ ) {
		std::vector<int> temp = ds.search(id_dist(gen));
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
	ofs.open(log_filename, std::ios_base::app); // Append to the specified log
	if ( !ofs.is_open() ) {
		std::cout << "Failed to open file: " << log_filename << "\n";
		return;
	}
	ofs << struct_type_arg << " " << k_arg << " " << duration.count() << " "
	    << duration1.count() << " " << duration2.count() << "\n";
	ofs.close();
}

int main(int argc, char *argv[]) {
	if ( argc != 4 ) {
		std::cout << "ERROR: Incorrect number of arguments.\n";
		std::cout << "Usage: " << argv[0]
		          << " <struct_type> <k_value> <experiment_type>\n";
		return 1;
	}

	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);

	// --- Argument Parsing ---
	std::string struct_type_arg = argv[1];
	std::string k_arg = argv[2];
	std::string exp_type_arg = argv[3];

	enum STRUCT_TYPE { D_ARRAY, S_ARRAY, LLPP };
	STRUCT_TYPE type;
	if ( struct_type_arg == "1" ) {
		type = D_ARRAY;
	}
	else if ( struct_type_arg == "2" ) {
		type = S_ARRAY;
	}
	else if ( struct_type_arg == "3" ) {
		type = LLPP;
	}
	else {
		std::cout << "ERROR: Invalid struct_type argument.\n";
		return 1;
	}

	long long insert_count = pow(2, std::stoi(k_arg));

	// --- Experiment Configuration ---
	std::string log_filename;
	std::uniform_int_distribution<long long> id_dist;
	const long long ID_MAX = 1ULL << 20;

	if ( exp_type_arg == "uniform" ) {
		log_filename = "result.log";
		id_dist = std::uniform_int_distribution<long long>(1, ID_MAX);
		std::cout << "Running UNIFORM experiment...\n";
	}
	else if ( exp_type_arg == "dense" ) {
		log_filename = "result1.log";
		id_dist = std::uniform_int_distribution<long long>(1024, 2047);
		std::cout << "Running DENSE experiment...\n";
	}
	else {
		std::cout << "ERROR: Invalid experiment_type argument. Use 'uniform' or "
		             "'dense'.\n";
		return 1;
	}

	// --- Run Test ---
	if ( type == D_ARRAY ) {
		std::cout << "Testing Dynamic Array\n";
		run_and_time<dynamic_array>(insert_count, struct_type_arg, k_arg, id_dist,
		                            log_filename);
	}
	else if ( type == S_ARRAY ) {
		std::cout << "Testing Static Array\n";
		run_and_time<static_array>(insert_count, struct_type_arg, k_arg, id_dist,
		                           log_filename);
	}
	else if ( type == LLPP ) {
		std::cout << "Testing Linked List++\n";
		run_and_time<linked_listpp>(insert_count, struct_type_arg, k_arg, id_dist,
		                            log_filename);
	}

	return 0;
}
