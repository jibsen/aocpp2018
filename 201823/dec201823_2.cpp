//
// Advent of Code 2018, day 23, part two
//

// I started out solving this by finding the largest clique of mutually
// overlapping bots and finding a common point for it. This worked for
// the input provided, but not in general.
//
// After a lot of time trying to find the bug in my code to find a common
// point for a clique, I realized the problem was that mutually overlapping
// does not guarantee there is a common point in dimensions higher than two.
//
// So instead this version finds the point with the most bots in range by
// searching an octree, exploring the octets with the most bots in range
// first. This works well for the input provided, but has terrible worst-
// case behavior.
//
// On a side note, checking if a cube and an octahedron overlap appears
// somewhat involved (chances are I missed an easy way to check), the options
// are:
//
//   - Center of octahedron is within the range of coordinates of the cube in
//     all three dimensions (inside the cube).
//   - Center of octahedron is within the range of coordinates of the cube in
//     two dimensions but outside in the third. In this case we need to check
//     if the octahedron overlaps the side facing it.
//   - Center of octahedron is within the range of coordinates of the cube in
//     one dimension, but outside in the two others. In this case the edge of
//     the octahedron facing the cube may overlap the edge of the cube.
//   - Otherwise the octahedron is outside the range of coordinates of the
//     cube in all three dimensions, and only overlaps if one of the corners
//     of the cube intersects one of the faces of the octahedron.

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <queue>
#include <tuple>
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

int manhattan_distance(const Nanobot &bot, int x, int y, int z)
{
	return std::abs(bot.x - x) + std::abs(bot.y - y) + std::abs(bot.z - z);
}

bool is_in_range(const Nanobot &bot, int x, int y, int z)
{
	return manhattan_distance(bot, x, y, z) <= bot.r;
}

bool in_range_side(int bot_check_side, int bot_other_side_1, int bot_other_side_2, int bot_r,
                   int check_side_min, int check_side_max,
                   int other_side_1_min, int other_side_1_max,
                   int other_side_2_min, int other_side_2_max)
{
	// Check other sides are within range of cube coordinates, and one-
	// dimensional Manhattan distance to check side of cube is within range
	if (bot_other_side_1 >= other_side_1_min && bot_other_side_1 <= other_side_1_max
	 && bot_other_side_2 >= other_side_2_min && bot_other_side_2 <= other_side_2_max) {
		if ((bot_check_side > check_side_max && bot_check_side - bot_r <= check_side_max)
		 || (bot_check_side < check_side_min && bot_check_side + bot_r >= check_side_min)) {
			 return true;
		}

	}

	return false;
}

bool in_range_edge(int bot_in_range_side, int bot_other_side_1, int bot_other_side_2, int bot_r,
                   int in_range_side_min, int in_range_side_max,
                   int other_side_1_min, int other_side_1_max,
                   int other_side_2_min, int other_side_2_max)
{
	// Check in range side is within range of cube coordinates, and two-
	// dimensional Manhattan distance to one of the edges of cube
	// perpendicular to that side is within range
	if (bot_in_range_side >= in_range_side_min && bot_in_range_side <= in_range_side_max) {
		int dist1 = std::abs(bot_other_side_1 - other_side_1_max) + std::abs(bot_other_side_2 - other_side_2_max);
		int dist2 = std::abs(bot_other_side_1 - other_side_1_max) + std::abs(other_side_2_min - bot_other_side_2);
		int dist3 = std::abs(other_side_1_min - bot_other_side_1) + std::abs(bot_other_side_2 - other_side_2_max);
		int dist4 = std::abs(other_side_1_min - bot_other_side_1) + std::abs(other_side_2_min - bot_other_side_2);

		int min_dist = std::min(std::min(dist1, dist2), std::min(dist3, dist4));

		return min_dist <= bot_r;
	}

	return false;
}

