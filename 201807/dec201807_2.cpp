//
// Advent of Code 2018, day 7, part two
//

// In the example with two workers, at second 4 both B and D are available,
// but as only one worker is free, D has to wait. Interestingly, this
// situation does not occur with the actual input and 5 workers; you can add
// more workers and the time stays the same.

#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int num_workers = 5;
constexpr int base_step_duration = 60;

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

	std::priority_queue<std::pair<int, char>, std::vector<std::pair<int, char>>, std::greater<>> pqueue;
	std::priority_queue<char, std::vector<char>, std::greater<>> waiting;

	// Add steps with no requirements to queue
	for (const auto &[ch, req_adj] : adj) {
		if (req_adj.first == 0) {
			waiting.push(ch);
		}
	}

	// Start steps that are waiting for a worker
	while (!waiting.empty() && pqueue.size() < num_workers) {
		char ch = waiting.top();
		waiting.pop();

		pqueue.push({base_step_duration + (ch - 'A') + 1, ch});
	}

	std::uint32_t done_steps = 0;

	while (!pqueue.empty()) {
		auto [time, step] = pqueue.top();
		pqueue.pop();

		done_steps |= UINT32_C(1) << (step - 'A');

		std::cout << time << ": done " << step << '\n';

		// Add steps that depend on current and for which we fulfill
		// all requirements
		for (char ch : adj[step].second) {
			if ((done_steps & adj[ch].first) == adj[ch].first) {
				waiting.push(ch);
			}
		}

		// Start steps that are waiting for a worker
		while (!waiting.empty() && pqueue.size() < num_workers) {
			char ch = waiting.top();
			waiting.pop();

			pqueue.push({time + base_step_duration + (ch - 'A') + 1, ch});
		}
	}

	return 0;
}
