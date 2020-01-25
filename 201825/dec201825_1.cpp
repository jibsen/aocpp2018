//
// Advent of Code 2018, day 25, part one
//

#include <cmath>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using Point = std::tuple<int, int, int, int>;

std::vector<Point> read_points()
{
	std::vector<Point> points;
	Point p;
	char sep;

	while (std::cin >> std::get<0>(p) >> sep >> std::get<1>(p) >> sep >> std::get<2>(p) >> sep >> std::get<3>(p)) {
		points.push_back(p);
	}

	return points;
}

int manhattan_distance(const Point &lhs, const Point &rhs)
{
	return std::abs(std::get<0>(lhs) - std::get<0>(rhs))
	     + std::abs(std::get<1>(lhs) - std::get<1>(rhs))
	     + std::abs(std::get<2>(lhs) - std::get<2>(rhs))
	     + std::abs(std::get<3>(lhs) - std::get<3>(rhs));
}

bool in_same_component(const Point &lhs, const Point &rhs)
{
	return manhattan_distance(lhs, rhs) <= 3;
}

int main()
{
	auto points = read_points();

	std::cout << "read " << points.size() << " points\n";

	std::vector<std::vector<int>> adj(points.size());

	for (int j = 1; j < points.size(); ++j) {
		for (int i = 0; i < j; ++i) {
			if (in_same_component(points[i], points[j])) {
				adj[i].push_back(j);
				adj[j].push_back(i);
			}
		}
	}

	int num_constellations = 0;

	std::vector<bool> visited(points.size(), false);
	std::queue<int> queue;

	for (int i = 0; i < points.size(); ++i) {
		if (!visited[i]) {
			++num_constellations;

			queue.push(i);
			visited[i] = true;

			while (!queue.empty()) {
				int p = queue.front();
				queue.pop();

				for (int v : adj[p]) {
					if (!visited[v]) {
						visited[v] = true;
						queue.push(v);
					}
				}
			}
		}
	}

	std::cout << num_constellations << " constellations\n";

	return 0;
}
