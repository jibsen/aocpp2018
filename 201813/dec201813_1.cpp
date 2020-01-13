//
// Advent of Code 2018, day 13, part one
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Cart {
	int y = 0;
	int x = 0;
	int next_dir = 0;
	char ch_under = ' ';
};

bool operator<(const Cart &lhs, const Cart &rhs) {
	return lhs.y != rhs.y ? lhs.y < rhs.y : lhs.x < rhs.x;
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

bool is_cart(char ch)
{
	return ch == '<' || ch == '>' || ch == '^' || ch == 'v';
}

void update_cart(std::vector<std::string> &map, Cart &cart)
{
	char cart_ch = map[cart.y][cart.x];
	map[cart.y][cart.x] = cart.ch_under;

	switch (cart_ch) {
	case '<':
		cart.x--;
		break;
	case '>':
		cart.x++;
		break;
	case '^':
		cart.y--;
		break;
	case 'v':
		cart.y++;
		break;
	default:
		std::cerr << "unknown cart type '" << cart_ch << "'\n";
		exit(1);
	}

	cart.ch_under = map[cart.y][cart.x];

	switch (cart.ch_under) {
	case '/':
		cart_ch = cart_ch == '<' ? 'v'
		        : cart_ch == '>' ? '^'
		        : cart_ch == '^' ? '>'
		        : cart_ch == 'v' ? '<'
			: '?';
		break;
	case '\\':
		cart_ch = cart_ch == '<' ? '^'
		        : cart_ch == '>' ? 'v'
		        : cart_ch == '^' ? '<'
		        : cart_ch == 'v' ? '>'
			: '?';
		break;
	case '+':
		switch (cart.next_dir) {
		case 0:
			cart_ch = cart_ch == '<' ? 'v'
			        : cart_ch == '>' ? '^'
			        : cart_ch == '^' ? '<'
			        : cart_ch == 'v' ? '>'
				: '?';
			break;
		case 1:
			break;
		case 2:
			cart_ch = cart_ch == '<' ? '^'
			        : cart_ch == '>' ? 'v'
			        : cart_ch == '^' ? '>'
			        : cart_ch == 'v' ? '<'
				: '?';
			break;
		}
		cart.next_dir = (cart.next_dir + 1) % 3;
		break;
	case '<':
	case '>':
	case '^':
	case 'v':
		std::cout << "crash at " << cart.x << ',' << cart.y << '\n';
		exit(0);
		break;
	default:
		break;
	}

	map[cart.y][cart.x] = cart_ch;
}

int main()
{
	auto map = read_map();

	std::vector<Cart> carts;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (is_cart(map[y][x])) {
				char ch_under = map[y][x] == '<' || map[y][x] == '>' ? '-' : '|';
				carts.push_back(Cart{y, x, 0, ch_under});
			}
		}
	}

	std::cout << carts.size() << " carts\n";

	for (int tick = 0; tick < 1000; ++tick) {
		std::sort(carts.begin(), carts.end());

		for (auto &cart : carts) {
			update_cart(map, cart);
		}
	}

	return 0;
}
