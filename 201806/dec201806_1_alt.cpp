//
// Advent of Code 2018, day 6, part one
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

// The first solution finds the closest coordinate by checking them all for
// each location in the bounding box, storing only an array of area sizes.
// This takes O(N * M) time and O(M) space, where N is the size of the
// bounding box, and M is the number of coordinates.
//
// We can instead do a BFS starting at all coordinates to fill in the
// bounding box, and then count the area sizes. This takes O(N) time, but
// also uses O(N) space.
//
// For input of sizes like in this problem both are more or less instant,
// but for instance 5000 random coordinates in the range [1, 1000] took 15
// seconds to process with the first, and less than one second with this
// version.

using Coordinate = std::pair<int, int>;

std::vector<Coordinate> read_coordinates()
{
	std::vector<Coordinate> coords;
	int x = 0;
	int y = 0;
	char sep = ',';

	while (std::cin >> x >> sep >> y && sep == ',') {
		coords.push_back({x, y});
	}

	return coords;
}

int manhattan_distance(const Coordinate &lhs, const Coordinate &rhs)
{
	return std::abs(lhs.first - rhs.first) + std::abs(lhs.second - rhs.second);
}

int main()
{
	auto coords = read_coordinates();

	// Find bounding box for coordinates
	int min_x = std::numeric_limits<int>::max();
	int min_y = std::numeric_limits<int>::max();
	int max_x = std::numeric_limits<int>::min();
	int max_y = std::numeric_limits<int>::min();

	for (const auto &coord : coords) {
		min_x = std::min(min_x, coord.first);
		min_y = std::min(min_y, coord.second);
		max_x = std::max(max_x, coord.first);
		max_y = std::max(max_y, coord.second);
	}

	// We assume the coordinates are positive and the minimum values
	// will be close to zero
	std::vector<std::vector<std::pair<int, int>>> grid;

	grid.resize(max_y + 1);

	for (int i = 0; i <= max_y; ++i) {
		grid[i].resize(max_x + 1, {0, -1});
	}

	std::queue<std::tuple<int, int, int, int>> queue;

	for (int i = 0; i < coords.size(); ++i) {
		queue.push({coords[i].first, coords[i].second, i, 0});
	}

	while (!queue.empty()) {
		auto [x, y, idx, dist] = queue.front();
		queue.pop();

		if (auto [cur_idx, cur_dist] = grid[y][x]; cur_dist != -1) {
			if (cur_idx != idx && cur_dist >= dist) {
				// Mark as equal distance (-1) or update with
				// closer distance
				grid[y][x] = {cur_dist == dist ? -1 : idx, dist};
			}

			continue;
		}

		grid[y][x] = {idx, dist};

		if (y - 1 >= min_y) {
			queue.push({x, y - 1, idx, dist + 1});
		}

		if (x - 1 >= min_x) {
			queue.push({x - 1, y, idx, dist + 1});
		}

		if (x + 1 <= max_x) {
			queue.push({x + 1, y, idx, dist + 1});
		}

		if (y + 1 <= max_y) {
			queue.push({x, y + 1, idx, dist + 1});
		}
	}

	// An area size of -1 will mean infinite
	std::vector<int> area_size(coords.size(), 0);

	for (int y = min_y; y <= max_y; ++y) {
		for (int x = min_x; x <= max_x; ++x) {
			auto [idx, dist] = grid[y][x];

			if (idx != -1 && area_size[idx] != -1) {
				if (x == min_x || x == max_x || y == min_y || y == max_y) {
					area_size[idx] = -1;
				}
				else {
					area_size[idx]++;
				}
			}
		}
	}

	std::cout << *std::max_element(area_size.begin(), area_size.end()) << '\n';

	return 0;
}
