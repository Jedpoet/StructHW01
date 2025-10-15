#include <vector>

const size_t ARRAY_SIZE = 1ULL << 20;

class static_array {
  public:
	static_array();
	void insert(long long id, int score);
	std::vector<int> search(long long id);
	long long sum();

  private:
	long long start;
	struct Node {
		std::vector<int> scores;
		long long next_id = -1;
	};
	std::vector<Node> a{ARRAY_SIZE};
};
