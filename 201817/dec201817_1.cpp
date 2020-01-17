//
// Advent of Code 2018, day 17, part one and two
//

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using Vein = std::pair<std::pair<int, int>, std::pair<int, int>>;

std::vector<Vein> read_veins()
{
	std::vector<Vein> veins;
	int first_coord = 0;
	int second_coord_first = 0;
	int second_coord_last = 0;
	char first_axis = 'x';
	char second_axis = 'y';

	while (std::scanf("%c=%d, %c=%d..%d ", &first_axis, &first_coord, &second_axis, &second_coord_first, &second_coord_last) == 5) {
		if (first_axis == 'x') {
			veins.push_back({{first_coord, first_coord}, {second_coord_first, second_coord_last}});
		}
		else {
			veins.push_back({{second_coord_first, second_coord_last}, {first_coord, first_coord}});
		}
	}

	return veins;
}

bool solid_below(const std::vector<std::string> &map, int x, int y)
{
	return map[y + 1][x] == '#' || map[y + 1][x] == '~';
}

int main()
{
	auto veins = read_veins();

	int min_x = std::numeric_limits<int>::max();
	int min_y = std::numeric_limits<int>::max();
	int max_x = std::numeric_limits<int>::min();
	int max_y = std::numeric_limits<int>::min();

	for (const auto &vein : veins) {
		min_x = std::min(min_x, vein.first.first);
		min_y = std::min(min_y, vein.second.first);
		max_x = std::max(max_x, vein.first.second);
		max_y = std::max(max_y, vein.second.second);
	}

	std::vector<std::string> map;

	map.reserve(max_y + 1);

	for (int y = 0; y <= max_y; ++y) {
		// max_x + 2 in case something spills over the right edge
		map.emplace_back(max_x + 2, '.');
	}

	for (const auto &vein : veins) {
		auto [x_range, y_range] = vein;

		for (int y = y_range.first; y <= y_range.second; ++y) {
			for (int x = x_range.first; x <= x_range.second; ++x) {
				map[y][x] = '#';
			}
		}
	}

	std::queue<std::pair<int, int>> queue;

	map[0][500] = '+';
	queue.push({500, 0});

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		// Move down until we hit something solid
		while (y < max_y && !solid_below(map, x, y)) {
			++y;
			map[y][x] = '|';
		}

		if (y == max_y) {
			continue;
		}

		int l = x;
		int r = x;

		// Find left edge, filling with wet sand
		while (map[y][l - 1] != '#') {
			--l;
			map[y][l] = '|';

			if (!solid_below(map, l, y)) {
				queue.push({l, y});
				break;
			}
 		}

		// Find right edge, filling with wet sand
		while (map[y][r + 1] != '#') {
			++r;
			map[y][r] = '|';

			if (!solid_below(map, r, y)) {
				queue.push({r, y});
				break;
			}
 		}

		// If both sides are clay, fill with water and continue one
		// line up
		if (map[y][l - 1] == '#' && map[y][r + 1] == '#') {
			for (int i = l; i <= r; ++i) {
				map[y][i] = '~';
			}
			queue.push({x, y - 1});
		}
	}

	for (const auto &line : map) {
		std::cout << line.substr(min_x - 1) << '\n';
	}

	int num_wet_tiles = 0;
	int num_water = 0;

	for (int y = min_y; y <= max_y; ++y) {
		for (char ch : map[y]) {
			num_wet_tiles += static_cast<int>(ch == '|' || ch == '~');
			num_water += static_cast<int>(ch == '~');
		}
	}

	std::cout << num_wet_tiles << '\n';
	std::cout << num_water << '\n';

	return 0;
}
