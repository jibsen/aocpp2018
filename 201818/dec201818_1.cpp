//
// Advent of Code 2018, day 18, part one
//

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> read_map()
{
	std::vector<std::string> map;
	std::string line;

	// Read map, adding border of spaces to make indexing easier
	while (std::getline(std::cin, line)) {
		map.push_back(" " + line + " ");
	}

	std::string empty_line(map[0].size(), ' ');

	map.insert(map.begin(), empty_line);
	map.push_back(empty_line);

	return map;
}

std::pair<int, int> adjacent_trees_lumberyards(const std::vector<std::string> &map, int x, int y)
{
	int num_trees = 0;
	int num_lumberyards = 0;

	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx == 0 && dy == 0) {
				continue;
			}

			num_trees += static_cast<int>(map[y + dy][x + dx] == '|');
			num_lumberyards += static_cast<int>(map[y + dy][x + dx] == '#');
		}
	}

	return {num_trees, num_lumberyards};
}

std::vector<std::string> update_map(const std::vector<std::string> &map)
{
	std::vector<std::string> res = map;

	for (int y = 1; y < map.size() - 1; ++y) {
		for (int x = 1; x < map[y].size() - 1; ++x) {
			auto [num_trees, num_lumberyards] = adjacent_trees_lumberyards(map, x, y);

			switch (map[y][x]) {
			case '.':
				if (num_trees >= 3) {
					res[y][x] = '|';
				}
				break;
			case '|':
				if (num_lumberyards >= 3) {
					res[y][x] = '#';
				}
				break;
			case '#':
				if (num_trees == 0 || num_lumberyards == 0) {
					res[y][x] = '.';
				}
				break;
			default:
				std::cerr << "unknown acre type\n";
				exit(1);
				break;
			}
		}
	}

	return res;
}

int main()
{
	auto map = read_map();

	for (int round = 1; round <= 10; ++round) {
		auto next = update_map(map);
		map.swap(next);
	}

	for (const auto &line : map) {
		std::cout << line << '\n';
	}

	int num_wooded = 0;
	int num_lumberyards = 0;

	for (const auto &line : map) {
		for (char ch : line) {
			num_wooded += static_cast<int>(ch == '|');
			num_lumberyards += static_cast<int>(ch == '#');
		}
	}

	std::cout << num_wooded * num_lumberyards << '\n';

	return 0;
}
