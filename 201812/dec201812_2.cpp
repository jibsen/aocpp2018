//
// Advent of Code 2018, day 12, part two
//

// Since the number of generations is too large to handle, we must hope
// that either it is possible to directly compute the state at a certain
// generation, or there is some pattern to the sum produced.
//
// Printing out the sum for the first few hundred generations shows that the
// value fluctuates initially, but after generation 94 the difference between
// consecutive sums is always 22.

#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

constexpr int num_rounds = 200;

std::deque<int> read_initial_state()
{
	std::deque<int> state;

	std::string line;

	std::getline(std::cin, line);

	if (auto p = line.find(':'); p != std::string::npos) {
		p += 2;

		while (p < line.size()) {
			state.push_back(line[p] == '#' ? 1 : 0);
			++p;
		}
	}

	return state;
}

std::array<int, 32> read_rules_lookup()
{
	std::array<int, 32> rule_lookup = {};

	std::string pattern;
	std::string sep;
	std::string result;

	while (std::cin >> pattern >> sep >> result && sep == "=>") {
		unsigned int mask = 0;

		for (char ch : pattern) {
			mask = (mask << 1) | ((ch == '#') ? 1U : 0U);
		}

		rule_lookup[mask] = result == "#" ? 1 : 0;
	}

	return rule_lookup;
}

int main()
{
	auto state = read_initial_state();

	auto lookup = read_rules_lookup();

	std::deque<int> next;

	int round = 0;
	int prev_sum = 0;
	int prev_difference = 0;
	int num_repeats = 0;

	for (; round < num_rounds; ++round) {
		// Add empty pots to front and back of state so loop
		// below will apply the rules expanding by one pot in
		// each direction
		state.push_front(0);
		state.push_back(0);
		state.push_back(0);
		state.push_back(0);

		unsigned int mask = state[1];

		for (int i = 0; i < state.size() - 2; ++i) {
			mask = (mask << 1) | static_cast<unsigned int>(state[i + 2]);
			next.push_back(lookup[mask & 0x1FU]);
		}

		state.swap(next);
		next.clear();

		int sum = 0;

		for (int i = 0; i < state.size(); ++i) {
			if (state[i] != 0) {
				sum += i - (round + 1);
			}
		}

		if (sum - prev_sum == prev_difference) {
			std::cout << prev_difference << " repeated @ round " << round << '\n';

			if (++num_repeats == 5) {
				break;
			}
		}
		else {
			num_repeats = 0;
		}

		prev_difference = sum - prev_sum;
		prev_sum = sum;
	}

	std::cout << (50000000000LL - round) * prev_difference + prev_sum << '\n';

	return 0;
}
