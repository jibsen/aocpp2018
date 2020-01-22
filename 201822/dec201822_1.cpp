//
// Advent of Code 2018, day 22, part one
//

#include <iostream>
#include <unordered_map>
#include <utility>

constexpr int depth = 3558;
constexpr int target_x = 15;
constexpr int target_y = 740;

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);

		return (17 * 37 + h1) * 37 + h2;
	}
};

int geological_index(int x, int y);

int erosion_level(int x, int y)
{
	static std::unordered_map<std::pair<int, int>, int, PairHash> lookup;

	if (auto it = lookup.find({x, y}); it != lookup.end()) {
		return it->second;
	}

	int level = (geological_index(x, y) + depth) % 20183;

	lookup[{x, y}] = level;

	return level;
}

int geological_index(int x, int y)
{
	if (x == 0 && y == 0) {
		return 0;
	}

	if (x == target_x && y == target_y) {
		return 0;
	}

	if (y == 0) {
		return x * 16807;
	}

	if (x == 0) {
		return y * 48271;
	}

	return erosion_level(x - 1, y) * erosion_level(x, y - 1);
}

int region_type(int x, int y)
{
	return erosion_level(x, y) % 3;
}

int main()
{
	int total_risk_level = 0;

	for (int y = 0; y <= target_y; ++y) {
		for (int x = 0; x <= target_x; ++x) {
			total_risk_level += region_type(x, y);
		}
	}

	std::cout << total_risk_level << '\n';

	return 0;
}
