#include <memory>
#include <set>
#include <vector>

struct nodepp {
  public:
	nodepp();
	long long id;
	std::vector<int> scores;
	std::unique_ptr<nodepp> next;
};

struct linked_listpp {
  public:
	linked_listpp();
	void insert(long long id, int score);
	std::vector<int> search(long long id);
	long long sum();

  private:
	std::set<long long> ids;
	std::unique_ptr<nodepp> head;
};
