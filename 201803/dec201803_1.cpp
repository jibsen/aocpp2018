//
// Advent of Code 2018, day 3, part one
//

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <vector>

struct Claim {
	int id = 0;
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

std::vector<Claim> read_claims()
{
	std::vector<Claim> claims;
	Claim c;

	while (std::scanf("#%d @ %d,%d: %dx%d ", &c.id, &c.x, &c.y, &c.w, &c.h) == 5) {
		claims.push_back(c);
	}

	return claims;
}

int main()
{
	auto claims = read_claims();

	std::vector<std::vector<int>> fabric;

	fabric.reserve(1000);

	for (int i = 0; i < 1000; ++i) {
		fabric.emplace_back(1000, 0);
	}

	for (const auto &claim : claims) {
		for (int y = claim.y; y < claim.y + claim.h; ++y) {
			for (int x = claim.x; x < claim.x + claim.w; ++x) {
				fabric[y][x]++;
			}
		}
	}

	std::size_t multi_claimed = 0;

	for (const auto &row : fabric) {
		multi_claimed += std::count_if(row.begin(), row.end(), [](int c) { return c > 1; });
	}

	std::printf("%zu\n", multi_claimed);

	return 0;
}
