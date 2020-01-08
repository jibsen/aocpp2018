//
// Advent of Code 2018, day 8, part one
//

#include <iostream>
#include <iterator>
#include <vector>

std::pair<std::vector<int>::iterator, int> sum_metadata(std::vector<int>::iterator it)
{
	int num_children = *it++;
	int num_metadata = *it++;

	int sum = 0;

	for (int i = 0; i < num_children; ++i) {
		auto [next_it, child_sum] = sum_metadata(it);
		sum += child_sum;
		it = next_it;
	}

	for (int i = 0; i < num_metadata; ++i) {
		sum += *it++;
	}

	return {it, sum};
}

int main()
{
	std::istream_iterator<int> it(std::cin);
	std::istream_iterator<int> end;

	std::vector<int> license(it, end);

	auto [next_it, sum] = sum_metadata(license.begin());

	if (next_it == license.end()) {
		std::cout << sum << '\n';
	}

	return 0;
}
