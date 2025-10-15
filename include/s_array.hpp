#include <vector>

class static_array {
  public:
	static_array();
	void insert(long long id, int score);
	std::vector<int> search(long long id);

  private:
	struct Node {
		long long id;
		std::vector<int> scores;
		long long next_id;
	};
	std::vector<Node> a(33554432);
	void exension();
};
