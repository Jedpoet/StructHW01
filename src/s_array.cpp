#include "s_array.hpp"

static_array::static_array() {
	start = -1;
}

void static_array::insert(long long id, int score) {
	a[id].scores.push_back(score);
	if ( a[id].scores.size() > 0 ) {
		// ID 已經存在並被串入鏈結，只新增成績，然後返回。
		return;
	}
	// 如果ID是最小的或目前還沒有值
	if ( id < start || start == -1 ) {
		a[id].next_id = start;
		start = id;
	}
	else {
		long long prv = start;

		// 尋找ID位置
		while ( a[prv].next_id < id && a[prv].next_id != -1 ) {
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
