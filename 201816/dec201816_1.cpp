//
// Advent of Code 2018, day 16, part one
//

#include <array>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using Registers = std::array<unsigned int, 4>;

struct Instruction {
	int opcode = -1;
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int c = 0;
};

struct Sample {
	Registers before = {};
	Instruction ins;
	Registers after = {};
};

std::vector<Sample> read_samples()
{
	std::vector<Sample> samples;
	Registers before;
	Registers after;
	Instruction ins;

	while (std::scanf("Before: [%u, %u, %u, %u] ", &before[0], &before[1], &before[2], &before[3]) == 4
	    && std::scanf("%d %u %u %u ", &ins.opcode, &ins.a, &ins.b, &ins.c) == 4
	    && std::scanf("After:  [%u, %u, %u, %u] ", &after[0], &after[1], &after[2], &after[3]) == 4) {
		samples.push_back({before, ins, after});
	}

	return samples;
}

Registers perform_instruction(int opcode, const Instruction &ins, Registers reg)
{
	switch (opcode) {
	case 0: // addr
		reg[ins.c] = reg[ins.a] + reg[ins.b];
		break;
	case 1: // addi
		reg[ins.c] = reg[ins.a] + ins.b;
		break;
	case 2: // mulr
		reg[ins.c] = reg[ins.a] * reg[ins.b];
		break;
	case 3: // muli
		reg[ins.c] = reg[ins.a] * ins.b;
		break;
	case 4: // banr
		reg[ins.c] = reg[ins.a] & reg[ins.b];
		break;
	case 5: // bani
		reg[ins.c] = reg[ins.a] & ins.b;
		break;
	case 6: // borr
		reg[ins.c] = reg[ins.a] | reg[ins.b];
		break;
	case 7: // bori
		reg[ins.c] = reg[ins.a] | ins.b;
		break;
	case 8: // setr
		reg[ins.c] = reg[ins.a];
		break;
	case 9: // seti
		reg[ins.c] = ins.a;
		break;
	case 10: // gtir
		reg[ins.c] = static_cast<unsigned int>(ins.a > reg[ins.b]);
		break;
	case 11: // gtri
		reg[ins.c] = static_cast<unsigned int>(reg[ins.a] > ins.b);
		break;
	case 12: // gtrr
		reg[ins.c] = static_cast<unsigned int>(reg[ins.a] > reg[ins.b]);
		break;
	case 13: // eqir
		reg[ins.c] = static_cast<unsigned int>(ins.a == reg[ins.b]);
		break;
	case 14: // eqri
		reg[ins.c] = static_cast<unsigned int>(reg[ins.a] == ins.b);
		break;
	case 15: // eqrr
		reg[ins.c] = static_cast<unsigned int>(reg[ins.a] == reg[ins.b]);
		break;
	default:
		std::cerr << "unknown opcode " << opcode << '\n';
		exit(1);
		break;
	}

	return reg;
}

int main()
{
	auto samples = read_samples();

	std::cout << samples.size() << " samples\n";

	int num_matching_at_least_three = 0;

	for (const auto &sample : samples) {
		int num_possible = 0;

		for (int opcode = 0; opcode < 16; ++opcode) {
			if (perform_instruction(opcode, sample.ins, sample.before) == sample.after) {
				++num_possible;
			}
		}

		num_matching_at_least_three += static_cast<int>(num_possible >= 3);
	}

	std::cout << num_matching_at_least_three << " behave like three or more opcodes\n";

	return 0;
}
