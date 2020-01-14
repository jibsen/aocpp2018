//
// Advent of Code 2018, day 14, part two
//

#include <iostream>
#include <vector>

constexpr int score_sequence = 909441;
constexpr int num_digits = 6;
constexpr int digits_mod = 1000000;

int main()
{
	std::vector<int> scoreboard;

	scoreboard.push_back(3);
	scoreboard.push_back(7);

	int last_digits = 37;
	int last_index = 1;

	int elf_a = 0;
	int elf_b = 1;

	for (;;) {
		int sum = scoreboard[elf_a] + scoreboard[elf_b];
		if (sum > 9) {
			scoreboard.push_back(sum / 10);
			sum %= 10;
		}
		scoreboard.push_back(sum);

		elf_a = (elf_a + scoreboard[elf_a] + 1) % scoreboard.size();
		elf_b = (elf_b + scoreboard[elf_b] + 1) % scoreboard.size();

		while (last_index < scoreboard.size()) {
			last_digits = (last_digits * 10 + scoreboard[last_index++]) % digits_mod;
			if (last_digits == score_sequence) {
				std::cout << last_index - num_digits << '\n';
				exit(0);
			}
		}
	}

	return 0;
}
