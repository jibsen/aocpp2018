//
// Advent of Code 2018, day 11, part two
//

#include <array>
#include <iostream>
#include <vector>

constexpr int serial_number = 5034;

constexpr int power_level(int x, int y, int serial)
{
	int rack_id = x + 10;
	int power_level = rack_id * y;
	power_level += serial;
	power_level *= rack_id;
	power_level = (power_level / 100) % 10;

	return power_level - 5;
}

int main()
{
	std::vector<std::vector<int>> grid;

	grid.reserve(301);

	for (int i = 0; i < 301; ++i) {
		grid.emplace_back(301, 0);
	}

	for (int y = 1; y < grid.size(); ++y) {
		for (int x = 1; x < grid[y].size(); ++x) {
			grid[y][x] = power_level(x, y, serial_number);
		}
	}

	int max_sum = std::numeric_limits<int>::min();
	int max_x = -1;
	int max_y = -1;
	int max_s = -1;

	for (int y = 1; y < grid.size(); ++y) {
		for (int x = 1; x < grid[y].size(); ++x) {
			int sum = 0;
			int limit = static_cast<int>(std::min(grid.size() - y, grid[y].size() - x)) + 1;

			for (int s = 1; s < limit; ++s) {
				// Expand sum by next row and column
				for (int bx = x; bx < x + s; ++bx) {
					sum += grid[y + s - 1][bx];
				}
				for (int by = y; by < y + s; ++by) {
					sum += grid[by][x + s - 1];
				}
				sum -= grid[y + s - 1][x + s - 1];

				if (sum > max_sum) {
					max_sum = sum;
					max_x = x;
					max_y = y;
					max_s = s;
				}
			}
		}
	}

	std::cout << "largest sum " << max_sum << " at " << max_x << ',' << max_y << ',' << max_s << '\n';

	return 0;
}
