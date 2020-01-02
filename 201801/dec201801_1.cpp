//
// Advent of Code 2018, day 1, part one
//

#include <iostream>
#include <iterator>
#include <numeric>

int main()
{
	std::istream_iterator<int> it(std::cin);
	std::istream_iterator<int> end;

	std::cout << std::accumulate(it, end, 0) << '\n';

	return 0;
}
