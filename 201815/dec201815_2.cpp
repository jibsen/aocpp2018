//
// Advent of Code 2018, day 15, part two
//

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
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

struct Unit {
	int y = 0;
	int x = 0;
	int hp = 200;
	int ap = 3;
	char type = ' ';
};

bool operator<(const Unit &lhs, const Unit &rhs) {
	return lhs.y == rhs.y ? lhs.x < rhs.x : lhs.y < rhs.y;
}

std::vector<std::string> read_map()
{
	std::vector<std::string> map;
	std::string line;

	while (std::getline(std::cin, line)) {
		map.push_back(line);
	}

	return map;
}

bool has_enemy_in_range(const std::vector<std::string> &map, const Unit &unit)
{
	char enemy_type = unit.type == 'E' ? 'G' : 'E';

	return map[unit.y - 1][unit.x] == enemy_type
	    || map[unit.y][unit.x - 1] == enemy_type
	    || map[unit.y][unit.x + 1] == enemy_type
	    || map[unit.y + 1][unit.x] == enemy_type;
}

bool is_enemy_adjacant(const std::vector<std::string> &map, const Unit &unit, int x, int y)
{
	char enemy_type = unit.type == 'E' ? 'G' : 'E';

	return map[y - 1][x] == enemy_type
	    || map[y][x - 1] == enemy_type
	    || map[y][x + 1] == enemy_type
	    || map[y + 1][x] == enemy_type;
}

std::pair<int, int> find_best_move(const std::vector<std::string> &map, const Unit &unit)
{
	std::queue<std::tuple<int, int, int>> queue;

	std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash> prev;

	queue.push({unit.x, unit.y, 0});
	prev[{unit.x, unit.y}] = {-1, -1};

	std::vector<std::pair<int, int>> nearest;
	int nearest_dist = std::numeric_limits<int>::max();

	while (!queue.empty()) {
		auto [x, y, dist] = queue.front();
		queue.pop();

		if (dist > nearest_dist) {
			break;
		}

		if (is_enemy_adjacant(map, unit, x, y)) {
			nearest.push_back({y, x});
			nearest_dist = dist;
			continue;
		}

		// Continue search in NWES order so backtracking finds the
		// correct first move
		if (map[y - 1][x] == '.') {
			if (auto [it, success] = prev.insert({{x, y - 1}, {x, y}}); success) {
				queue.push({x, y - 1, dist + 1});
			}
		}

		if (map[y][x - 1] == '.') {
			if (auto [it, success] = prev.insert({{x - 1, y}, {x, y}}); success) {
				queue.push({x - 1, y, dist + 1});
			}
		}

		if (map[y][x + 1] == '.') {
			if (auto [it, success] = prev.insert({{x + 1, y}, {x, y}}); success) {
				queue.push({x + 1, y, dist + 1});
			}
		}

		if (map[y + 1][x] == '.') {
			if (auto [it, success] = prev.insert({{x, y + 1}, {x, y}}); success) {
				queue.push({x, y + 1, dist + 1});
			}
		}
	}

	if (!nearest.empty()) {
		std::sort(nearest.begin(), nearest.end());

		// Backtrack from first nearest in reading order
		auto [y, x] = nearest.front();

		for (;;) {
			auto [prev_x, prev_y] = prev[{x, y}];

			if (prev_x == unit.x && prev_y == unit.y) {
				return {x, y};
			}

			x = prev_x;
			y = prev_y;
		}
	}

	return {-1, -1};
}

void perform_move(std::vector<std::string> &map, Unit &unit)
{
	if (unit.type == 'X') {
		return;
	}

	if (has_enemy_in_range(map, unit)) {
		return;
	}

	auto [x, y] = find_best_move(map, unit);

	if (x != -1) {
		map[unit.y][unit.x] = '.';

		unit.x = x;
		unit.y = y;

		map[unit.y][unit.x] = unit.type;
	}
}

std::vector<Unit>::iterator find_alive_unit_at(std::vector<Unit> &units, int x, int y)
{
	return std::find_if(units.begin(), units.end(), [&](const Unit &unit) { return unit.type != 'X' && unit.x == x && unit.y == y; });
}

std::vector<Unit>::iterator find_best_enemy(std::vector<Unit> &units, const Unit &unit)
{
	std::vector<std::vector<Unit>::iterator> enemies;

	enemies.push_back(find_alive_unit_at(units, unit.x, unit.y - 1));
	enemies.push_back(find_alive_unit_at(units, unit.x - 1, unit.y));
	enemies.push_back(find_alive_unit_at(units, unit.x + 1, unit.y));
	enemies.push_back(find_alive_unit_at(units, unit.x, unit.y + 1));

	int lowest_hp = std::numeric_limits<int>::max();
	auto best_enemy = units.end();

	for (auto enemy : enemies) {
		if (enemy != units.end() && enemy->type != unit.type) {
			if (enemy->hp < lowest_hp) {
				lowest_hp = enemy->hp;
				best_enemy = enemy;
			}
		}
	}

	return best_enemy;
}

void perform_attack(std::vector<std::string> &map, std::vector<Unit> &units, Unit &unit)
{
	if (unit.type == 'X') {
		return;
	}

	if (!has_enemy_in_range(map, unit)) {
		return;
	}

	auto enemy = find_best_enemy(units, unit);

	if (enemy == units.end()) {
		return;
	}

	enemy->hp -= unit.ap;

	if (enemy->hp <= 0) {
		enemy->type = 'X';
		map[enemy->y][enemy->x] = '.';
	}
}

bool no_enemies_left(const std::vector<Unit> &units)
{
	int num_elfs = 0;
	int num_goblins = 0;

	for (const auto &unit : units) {
		num_elfs += static_cast<int>(unit.type == 'E');
		num_goblins += static_cast<int>(unit.type == 'G');
	}

	return num_elfs == 0 || num_goblins == 0;
}

std::pair<int, bool> simulate_combat(std::vector<std::string> map, int elf_ap)
{
	std::vector<Unit> units;

	int num_elves = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] == 'G' || map[y][x] == 'E') {
				units.push_back(Unit{y, x, 200, map[y][x] == 'E' ? elf_ap : 3, map[y][x]});

				num_elves += static_cast<int>(map[y][x] == 'E');
			}
		}
	}

	bool done = false;

	for (int round = 0; !done; ++round) {
		std::sort(units.begin(), units.end());

		for (auto &unit : units) {
			if (no_enemies_left(units)) {
				done = true;
				break;
			}

			perform_move(map, unit);
			perform_attack(map, units, unit);
		}

		// Remove dead units
		units.erase(std::remove_if(units.begin(), units.end(),
			[](const Unit &unit) {
				return unit.type == 'X';
			}), units.end());

		if (done) {
			int total_hp = std::accumulate(units.begin(), units.end(), 0,
				[](int acc, const Unit &unit) {
					return acc + unit.hp;
				});

			return {round * total_hp, units.front().type == 'E' && units.size() == num_elves};
		}
	}

	return {-1, false};
}

int main()
{
	auto map = read_map();

	// I started out writing a binary search to find the AP needed for
	// the elves to win (double AP until elves win, then binary search
	// back to find the lowest AP where they do).
	//
	// It turns out they do not win for every AP value above the first,
	// so we do a linear search instead.
	for (int elf_ap = 4; ; ++elf_ap) {
		auto [outcome, elves_won] = simulate_combat(map, elf_ap);

		if (elves_won) {
			std::cout << "with AP " << elf_ap << " elves won with outcome " << outcome << '\n';
			break;
		}
	}

	return 0;
}
