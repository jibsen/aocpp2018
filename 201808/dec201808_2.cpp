//
// Advent of Code 2018, day 8, part two
//

#include <iostream>
#include <iterator>
#include <vector>

std::pair<std::vector<int>::iterator, int> compute_value(std::vector<int>::iterator it)
{
	int num_children = *it++;
	int num_metadata = *it++;

	std::vector<int> child_values;

	for (int i = 0; i < num_children; ++i) {
		auto [next_it, value] = compute_value(it);
		child_values.push_back(value);
		it = next_it;
	}

	int value = 0;

	for (int i = 0; i < num_metadata; ++i) {
		if (num_children == 0) {
			value += *it++;
		}
		else {
			int idx = *it++ - 1;
			if (idx >= 0 && idx < child_values.size()) {
				value += child_values[idx];
			}
		}
	}

	return {it, value};
}

int main()
{
	std::istream_iterator<int> it(std::cin);
	std::istream_iterator<int> end;

	std::vector<int> license(it, end);

	auto [next_it, value] = compute_value(license.begin());

	if (next_it == license.end()) {
		std::cout << value << '\n';
	}

	return 0;
}
