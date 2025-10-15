#include "d_array.hpp"

dynamic_array::dynamic_array() {
	a = std::make_unique<std::vector<Node>>(1);
	capacity = 1;
	count = 0;
}

void dynamic_array::insert(long long id, int score) {
	long long low = 0, high = count - 1;
	long long target_idx = count;

	while ( low <= high ) {
		long long mid = low + (high - low) / 2;
		if ( (*a)[mid].id == id ) {
			(*a)[mid].scores.push_back(score);
			return;
		}
		if ( (*a)[mid].id < id ) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}
	target_idx = low;

	if ( count == capacity ) {
		exension();
	}

	for ( long long i = count; i > target_idx; --i ) {
		(*a)[i] = std::move((*a)[i - 1]);
	}

	(*a)[target_idx] = {id, std::vector<int>{score}};
	count++;
}

std::vector<int> dynamic_array::search(long long id) {
	long long low = 0, high = count - 1;
	while ( low <= high ) {
		long long mid = low + (high - low) / 2;
		if ( (*a)[mid].id == id ) {
			return (*a)[mid].scores;
		}
		if ( (*a)[mid].id < id ) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}

	return std::vector<int>(1, -1);
}

void dynamic_array::exension() {
	long long new_capacity = (capacity == 0) ? 1 : capacity * 10;
	std::unique_ptr<std::vector<Node>> b =
	    std::make_unique<std::vector<Node>>(new_capacity);
	for ( long long i = 0; i < count; i++ ) {
		(*b)[i] = std::move((*a)[i]);
	}
	capacity = new_capacity;
	a = std::move(b);
}

long long dynamic_array::sum() {
	long long total = 0;
	for ( auto i = (*a).begin(); i != (*a).end(); i++ ) {
		for ( auto j = i->scores.begin(); j != i->scores.end(); j++ ) {
			if ( *j == -1 ) {
				continue;
			}
			else {
				total += *j;
			}
		}
	}
	return total;
}
