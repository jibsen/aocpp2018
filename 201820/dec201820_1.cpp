//
// Advent of Code 2018, day 20, part one
//

// Discovering the map corresponds to exploring all possible paths that match
// the regular expression. In general this could quickly grow to a huge number
// as each option becomes a factor in the number of possibilities, for
// instance (N|S|E|W)(N|S|E|W)(N|S|E|W)(N|S|E|W) matches 256 paths.
//
// Luckily both the examples and the supplied input are especially simple
// regular expressions with the following two properties:
//
//   - The only options that are ever followed by a motion are detours
//   - Every option that contains the empty option is a detour
//
// That means we can discover the map by linearly scanning the regex and
// keeping track of the position the last option started at.

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);

		return (17 * 37 + h1) * 37 + h2;
	}
};

std::string read_regex()
{
	std::string line;

	std::getline(std::cin, line);

	return line;
}

bool is_simple_regex(const std::string &regex)
{
	for (int i = 0; i < regex.size(); ++i) {
		// The only options that are ever followed by a motion are
		// detours
		if (regex[i] == ')') {
			if (regex[i + 1] != '$' && regex[i + 1] != ')' && regex[i + 1] != '|') {
				if (regex[i - 1] != '|') {
					return false;
				}
			}
		}

		// Every option that contains the empty option is a detour
		if (regex[i] == '|' && regex[i + 1] == ')') {
			int dx = 0;
			int dy = 0;

			for (int j = i - 1; regex[j] != '('; --j) {
				dx += static_cast<int>(regex[j] == 'W') - static_cast<int>(regex[j] == 'E');
				dy += static_cast<int>(regex[j] == 'N') - static_cast<int>(regex[j] == 'S');
			}

			if (dx != 0 || dy != 0) {
				return false;
			}
		}

	}

	return true;
}

std::vector<std::string> discover_map_simple(const std::string &regex)
{
	std::vector<std::string> map;

	map.reserve(256);

	for (int i = 0; i < 256; ++i) {
		map.emplace_back(256, '#');
	}

	std::stack<std::pair<int, int>> stack;

	map[128][128] = ' ';

	int x = 128;
	int y = 128;

	for (auto it = std::next(regex.begin()); *it != '$'; ++it) {
		switch(*it) {
		case '(':
			stack.push({x, y});
			break;
		case '|':
			x = stack.top().first;
			y = stack.top().second;
			break;
		case ')':
			stack.pop();
			break;
		case 'N':
		case 'S':
		case 'E':
		case 'W':
			{
				int dx = static_cast<int>(*it == 'W') - static_cast<int>(*it == 'E');
				int dy = static_cast<int>(*it == 'N') - static_cast<int>(*it == 'S');

				x += dx;
				y += dy;

				map[y][x] = dx != 0 ? '|' : '-';

				x += dx;
				y += dy;

				map[y][x] = ' ';
			}
			break;
		default:
			std::cerr << "unknown char '" << *it << "'\n";
			exit(1);
			break;
		}
	}

	return map;
}

int find_longest_path(const std::vector<std::string> &map, int start_x, int start_y)
{
	std::unordered_set<std::pair<int, int>, PairHash> seen;
	std::queue<std::tuple<int, int, int>> queue;

	queue.push({start_x, start_y, 0});

	int max_dist = std::numeric_limits<int>::min();

	while (!queue.empty()) {
		auto [x, y, dist] = queue.front();
		queue.pop();

		max_dist = std::max(max_dist, dist);

		if (map[y + 1][x] != '#') {
			if (auto [it, success] = seen.insert({x, y + 2}); success) {
				queue.push({x, y + 2, dist + 1});
			}
		}

		if (map[y - 1][x] != '#') {
			if (auto [it, success] = seen.insert({x, y - 2}); success) {
				queue.push({x, y - 2, dist + 1});
			}
		}

		if (map[y][x - 1] != '#') {
			if (auto [it, success] = seen.insert({x - 2, y}); success) {
				queue.push({x - 2, y, dist + 1});
			}
		}

		if (map[y][x + 1] != '#') {
			if (auto [it, success] = seen.insert({x + 2, y}); success) {
				queue.push({x + 2, y, dist + 1});
			}
		}
	}

	return max_dist;
}

int main()
{
	auto regex = read_regex();

	if (is_simple_regex(regex)) {
		auto map = discover_map_simple(regex);

		std::cout << find_longest_path(map, 128, 128) << '\n';
	}

	return 0;
}
