#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_set>

std::vector<int> read_frequency_changes()
{
	std::istream_iterator<int> it(std::cin);
	std::istream_iterator<int> end;

	std::vector<int> fcl;

	std::copy(it, end, std::back_inserter(fcl));

	return fcl;
}

int main()
{
	std::vector<int> fcl = read_frequency_changes();

	std::unordered_set<int> seen;
	int freq = 0;

	for (;;) {
		for (int fc : fcl) {
			freq += fc;

			if (auto [it, success] = seen.insert(freq); !success) {
				std::cout << freq << '\n';
				exit(0);
			}
		}
	}

	return 0;
}
