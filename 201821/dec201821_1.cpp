//
// Advent of Code 2018, day 21, part one
//

// The only operation involving R0 is the comparison in instruction 28
// (eqrr 3 0 5), the code there halts the program if R0 is equal to R3.
//
// So if we run the program until the first time we encounter this
// instruction and print the value in R3, we know what to put in R0 to
// stop as soon as possible.

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Registers = std::array<unsigned int, 7>;

struct Instruction {
	int opcode = -1;
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int c = 0;
};

using Program = std::vector<Instruction>;

const std::unordered_map<std::string, int> opcode_lookup = {
	{ "addr", 0 }, { "addi", 1 },
	{ "mulr", 2 }, { "muli", 3 },
	{ "banr", 4 }, { "bani", 5 },
	{ "borr", 6 }, { "bori", 7 },
	{ "setr", 8 }, { "seti", 9 },
	{ "gtir", 10 }, { "gtri", 11 }, { "gtrr", 12 },
	{ "eqir", 13 }, { "eqri", 14 }, { "eqrr", 15 }
};

std::pair<Program, int> read_program_ipreg()
{
	Program program;
	Instruction ins;
	std::string opcode;
	int ip_reg = -1;

	// Using opcode to consume #ip
	std::cin >> opcode >> ip_reg;

	while (std::cin >> opcode >> ins.a >> ins.b >> ins.c) {
		ins.opcode = opcode_lookup.at(opcode);
		program.push_back(ins);
	}

	return {program, ip_reg};
}

class Computer {
	Registers reg = {};
	Program program;
	int ip_reg = 6;
	int ip = 0;
	bool halt = false;

public:
	Computer(Program program_, int ip_reg_)
	 : program(std::move(program_)), ip_reg(ip_reg_) {}

	void step();

	int run() {
		while (!halt) {
			step();
		}

		return reg[0];
	};
};

void Computer::step()
{
	reg[ip_reg] = ip;

	const auto &ins = program[ip];

	switch (ins.opcode) {
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
		std::cerr << "unknown opcode " << ins.opcode << '\n';
		exit(1);
		break;
	}

	ip = reg[ip_reg];
	++ip;

	if (ip < 0 || ip >= program.size()) {
		halt = true;
	}

	if (ip == 28) {
		std::cout << reg[3] << '\n';
		halt = true;
	}
}

int main()
{
	auto [program, ip_reg] = read_program_ipreg();

	std::cout << program.size() << " instructions read\n";

	Computer c(program, ip_reg);

	c.run();

	return 0;
}
