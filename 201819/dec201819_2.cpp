//
// Advent of Code 2018, day 19, part two
//

// Setting register 0 to 1 and letting the program run for a few minutes,
// we quickly realize that it's going to take a long time to complete.
//
// If we check the contents of the six registers in part one every time the
// value in register 0 changes, we get:
//
//       R0        R1        R2        R3        R4        R5
//     ------------------------------------------------------
//        1       892         7       892         1         1
//        3       892         7       446         1         2
//        7       892         7       223         1         4
//      230       892         7         4         1       223
//      676       892         7         2         1       446
//     1568       892         7         1         1       892
//
// We notice that R1 is constant, and that R1 = R3 * R5. In fact, the values
// in R3 and R5 are all the divisors of 892. Also, R0 is the partial sum of
// the values in R5 (due to the instruction addr 5 0 0).
//
// So our guess is that the program is computing the sum of all divisors of
// the number in R1. Looking at the first few times R0 changes in part two
// supports this:
//
//       R0        R1        R2        R3        R4        R5
//     ------------------------------------------------------
//        1  10551292         7  10551292         1         1
//        3  10551292         7   5275646         1         2
//        7  10551292         7   2637823         1         4
//       68  10551292         7    172972         1        61
//      151  10551292         7    127124         1        83
//      273  10551292         7     86486         1       122
//      439  10551292         7     63562         1       166
//      683  10551292         7     43243         1       244
//     1015  10551292         7     31781         1       332

#include <cmath>
#include <iostream>

long long sum_divisors(long long n)
{
	long long limit = static_cast<long long>(std::sqrt(n));
	long long sum = 0;

	for (long long i = 1; i <= limit; ++i) {
		if (n % i == 0) {
			sum += i;
			sum += n / i;
		}
	}

	return sum;
}

int main()
{
	std::cout << sum_divisors(10551292LL) << '\n';

	return 0;
}
