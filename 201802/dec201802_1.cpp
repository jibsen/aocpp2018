//
// Advent of Code 2018, day 2, part one
//

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> read_box_ids()
{
	std::vector<std::string> ids;

	for (std::string s; std::getline(std::cin, s); ) {
		ids.push_back(s);
	}

	return ids;
}

int main()
{
	auto ids = read_box_ids();

	int num_appear_twice = 0;
	int num_appear_thrice = 0;

	for (const auto &id : ids) {
		std::array<int, 'z' - 'a' + 1> freq = {};

		for (char ch : id) {
			freq[ch - 'a']++;
		}

		if (std::find(freq.begin(), freq.end(), 2) != freq.end()) {
			++num_appear_twice;
		}

		if (std::find(freq.begin(), freq.end(), 3) != freq.end()) {
			++num_appear_thrice;
		}
	}

	std::cout << num_appear_twice * num_appear_thrice << '\n';

	return 0;
}
