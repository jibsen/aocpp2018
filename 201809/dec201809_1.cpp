//
// Advent of Code 2018, day 9, part one and two
//

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

constexpr int num_players = 416;
constexpr long long last_marble = 71617;

class Circle {
	using iterator = std::list<long long>::iterator;

	std::list<long long> list = { 0 };
	iterator cur = list.begin();

	iterator next(iterator it, int n) {
		for (int i = 0; i < n; ++i) {
			it = std::next(it);
			if (it == list.end()) {
				it = list.begin();
			}
		}
		return it;
	}

	iterator prev(iterator it, int n) {
		for (int i = 0; i < n; ++i) {
			if (it == list.begin()) {
				it = list.end();
			}
			it = std::prev(it);
		}
		return it;
	}

public:
	long long add_marble(long long m);
};

long long Circle::add_marble(long long m)
{
	if (m % 23 == 0) {
		auto it = prev(cur, 7);

		long long score = m + *it;

		cur = list.erase(it);

		return score;
	}

	auto it = next(cur, 2);

	cur = list.insert(it, m);

	return 0;
}

int main()
{
	Circle circle;

	std::vector<long long> scores(num_players);

	auto player = scores.begin();

	long long marble = 1;

	for (; marble <= last_marble; ++marble) {
		*player += circle.add_marble(marble);

		if (++player == scores.end()) {
			player = scores.begin();
		}
	}

	std::cout << "score after " << marble - 1 << " marbles: "
	          << *std::max_element(scores.begin(), scores.end()) << '\n';

	for (; marble <= last_marble * 100; ++marble) {
		*player += circle.add_marble(marble);

		if (++player == scores.end()) {
			player = scores.begin();
		}
	}

	std::cout << "score after " << marble - 1 << " marbles: "
	          << *std::max_element(scores.begin(), scores.end()) << '\n';

	return 0;
}