int num_in_range(const std::vector<Nanobot> &bots, int x, int y, int z, int d)
{
	static const std::array<std::tuple<int, int, int>, 8> corners = {{
		{  1,  1,  1 },
		{ -1,  1,  1 },
		{  1, -1,  1 },
		{ -1, -1,  1 },
		{  1,  1, -1 },
		{ -1,  1, -1 },
		{  1, -1, -1 },
		{ -1, -1, -1 }
	}};

	int x_min = x;
	int x_max = x + d - 1;
	int y_min = y;
	int y_max = y + d - 1;
	int z_min = z;
	int z_max = z + d - 1;

	int num = 0;

	for (const auto &bot : bots) {
		// Check if center of octahedron is inside cube
		if (bot.x >= x_min && bot.x <= x_max
		 && bot.y >= y_min && bot.y <= y_max
		 && bot.z >= z_min && bot.z <= z_max) {
			 ++num;
			 continue;
		}

		// Check if octahedron overlaps side facing it
		if (in_range_side(bot.x, bot.y, bot.z, bot.r, x_min, x_max, y_min, y_max, z_min, z_max)
		 || in_range_side(bot.y, bot.x, bot.z, bot.r, y_min, y_max, x_min, x_max, z_min, z_max)
		 || in_range_side(bot.z, bot.x, bot.y, bot.r, z_min, z_max, x_min, x_max, y_min, y_max)) {
			 ++num;
			 continue;
		}

		// Check if edge of octahedron facing the cube overlaps edge
		// of cube
		if (in_range_edge(bot.x, bot.y, bot.z, bot.r, x_min, x_max, y_min, y_max, z_min, z_max)
		 || in_range_edge(bot.y, bot.x, bot.z, bot.r, y_min, y_max, x_min, x_max, z_min, z_max)
		 || in_range_edge(bot.z, bot.x, bot.y, bot.r, z_min, z_max, x_min, x_max, y_min, y_max)) {
			 ++num;
			 continue;
		}

		// Check if one of the corners of the cube intersects one of
		// the faces of the octahedron
		for (auto [dx, dy, dz] : corners) {
			if (is_in_range(bot, dx < 0 ? x_min : x_max, dy < 0 ? y_min : y_max, dz < 0 ? z_min : z_max)) {
				++num;
				break;
			}
		}
	}

	return num;
}

int main()
{
	static const std::array<std::tuple<int, int, int>, 8> octants = {{
		{  1,  1,  1 },
		{ -1,  1,  1 },
		{  1, -1,  1 },
		{ -1, -1,  1 },
		{  1,  1, -1 },
		{ -1,  1, -1 },
		{  1, -1, -1 },
		{ -1, -1, -1 }
	}};

	auto bots = read_nanobots();

	std::cout << "read " << bots.size() << " nanobots\n";

	int max_coord = 0;

	for (const auto &bot : bots) {
		max_coord = std::max(max_coord, std::abs(bot.x));
		max_coord = std::max(max_coord, std::abs(bot.y));
		max_coord = std::max(max_coord, std::abs(bot.z));
	}

	std::cout << "max coordinate = " << max_coord << '\n';

	int start_diameter = 1;

	while (start_diameter / 2 <= max_coord) {
		start_diameter *= 2;
	}

	std::cout << "start diameter = " << start_diameter << '\n';

	using Element = std::tuple<int, int, int, std::tuple<int, int, int>>;
	std::priority_queue<Element, std::vector<Element>, std::greater<>> pqueue;

	pqueue.push({static_cast<int>(bots.size()), 0, start_diameter, {-start_diameter / 2, -start_diameter / 2, -start_diameter / 2}});

	while (!pqueue.empty()) {
		auto [bots_not_in_range, distance, diameter, corner] = pqueue.top();
		auto [x, y, z] = corner;
		pqueue.pop();

		if (diameter == 1) {
			std::cout << x << ',' << y << ',' << z << ' ' << '\n';
			std::cout << "bots not in range = " << bots_not_in_range << '\n';
			std::cout << "distance = " << distance << '\n';

			break;
		}

		for (auto [dx, dy, dz] : octants) {
			int child_x = dx < 0 ? x : x + diameter / 2;
			int child_y = dy < 0 ? y : y + diameter / 2;
			int child_z = dz < 0 ? z : z + diameter / 2;

			int d = diameter / 2;

			// Distance to child corner closest to 0, 0, 0
			int dist_x = child_x < 0 ? child_x + d - 1 : child_x;
			int dist_y = child_y < 0 ? child_y + d - 1 : child_y;
			int dist_z = child_z < 0 ? child_z + d - 1 : child_z;
			int dist = std::abs(dist_x) + std::abs(dist_y) + std::abs(dist_z);

			int not_in_range = static_cast<int>(bots.size()) - num_in_range(bots, child_x, child_y, child_z, d);

			pqueue.push({not_in_range, dist, d, {child_x, child_y, child_z}});
		}
	}

	return 0;
}
