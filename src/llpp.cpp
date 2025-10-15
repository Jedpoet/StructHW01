#include "llpp.hpp"
#include <utility>
#include <vector>

nodepp::nodepp() {
	next = nullptr;
}

linked_listpp::linked_listpp() {
	head = nullptr;
}

void linked_listpp::insert(long long id, int score) {
	// 如果id出現過，找到他並插入成績
	if ( ids.count(id) ) {
		nodepp *temp_ptr = head.get();
		// 遍歷 linked_listpp
		while ( temp_ptr != nullptr && temp_ptr->id != id ) {
			temp_ptr = temp_ptr->next.get();
		}
		if ( temp_ptr != nullptr ) {
			temp_ptr->scores.push_back(score);
		}
		return;
	}

	// 如果id沒出現過，創建新Node並插入成績

	// 建立新Node
	std::unique_ptr<nodepp> new_nodepp = std::make_unique<nodepp>();
	new_nodepp->id = id;
	new_nodepp->scores.push_back(score);
	ids.insert(id);

	// 如果還沒有任何數據，直接寫在head
	if ( !head ) {
		head = std::move(new_nodepp);
		return;
	}

	if ( new_nodepp.get() < head.get() ) {
		new_nodepp->next = std::move(head);
		head = std::move(new_nodepp);
		return;
	}

	nodepp *prev_node = head.get();
	nodepp *next_node = head->next.get();
	// 只有head而已

	// 遍歷linked_listpp 尋找新Node的位置
	while ( next_node != nullptr && next_node < new_nodepp.get() ) {
		prev_node = next_node;
		next_node = next_node->next.get();
	}
	new_nodepp->next = std::move(prev_node->next);
	prev_node->next = std::move(new_nodepp);
}

std::vector<int> linked_listpp::search(long long id) {
	// 如果id出現過，找到他並插入成績
	if ( ids.count(id) ) {
		nodepp *temp_ptr = head.get();
		// 遍歷 linked_listpp
		while ( temp_ptr != nullptr && temp_ptr->id != id ) {
			temp_ptr = temp_ptr->next.get();
		}

		return temp_ptr->scores;
	}
	else {
		return {-1};
	}
}

long long linked_listpp::sum() {
	nodepp *pin = head.get();
	long long total = 0;

	while ( pin != nullptr ) {
		for ( auto i = pin->scores.begin(); i != pin->scores.end(); i++ ) {
			total += *i;
		}
		pin = pin->next.get();
	}

	return total;
}
