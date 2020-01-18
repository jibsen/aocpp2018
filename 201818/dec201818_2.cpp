//
// Advent of Code 2018, day 18, part two
//

// Like in day 12 part two, the number of steps is too large to simulate,
// so we must see if we end up in a cycle.

#include <iostream>
#include <string>
#include <unordered_map>
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

int get_resource_value(const std::vector<std::string> &map)
{
	int num_wooded = 0;
	int num_lumberyards = 0;

	for (const auto &line : map) {
		for (char ch : line) {
			num_wooded += static_cast<int>(ch == '|');
			num_lumberyards += static_cast<int>(ch == '#');
		}
	}

	return num_wooded * num_lumberyards;
}

int main()
{
	auto map = read_map();

	std::unordered_map<int, int> seen;
	int prev_repeat_round = 0;
	int repeat_distance = 0;
	int num_repeats = 0;

	// Look for 5 resource values in a row that appeared earlier at the
	// same distance back
	for (int round = 1; round <= 2000; ++round) {
		auto next = update_map(map);
		map.swap(next);

		int resource_value = get_resource_value(map);

		if (auto [it, success] = seen.insert({resource_value, round}); !success) {
			std::cout << "round " << round << ": " << resource_value << " seen " << round - it->second << " back\n";

			if (round == prev_repeat_round + 1 && round - it->second == repeat_distance) {
				if (++num_repeats == 5) {
					break;
				}
			}
			else {
				num_repeats = 1;
				repeat_distance = round - it->second;
				seen[resource_value] = round;
			}

			prev_repeat_round = round;
		}
	}

	std::vector<int> resource_values;

	// Gather all resource values for the cycle
	for (int i = 0; i < repeat_distance; ++i) {
		auto next = update_map(map);
		map.swap(next);

		resource_values.push_back(get_resource_value(map));
	}

	std::cout << resource_values[(1000000000LL - (prev_repeat_round + 2)) % repeat_distance] << '\n';

	return 0;
}
