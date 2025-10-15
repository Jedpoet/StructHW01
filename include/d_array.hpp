#include <memory>
#include <vector>

class dynamic_array {
  public:
	dynamic_array();
	void insert(long long id, int score);
	std::vector<int> search(long long id);

  private:
	struct Node {
		long long id;
		std::vector<int> scores;
	};

	std::unique_ptr<std::vector<Node>> a;
	long long capacity;
	long long count;

	void exension();
};