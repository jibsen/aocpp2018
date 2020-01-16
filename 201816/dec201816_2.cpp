//
// Advent of Code 2018, day 16, part two
//

#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
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

using Program = std::vector<Instruction>;

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

Program read_program()
{
	Program program;
	Instruction ins;

	while (std::cin >> ins.opcode >> ins.a >> ins.b >> ins.c) {
		program.push_back(ins);
	}

	return program;
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

constexpr int count_bits_set(std::uint32_t v)
{
	int count = 0;

	while (v != 0) {
		v &= v - 1;
		++count;
	}

	return count;
}

constexpr int lowest_bit_set(std::uint32_t v)
{
	if (v == 0) {
		return 32;
	}

	int count = 0;

	while ((v & 1U) == 0) {
		v >>= 1;
		++count;
	}

	return count;
}

std::vector<int> opcode_lookup_from_possibilities(std::vector<std::uint32_t> possibilities)
{
	std::vector<int> lookup(possibilities.size(), -1);

	for (;;) {
		// Find opcode with only one possible value
		auto it = find_if(possibilities.begin(), possibilities.end(),
		                  [](const auto &v) { return count_bits_set(v) == 1; });

		if (it == possibilities.end()) {
			break;
		}

		std::uint32_t mask = *it;
		int value = lowest_bit_set(mask);

		lookup[std::distance(possibilities.begin(), it)] = value;

		// Remove that value from the possibilities of all opcodes
		for (auto &v : possibilities) {
			v &= ~mask;
		}
	}

	return lookup;
}

int main()
{
	auto samples = read_samples();

	std::cout << samples.size() << " samples read\n";

	std::vector<std::uint32_t> possibilities(16, 0);

	for (const auto &sample : samples) {
		std::uint32_t mask = 0;

		for (int opcode = 0; opcode < 16; ++opcode) {
			if (perform_instruction(opcode, sample.ins, sample.before) == sample.after) {
				mask |= 1U << opcode;
			}
		}

		possibilities[sample.ins.opcode] |= mask;
	}

	auto opcode_lookup = opcode_lookup_from_possibilities(possibilities);

	auto program = read_program();

	std::cout << program.size() << " instructions read\n";

	Registers reg = {};

	for (const auto &ins : program) {
		reg = perform_instruction(opcode_lookup[ins.opcode], ins, reg);
	}

	std::cout << "register 0 = " << reg[0] << '\n';

	return 0;
}
