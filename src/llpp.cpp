#include "llpp.hpp"
#include <vector>

nodepp::nodepp() {
	next = nullptr;
}

linked_listpp::linked_listpp() {
	head = nullptr;
	tail = nullptr;
}

void linked_listpp::insert(long long id, int score) {
	// 如果還沒有任何數據，直接寫在head
	if ( !head ) {
		head = std::make_unique<nodepp>();
		head->id = id;
		head->scores.push_back(score);
		return;
	}

	// 如果id出現過，找到他並插入成績
	if ( ids.count(id) ) {
		nodepp *temp_ptr = head.get();
		// 遍歷 linked_listpp
		while ( temp_ptr->id != id || temp_ptr != nullptr ) {
			temp_ptr = temp_ptr->next.get();
		}
		// 如果還是沒找到，報錯
		if ( temp_ptr == nullptr ) {
			return;
		}
		temp_ptr->scores.push_back(score);
	}
	// 如果id沒出現過，創建新Node並插入成績
	else {
		// 建立新Node
		std::unique_ptr<nodepp> new_nodepp = std::make_unique<nodepp>();
		new_nodepp->id = id;
		new_nodepp->scores.push_back(score);

		nodepp *prev_node = head.get();
		// 如果head就比新ndoe大，取代head
		if ( head.get() < new_nodepp.get() ) {
			new_nodepp->next = std::move(head);
			head = std::move(new_nodepp);
		}
		nodepp *next_node = head->next.get();
		// 遍歷linked_listpp 尋找新Node的位置
		while ( next_node < new_nodepp.get() ) {
			prev_node = next_node;
			next_node = next_node->next.get();
		}
		new_nodepp->next = std::move(prev_node->next);
		prev_node->next = std::move(new_nodepp);
	}
}
