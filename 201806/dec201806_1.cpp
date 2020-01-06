//
// Advent of Code 2018, day 6, part one
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// Due to using the Manhattan distance, any location on a bounding box of the
// set of coordinates will continue having the same relative distance to all
// coordinates going outwards. For example:
//
//                             bbb.a.ccc
//   B     C      Bb.a.cC      bBb.a.cCc
//      A     =>  ..aAa..  =>  ...aAa...  =>  etc.
//   D     E      Dd.a.eE      dDd.a.eEe
//                             ddd.a.eee
//
// If we were using Euclidean distance, the area of A would be finite, but
// here it is infinite, as moving one step further north or south from A
// also moves one step further away from each of the other coordinates.
//
// So the area of any coordinate that has a location on the bounding box
// which it is closest to, is infinite.

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

	// An area size of -1 will mean infinite
	std::vector<int> area_size(coords.size(), 0);

	for (int y = min_y; y <= max_y; ++y) {
		for (int x = min_x; x <= max_x; ++x) {
			int min_distance = std::numeric_limits<int>::max();
			int closest_coord = -1;
			bool multiple_closest = false;

			for (int i = 0; i < coords.size(); ++i) {
				int distance = manhattan_distance({x, y}, coords[i]);

				if (distance < min_distance) {
					min_distance = distance;
					closest_coord = i;
					multiple_closest = false;
				}
				else if (distance == min_distance) {
					multiple_closest = true;
				}
			}

			if (!multiple_closest && area_size[closest_coord] != -1) {
				// If location is on bounding box, mark area
				// as infinite, otherwise increment it
				if (x == min_x || x == max_x || y == min_y || y == max_y) {
					area_size[closest_coord] = -1;
				}
				else {
					area_size[closest_coord]++;
				}
			}
		}
	}

	std::cout << *std::max_element(area_size.begin(), area_size.end()) << '\n';

	return 0;
}
