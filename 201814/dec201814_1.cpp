//
// Advent of Code 2018, day 14, part one
//

#include <iostream>
#include <vector>

constexpr int num_recipies = 909441;

int main()
{
	std::vector<int> scoreboard;

	scoreboard.reserve(2 * num_recipies + 10);

	scoreboard.push_back(3);
	scoreboard.push_back(7);

	int elf_a = 0;
	int elf_b = 1;

	while (scoreboard.size() < num_recipies + 10) {
		int sum = scoreboard[elf_a] + scoreboard[elf_b];
		if (sum > 9) {
			scoreboard.push_back(sum / 10);
			sum %= 10;
		}
		scoreboard.push_back(sum);

		elf_a = (elf_a + scoreboard[elf_a] + 1) % scoreboard.size();
		elf_b = (elf_b + scoreboard[elf_b] + 1) % scoreboard.size();
	}

	for (int i = num_recipies; i < num_recipies + 10; ++i) {
		std::cout << scoreboard[i];
	}
	std::cout << '\n';

	return 0;
}
