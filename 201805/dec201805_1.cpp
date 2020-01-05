//
// Advent of Code 2018, day 5, part one
//

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

	perform_reactions(polymer);

	std::cout << polymer.size() << '\n';

	return 0;
}
