//
// Advent of Code 2018, day 10, part one and two
//

// Assuming the message is going to appear near the origin, we can find a
// range of likely timesteps by finding the minimum and maximum of x / dx
// and y / dy (where the velocities are non-zero).
//
// Then we can write the points to an image if all are within some area,
// and looking through the images we can see what timestep produces the
// message.

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point {
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;
};

std::vector<Point> read_points()
{
	std::vector<Point> points;
	std::string line;

	while (std::getline(std::cin, line)) {
		Point point;

		if (auto p = line.find('<'); p != std::string::npos) {
			std::istringstream in(line.substr(p + 1));

			char sep;

			in >> point.x >> sep >> point.y;
		}

		if (auto p = line.rfind('<'); p != std::string::npos) {
			std::istringstream in(line.substr(p + 1));

			char sep;

			in >> point.dx >> sep >> point.dy;
		}

		points.push_back(point);
	}

	return points;
}

std::pair<int, int> get_likely_range(const std::vector<Point> &points)
{
	int min_x = std::numeric_limits<int>::max();
	int min_y = std::numeric_limits<int>::max();
	int max_x = std::numeric_limits<int>::min();
	int max_y = std::numeric_limits<int>::min();

	for (const auto &p : points) {
		if (p.dx != 0) {
			min_x = std::min(min_x, std::abs(p.x / p.dx));
			max_x = std::max(max_x, std::abs(p.x / p.dx));
		}
		if (p.dy != 0) {
			min_y = std::min(min_y, std::abs(p.y / p.dy));
			max_y = std::max(max_y, std::abs(p.y / p.dy));
		}
	}

	int first = std::min(min_x, min_y);
	int last = std::max(max_x, max_y);

	return {first, last};
}

template<typename Img>
void write_plain_pbm(const Img &img, const std::string &filename)
{
	std::ofstream out(filename);

	// Write as Plain PBM image
	out << "P1\n";
	out << img[0].size() << ' ' << img.size() << '\n';

	int line_len = 0;

	for (const auto &row : img) {
		for (int pixel : row) {
			out << pixel << ' ';

			line_len += 2;

			if (line_len == 50) {
				out << '\n';
				line_len = 0;
			}
		}
	}
}

int main()
{
	auto points = read_points();

	auto [first, last] = get_likely_range(points);

	std::cout << "range: " << first << '-' << last << '\n';

	for (int i = first; i <= last; ++i) {
		static std::array<std::array<int, 250>, 250> img;

		for (auto &row : img) {
			row.fill(0);
		}

		int count = 0;

		for (const auto &p : points) {
			int x = p.x + i * p.dx;
			int y = p.y + i * p.dy;

			if (y >= 0 && y < img.size() && x >= 0 && x < img[y].size()) {
				img[y][x] = 1;
				++count;
			}
		}

		if (count == points.size()) {
			write_plain_pbm(img, "img" + std::to_string(i) + ".pbm");
		}
	}

	return 0;
}
