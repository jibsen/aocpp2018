//
// Advent of Code 2018, day 3, part two
//

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

bool claims_overlap(const Claim &lhs, const Claim &rhs)
{
	return rhs.x < lhs.x + lhs.w && rhs.y < lhs.y + lhs.h
	    && lhs.x < rhs.x + rhs.w && lhs.y < rhs.y + rhs.h;
}

int main()
{
	auto claims = read_claims();

	// If two claims overlap, set their ids to 0
	for (int j = 1; j < claims.size(); ++j) {
		for (int i = 0; i < j; ++i) {
			if (claims_overlap(claims[i], claims[j])) {
				claims[i].id = 0;
				claims[j].id = 0;
			}
		}
	}

	for (const auto &claim : claims) {
		if (claim.id != 0) {
			std::printf("%d\n", claim.id);
		}
	}

	return 0;
}
