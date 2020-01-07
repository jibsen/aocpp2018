//
// Advent of Code 2018, day 7, part one
//

#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<char, std::pair<std::uint32_t, std::vector<char>>> read_instructions()
{
	std::unordered_map<char, std::pair<std::uint32_t, std::vector<char>>> adj;
	std::string line;

	while (std::getline(std::cin, line)) {
		adj[line[5]].second.push_back(line[36]);
		adj[line[36]].first |= UINT32_C(1) << (line[5] - 'A');
	}

	return adj;
}

int main()
{
	auto adj = read_instructions();

	std::priority_queue<char, std::vector<char>, std::greater<>> pqueue;

	// Add steps with no requirements to queue
	for (const auto &[ch, req_adj] : adj) {
		if (req_adj.first == 0) {
			pqueue.push(ch);
		}
	}

	std::uint32_t done_steps = 0;

	while (!pqueue.empty()) {
		char step = pqueue.top();
		pqueue.pop();

		done_steps |= UINT32_C(1) << (step - 'A');

		std::cout << step;

		// Add steps that depend on current and for which we fulfill
		// all requirements
		for (char ch : adj[step].second) {
			if ((done_steps & adj[ch].first) == adj[ch].first) {
				pqueue.push(ch);
			}
		}
	}

	std::cout << '\n';

	return 0;
}
