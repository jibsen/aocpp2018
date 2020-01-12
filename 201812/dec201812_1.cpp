//
// Advent of Code 2018, day 12, part one
//

#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

constexpr int num_rounds = 20;

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

	for (int round = 0; round < num_rounds; ++round) {
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
	}

	int sum = 0;

	for (int i = 0; i < state.size(); ++i) {
		if (state[i] != 0) {
			// Subtracting num_rounds since we added one pot to
			// the front every round
			sum += i - num_rounds;
		}
	}

	std::cout << sum << '\n';

	return 0;
}
