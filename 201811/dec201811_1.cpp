//
// Advent of Code 2018, day 11, part one
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

int sum_at(const std::vector<std::vector<int>> &grid, int x, int y)
{
	int sum = 0;

	for (int dy = 0; dy < 3; ++dy) {
		for (int dx = 0; dx < 3; ++dx) {
			sum += grid[y + dy][x + dx];
		}
	}

	return sum;
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

	for (int y = 1; y < grid.size() - 2; ++y) {
		for (int x = 1; x < grid[y].size() - 2; ++x) {
			int sum = sum_at(grid, x, y);

			if (sum > max_sum) {
				max_sum = sum;
				max_x = x;
				max_y = y;
			}
		}
	}

	std::cout << "largest sum " << max_sum << " at " << max_x << ',' << max_y << '\n';

	return 0;
}
