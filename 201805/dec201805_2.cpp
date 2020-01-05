//
// Advent of Code 2018, day 5, part two
//

#include <algorithm>
#include <cctype>
#include <iostream>
#include <list>
#include <string>

void perform_reactions(std::list<char> &polymer)
{
	for (auto it = polymer.begin(); it != polymer.end(); ) {
		if (it != polymer.begin()) {
			auto prev = std::prev(it);

			if (*it != *prev && std::toupper(*it) == std::toupper(*prev)) {
				polymer.erase(prev);
				it = polymer.erase(it);
				continue;
			}
		}
		++it;
	}
}

int main()
{
	std::string line;
	std::getline(std::cin, line);

	std::list<char> polymer(line.begin(), line.end());

	const std::string units = "abcdefghijklmnopqrstuvwxyz";

	int min_length = std::numeric_limits<int>::max();

	for (char remove_ch : units) {
		std::list<char> new_polymer(polymer);

		new_polymer.remove_if([&](char ch) { return std::tolower(ch) == remove_ch; });

		perform_reactions(new_polymer);

		min_length = std::min(min_length, static_cast<int>(new_polymer.size()));
	}

	std::cout << min_length << '\n';

	return 0;
}
