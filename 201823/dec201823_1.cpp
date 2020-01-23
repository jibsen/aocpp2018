//
// Advent of Code 2018, day 23, part one
//

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <vector>

struct Nanobot {
	int x = 0;
	int y = 0;
	int z = 0;
	int r = 0;
};

std::vector<Nanobot> read_nanobots()
{
	std::vector<Nanobot> bots;
	Nanobot bot;

	while (std::scanf("pos=<%d,%d,%d>, r=%d ", &bot.x, &bot.y, &bot.z, &bot.r) == 4) {
		bots.push_back(bot);
	}

	return bots;
}

int manhattan_distance(const Nanobot &lhs, const Nanobot &rhs)
{
	return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) + std::abs(lhs.z - rhs.z);
}

int main()
{
	auto bots = read_nanobots();

	auto strongest = std::max_element(bots.begin(), bots.end(),
		[](const auto &lhs, const auto &rhs) {
			return lhs.r < rhs.r;
		});

	std::size_t num_in_range = std::count_if(bots.begin(), bots.end(),
		[&](const auto &bot) {
			return manhattan_distance(*strongest, bot) <= strongest->r;
		});

	std::printf("%zu\n", num_in_range);

	return 0;
}
