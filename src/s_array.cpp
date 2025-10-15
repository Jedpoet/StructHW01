#include "s_array.hpp"

static_array::static_array() {
	start = -1;
}

void static_array::insert(long long id, int score) {
	// If scores are not empty, it means the node is already in the linked list.
	// Just add the score and return.
	if ( !a[id].scores.empty() ) {
		a[id].scores.push_back(score);
		return;
	}

	// If we are here, it's a new ID.
	// Add the score first.
	a[id].scores.push_back(score);

	// 如果ID是最小的或目前還沒有值
	if ( start == -1 || id < start ) {
		a[id].next_id = start;
		start = id;
	}
	else {
		long long prv = start;

		// 尋找ID位置
		while ( a[prv].next_id != -1 && a[prv].next_id < id ) {
			prv = a[prv].next_id;
		}

		a[id].next_id = a[prv].next_id;
		a[prv].next_id = id;
	}
}

std::vector<int> static_array::search(long long id) {
	if ( a[id].scores.size() == 0 ) {
		return {-1};
	}
	else {
		return a[id].scores;
	}
}

long long static_array::sum() {
	long long pin = start;
	long long total = 0;
	while ( pin != -1 ) {
		for ( auto i = a[pin].scores.begin(); i != a[pin].scores.end(); i++ ) {
			total += *i;
		}
		pin = a[pin].next_id;
	}

	return total;
}
