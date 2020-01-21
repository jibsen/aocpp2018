//
// Advent of Code 2018, day 21, part two
//

// The program in the input roughly translates to:
//
//     do {
//         R1 = R3 | 0x00010000;
//         R3 = 10373714;
//
//         while (R1 > 0) {
//             R3 = ((R3 + (R1 & 0xFF)) * 65899) & 0x00FFFFFF;
//
//             R1 /= 256;
//         }
//     } while (R3 != R0);
//
// The values produced in R3 repeat after a while (the cycle does not include
// 0, which is why it runs forever by default). If we find the value before
// the first repeat of R3, we have the longest possible run that halts.

#include <iostream>
#include <unordered_set>

int find_R3_before_repeat()
{
	std::unordered_set<unsigned int> seen;

	unsigned int R0 = 0;
	unsigned int R1 = 0;
	unsigned int R3 = 0;

	unsigned int prev = 0;

	do {
		R1 = R3 | 0x00010000;
		R3 = 10373714;

		while (R1 > 0) {
			R3 = ((R3 + (R1 & 0xFF)) * 65899) & 0x00FFFFFF;

			R1 /= 256;
		}

		if (auto [it, success] = seen.insert(R3); !success) {
			return prev;
		}

		prev = R3;
	} while (R3 != R0);

	return 0;
}

int main()
{
	std::cout << find_R3_before_repeat() << '\n';

	return 0;
}
