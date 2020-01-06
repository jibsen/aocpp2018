//
// Advent of Code 2018, day 6, part two
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

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

	int region_size = 0;

	// We assume the region is within the bounding box
	for (int y = min_y; y <= max_y; ++y) {
		for (int x = min_x; x <= max_x; ++x) {
			int total_dist = std::accumulate(coords.begin(), coords.end(), 0,
				[&](int acc, const auto &p) {
					return acc + manhattan_distance({x, y}, p);
				});

			if (total_dist < 10000) {
				++region_size;
			}
		}
	}

	std::cout << region_size << '\n';

	return 0;
}
