//
// Advent of Code 2018, day 2, part two
//

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

int hamming_distance(const std::string &lhs, const std::string &rhs)
{
	if (lhs.size() < rhs.size()) {
		return hamming_distance(rhs, lhs);
	}

	int num = static_cast<int>(lhs.size() - rhs.size());

	for (int i = 0; i < rhs.size(); ++i) {
		num += static_cast<int>(lhs[i] != rhs[i]);
	}

	return num;
}

int main()
{
	auto ids = read_box_ids();

	for (int j = 1; j < ids.size(); ++j) {
		for (int i = 0; i < j; ++i) {
			if (hamming_distance(ids[i], ids[j]) == 1) {
				std::cout << ids[i] << '\n';
				std::cout << ids[j] << '\n';
			}
		}
	}

	return 0;
}
