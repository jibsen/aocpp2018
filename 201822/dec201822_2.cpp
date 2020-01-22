//
// Advent of Code 2018, day 22, part two
//

// We can use Dijkstra's algorithm to find the shortest path, we just have
// to handle arriving at the same coordinates with different tools equipped.

#include <array>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
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

struct TupleHash {
	template<typename... Ts>
	std::size_t operator()(const std::tuple<Ts...> &tuple) const noexcept
	{
		std::size_t hash = 17;

		std::apply([&hash](const auto &... args) {
			(..., (hash = hash * 37 + std::hash<Ts>()(args)));
		}, tuple);

		return hash;
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

int needed_tool(int cur_region, int cur_tool, int new_x, int new_y)
{
	// -1 = keep current, 0 = torch, 1 = climb, 2 = neither
	static const std::array<std::array<int, 3>, 3> lookup = {{
		{ -1,  1,  0 },
		{  1, -1,  2 },
		{  0,  2, -1 }
	}};

	if (new_x == target_x && new_y == target_y) {
		return 0;
	}

	int new_tool = lookup[cur_region][region_type(new_x, new_y)];

	return new_tool == -1 ? cur_tool : new_tool;
}

int main()
{
	std::unordered_map<std::tuple<int, int, int>, int, TupleHash> distance;
	std::priority_queue<std::tuple<int, int, int, int>, std::vector<std::tuple<int, int, int, int>>, std::greater<>> pqueue;

	pqueue.push({0, 0, 0, 0});

	int min_dist = std::numeric_limits<int>::max();

	while (!pqueue.empty()) {
		auto [dist, x, y, tool] = pqueue.top();
		pqueue.pop();

		if (x == target_x && y == target_y) {
			min_dist = std::min(min_dist, dist);
			continue;
		}

		// Skip if we are so far away from the target that taking the
		// shortest path back would not improve distance
		if (dist + std::abs(target_x - x) + std::abs(target_y - y) >= min_dist) {
			continue;
		}

		int cur_region = region_type(x, y);

		static const std::array<std::pair<int, int>, 4> directions = {{
			{ 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 }
		}};

		for (auto [dx, dy] : directions) {
			if (x + dx < 0 || y + dy < 0) {
				continue;
			}

			int new_tool = needed_tool(cur_region, tool, x + dx, y + dy);

			int new_dist = dist + 1 + (tool != new_tool ? 7 : 0);

			if (auto [it, success] = distance.insert({{x + dx, y + dy, new_tool}, new_dist}); !success) {
				if (new_dist < it->second) {
					it->second = new_dist;
					pqueue.push({new_dist, x + dx, y + dy, new_tool});
				}
			}
			else {
				pqueue.push({new_dist, x + dx, y + dy, new_tool});
			}
		}
	}

	std::cout << min_dist << '\n';

	return 0;
}
